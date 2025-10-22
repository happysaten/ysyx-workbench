/* verilator lint_off DECLFILENAME */

// AXI4-Lite CLINT模块
// 作为slave设备，提供只读的mtime寄存器

module clint #(
    parameter int MTIME_ADDR = 32'h0a000048  // mtime基地址（8字节对齐）
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
    logic addr_match_lo;
    logic addr_match_hi;

    assign addr_match_lo = (s.araddr == MTIME_ADDR);  // mtime低32位
    assign addr_match_hi = (s.araddr == MTIME_ADDR + 4);  // mtime高32位
    assign addr_match_ar = addr_match_lo || addr_match_hi;
    assign addr_match_aw = 1'b0;  // CLINT不支持写操作

    // 64位mtime寄存器
    logic [63:0] mtime;

    // mtime每周期加1
    always_ff @(posedge clk) begin
        if (reset) begin
            mtime <= 64'h0;
        end else begin
            mtime <= mtime + 64'h1;
        end
    end

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

    // 读地址通道
    assign s.arready = (rd_state == IDLE_RD);

    // 读数据通道 - 根据地址返回mtime的低32位或高32位
    logic addr_match_lo_reg, addr_match_hi_reg;
    always @(posedge clk) begin
        if (reset) begin
            addr_match_lo_reg <= 1'b0;
            addr_match_hi_reg <= 1'b0;
        end else if (s.arvalid && s.arready) begin
            addr_match_lo_reg <= addr_match_lo;
            addr_match_hi_reg <= addr_match_hi;
        end
    end
    assign s.rvalid = (rd_state == WAIT_RRESP);
    assign s.rdata   = (rd_state == WAIT_RRESP) ?
                       (addr_match_lo_reg? mtime[31:0] :
                        addr_match_hi_reg ? mtime[63:32] : 32'h0) : 32'h0;
    assign s.rresp = (rd_state == WAIT_RRESP) && (!addr_match_ar) ? 2'b10 : 2'b00;  // SLVERR or OKAY

    // 写地址通道
    assign s.awready = (wr_state == IDLE_WR);

    // 写数据通道
    assign s.wready = (wr_state == IDLE_WR || wr_state == WAIT_WDATA);

    // 写回复通道 - CLINT不支持写操作，返回错误
    assign s.bvalid = (wr_state == WAIT_WRESP);
    assign s.bresp = 2'b10;  // SLVERR - 从设备错误

endmodule
