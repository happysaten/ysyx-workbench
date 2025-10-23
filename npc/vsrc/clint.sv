/* verilator lint_off DECLFILENAME */

// AXI4-Lite CLINT模块
// 作为slave设备，提供只读的mtime寄存器

module clint #(
    parameter int MTIME_ADDR = 32'ha0000048,  // mtime基地址
    parameter int CLINT_SIZE = 32'h8          // CLINT地址空间大小
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

    // 地址匹配信号
    logic addr_match_ar;

    assign addr_match_ar = (s.araddr >= MTIME_ADDR) && (s.araddr < MTIME_ADDR + CLINT_SIZE);

    // mtime寄存器 - 固定为64位
    logic [63:0] mtime;

    // mtime每周期加1
    always_ff @(posedge clk) begin
        if (reset) mtime <= '0;
        else mtime <= mtime + 1'b1;
    end

    // 状态机更新逻辑
    always_ff @(posedge clk) begin
        if (reset) begin
            rd_state <= IDLE_RD;
        end else begin
            rd_state <= next_rd_state;
        end
    end

    // 读状态机next逻辑
    always_comb begin
        unique case (rd_state)
            IDLE_RD: next_rd_state = (s.arvalid && s.arready) ? WAIT_RRESP : IDLE_RD;
            WAIT_RRESP: next_rd_state = (s.rvalid && s.rready) ? IDLE_RD : WAIT_RRESP;
        endcase
    end

    import "DPI-C" function void difftest_skip_ref();

    // 读地址通道
    assign s.arready = (rd_state == IDLE_RD);

    // 读数据通道 - 根据地址偏移返回mtime的相应部分
    logic addr_match_ar_reg;
    logic [31:0] addr_offset;

    always @(posedge clk) begin
        if (reset) begin
            addr_match_ar_reg <= 1'b0;
            addr_offset <= 32'h0;
        end else if (s.arvalid && s.arready) begin
            addr_match_ar_reg <= addr_match_ar;
            addr_offset <= s.araddr - MTIME_ADDR;
        end
    end
    always_comb assert (addr_offset == 32'h0 || addr_offset == 32'h4);

    assign s.rvalid = (rd_state == WAIT_RRESP);
    assign s.rdata  = addr_match_ar_reg ? mtime[addr_offset*8+:32] : 32'h0;
    assign s.rresp  = addr_match_ar_reg ? 2'b00 : 2'b10;  // OKAY or SLVERR

    always_comb if (rd_state == WAIT_RRESP) difftest_skip_ref();

    // 写通道 - CLINT不支持写操作
    assign s.awready = 1'b0;
    assign s.wready  = 1'b0;
    assign s.bvalid  = 1'b0;
    assign s.bresp   = 2'b10;  // SLVERR - 从设备错误

endmodule
