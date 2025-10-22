/* verilator lint_off DECLFILENAME */

// AXI4-Lite UART模块
// 作为slave设备，支持写操作输出字符

module uart #(
    parameter int UART_ADDR = 32'ha00003f8
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
    typedef enum logic [1:0] {
        IDLE_WR,
        WAIT_WDATA,
        WAIT_WRESP
    } wr_state_t;

    wr_state_t wr_state, next_wr_state;

    // 地址匹配信号
    logic addr_match_aw;
    logic addr_match_ar;

    assign addr_match_aw = (s.awaddr == UART_ADDR);
    assign addr_match_ar = (s.araddr == UART_ADDR);

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
            IDLE_WR:
            next_wr_state = (s.awvalid && s.awready) ? (s.wvalid && s.wready ? WAIT_WRESP: WAIT_WDATA) : IDLE_WR;
            WAIT_WDATA: next_wr_state = (s.wvalid && s.wready) ? WAIT_WRESP : WAIT_WDATA;
            WAIT_WRESP: next_wr_state = (s.bvalid && s.bready) ? IDLE_WR : WAIT_WRESP;
        endcase
    end

    import "DPI-C" function void difftest_skip_ref();

    // 读地址通道
    assign s.arready = (rd_state == IDLE_RD);

    // 读数据通道
    assign s.rvalid  = (rd_state == WAIT_RRESP);
    assign s.rdata   = 32'h0;  // UART 读取返回0
    assign s.rresp   = (rd_state == WAIT_RRESP) ? (!addr_match_ar) : 1'b0;
    always_comb if (rd_state == WAIT_RRESP) difftest_skip_ref();

    // 写地址通道
    assign s.awready = (wr_state == IDLE_WR);

    // 写数据通道
    assign s.wready  = (wr_state == IDLE_WR || wr_state == WAIT_WDATA);

    // 写回复通道
    assign s.bvalid  = (wr_state == WAIT_WRESP);
    assign s.bresp   = !addr_match_aw;

    // UART 字符输出逻辑
    logic [31:0] aw_addr_reg;
    logic [ 7:0] serial_base;  // 模拟C代码中的serial_base[0]

    always_ff @(posedge clk) begin
        if (reset) begin
            aw_addr_reg <= 32'h0;
        end else if (s.awvalid && s.awready) begin
            aw_addr_reg <= s.awaddr;
            difftest_skip_ref();
        end
    end

    // 写入数据到寄存器
    always_ff @(posedge clk) begin
        if (reset) begin
            serial_base <= 8'h0;
        end else if (s.wvalid && s.wready) begin
            if (aw_addr_reg == UART_ADDR || (s.awvalid && addr_match_aw)) begin
                serial_base <= s.wdata[7:0];
                $write("%c", s.wdata[7:0]);
                difftest_skip_ref();
            end
        end
    end

endmodule
