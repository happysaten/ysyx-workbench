/* verilator lint_off DECLFILENAME */

// AXI4-Lite UART模块
// 作为slave设备，支持写操作输出字符

module uart #(
    parameter int UART_ADDR = 32'ha00003f8,
    parameter int UART_SIZE = 32'h8  // UART地址空间大小
) (
    input logic             clk,
    input logic             reset,
          axi_lite_if.slave s       // 使用interface替代所有独立的AXI信号
);

    // 定义读状态枚举
    typedef enum logic {
        IDLE_RD,
        WAIT_RRESP
    } rd_state_t;

    rd_state_t rd_state, next_rd_state;

    // 定义写状态枚举
    typedef enum logic [2:0] {
        IDLE_WR,
        WAIT_WDATA,
        WAIT_WADDR,
        WAIT_WRESP
    } wr_state_t;

    wr_state_t wr_state, next_wr_state;

    // 地址匹配信号
    logic addr_match_aw;
    logic addr_match_ar;

    assign addr_match_aw = (s.awaddr >= UART_ADDR) && (s.awaddr < UART_ADDR + UART_SIZE);
    assign addr_match_ar = (s.araddr >= UART_ADDR) && (s.araddr < UART_ADDR + UART_SIZE);

    // 状态机更新逻辑
    always_ff @(posedge clk) begin
        if (reset) begin
            rd_state <= IDLE_RD;
            wr_state <= IDLE_WR;
        end else begin
            rd_state <= next_rd_state;
            wr_state <= next_wr_state;
        end
    end

    // 读状态机next逻辑
    always_comb begin
        unique case (rd_state)
            IDLE_RD: next_rd_state = (s.arvalid && s.arready) ? WAIT_RRESP : IDLE_RD;
            WAIT_RRESP: next_rd_state = (s.rvalid && s.rready) ? IDLE_RD : WAIT_RRESP;
        endcase
    end

    // 写状态机next逻辑
    always_comb begin
        unique case (wr_state)
            IDLE_WR: begin
                if (s.awvalid && s.awready && s.wvalid && s.wready) begin
                    // 地址和数据同时到达
                    next_wr_state = WAIT_WRESP;
                end else if (s.awvalid && s.awready) begin
                    // 只有地址到达
                    next_wr_state = WAIT_WDATA;
                end else if (s.wvalid && s.wready) begin
                    // 只有数据到达
                    next_wr_state = WAIT_WADDR;
                end else begin
                    next_wr_state = IDLE_WR;
                end
            end
            WAIT_WDATA: next_wr_state = (s.wvalid && s.wready) ? WAIT_WRESP : WAIT_WDATA;
            WAIT_WADDR: next_wr_state = (s.awvalid && s.awready) ? WAIT_WRESP : WAIT_WADDR;
            WAIT_WRESP: next_wr_state = (s.bvalid && s.bready) ? IDLE_WR : WAIT_WRESP;
        endcase
    end

    import "DPI-C" function void difftest_skip_ref();

    // 读地址通道
    assign s.arready = (rd_state == IDLE_RD);

    // 读数据通道
    logic addr_match_ar_reg;
    always @(posedge clk) begin
        if (reset) begin
            addr_match_ar_reg <= 1'b0;
        end else if (s.arvalid && s.arready) begin
            addr_match_ar_reg <= addr_match_ar;
        end
    end
    assign s.rvalid = (rd_state == WAIT_RRESP);
    assign s.rdata  = 32'h0;  // UART 读取返回0
    assign s.rresp  = addr_match_ar_reg ? 2'b00 : 2'b10;  // OKAY or SLVERR
    // always_comb if (rd_state == WAIT_RRESP) difftest_skip_ref();
    always_comb if (rd_state == WAIT_RRESP) $fatal("do not support read");

    logic [31:0] wr_addr_reg;
    logic [31:0] wr_data_reg;
    logic wr_addr_received, wr_data_received;
    logic addr_match_aw_reg;
    logic addr_is_base_reg;  // 是否是基地址（第一个字节）
    logic [7:0] serial_base;  // 模拟C代码中的serial_base[0]

    // 保存写地址
    always_ff @(posedge clk) begin
        if (reset) begin
            wr_addr_reg <= 32'h0;
            wr_addr_received <= 1'b0;
            addr_match_aw_reg <= 1'b0;
            addr_is_base_reg <= 1'b0;
        end else if (s.awvalid && s.awready) begin
            wr_addr_reg <= s.awaddr;
            wr_addr_received <= 1'b1;
            addr_match_aw_reg <= addr_match_aw;
            addr_is_base_reg <= (s.awaddr == UART_ADDR);
            difftest_skip_ref();
        end else if (wr_state == IDLE_WR) begin
            wr_addr_received <= 1'b0;
        end
    end

    // 保存写数据
    always_ff @(posedge clk) begin
        if (reset) begin
            wr_data_reg <= 32'h0;
            wr_data_received <= 1'b0;
        end else if (s.wvalid && s.wready) begin
            wr_data_reg <= s.wdata;
            wr_data_received <= 1'b1;
        end else if (wr_state == IDLE_WR) begin
            wr_data_received <= 1'b0;
        end
    end

    // 写地址通道
    assign s.awready = (wr_state == IDLE_WR) || (wr_state == WAIT_WADDR);

    // 写数据通道
    assign s.wready  = (wr_state == IDLE_WR) || (wr_state == WAIT_WDATA);

    // 写回复通道
    logic final_addr_match;
    logic final_addr_is_base;
    logic [31:0] final_waddr;
    logic [31:0] final_wdata;

    assign final_waddr = wr_addr_received ? wr_addr_reg : s.awaddr;
    assign final_wdata = wr_data_received ? wr_data_reg : s.wdata;
    assign final_addr_match = wr_addr_received ? addr_match_aw_reg : addr_match_aw;
    assign final_addr_is_base = wr_addr_received ? addr_is_base_reg : (s.awaddr == UART_ADDR);

    assign s.bvalid = (wr_state == WAIT_WRESP);
    // 只有在地址匹配且是基地址时才返回OKAY，否则返回SLVERR
    assign s.bresp = (final_addr_match && final_addr_is_base) ? 2'b00 : 2'b10;

    // 写入数据到寄存器
    logic write_complete;
    assign write_complete = (wr_state == WAIT_WDATA && s.wvalid && s.wready) ||
                           (wr_state == WAIT_WADDR && s.awvalid && s.awready) ||
                           (wr_state == IDLE_WR && s.awvalid && s.awready && s.wvalid && s.wready);

    always_ff @(posedge clk) begin
        if (reset) begin
            serial_base <= 8'h0;
        end else if (write_complete) begin
            if (final_addr_match) begin
                if (final_addr_is_base) begin
                    serial_base <= final_wdata[7:0];
                    $write("%c", final_wdata[7:0]);
                    difftest_skip_ref();
                end else begin
                    $fatal("do not support offset = %h", final_waddr - UART_ADDR);
                end
            end
        end
    end

endmodule
