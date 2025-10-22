/* verilator lint_off DECLFILENAME */

// AXI4-Lite CLINT模块
// 作为slave设备，提供64位mtime寄存器的读取功能

module clint #(
    parameter int CLINT_ADDR = 32'ha0000048  // mtime低32位地址, 高32位在+4
) (
    input logic             clk,
    input logic             reset,
          axi_lite_if.slave s       // 使用interface替代所有独立的AXI信号
);

    // 定义读状态枚举
    typedef enum logic [1:0] {
        IDLE_RD,
        ADDR_RD,
        DATA_RD
    } rd_state_t;

    rd_state_t rd_state, next_rd_state;

    // 定义写状态枚举
    typedef enum logic [2:0] {
        IDLE_WR,
        ADDR_WR,
        DATA_WR,
        RESP_WR
    } wr_state_t;

    wr_state_t wr_state, next_wr_state;

    // 64位mtime寄存器
    logic [63:0] mtime;

    // 地址匹配信号
    logic addr_match_aw;
    logic addr_match_ar;
    logic addr_match_ar_lo;  // mtime低32位
    logic addr_match_ar_hi;  // mtime高32位

    assign addr_match_aw = (s.awaddr == CLINT_ADDR) || (s.awaddr == CLINT_ADDR + 4);
    assign addr_match_ar_lo = (s.araddr == CLINT_ADDR);
    assign addr_match_ar_hi = (s.araddr == CLINT_ADDR + 4);
    assign addr_match_ar = addr_match_ar_lo || addr_match_ar_hi;

    // mtime自增逻辑
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
            IDLE_RD: next_rd_state = (s.arvalid && s.arready) ? ADDR_RD : IDLE_RD;
            ADDR_RD: next_rd_state = DATA_RD;
            DATA_RD: next_rd_state = (s.rvalid && s.rready) ? IDLE_RD : DATA_RD;
            default: next_rd_state = IDLE_RD;
        endcase
    end

    // 写状态机next逻辑
    always_comb begin
        unique case (wr_state)
            IDLE_WR:
            if (s.awvalid && s.awready && s.wvalid && s.wready) next_wr_state = RESP_WR;
            else if (s.awvalid && s.awready) next_wr_state = DATA_WR;
            else if (s.wvalid && s.wready) next_wr_state = ADDR_WR;
            else next_wr_state = IDLE_WR;
            ADDR_WR: next_wr_state = (s.awvalid && s.awready) ? RESP_WR : ADDR_WR;
            DATA_WR: next_wr_state = (s.wvalid && s.wready) ? RESP_WR : DATA_WR;
            RESP_WR: next_wr_state = (s.bvalid && s.bready) ? IDLE_WR : RESP_WR;
            default: next_wr_state = IDLE_WR;
        endcase
    end

    // 保存读地址
    logic [31:0] ar_addr_reg;

    always_ff @(posedge clk) begin
        if (reset) begin
            ar_addr_reg <= 32'h0;
        end else if (s.arvalid && s.arready) begin
            ar_addr_reg <= s.araddr;
        end
    end

    // 读地址通道
    assign s.arready = (rd_state == IDLE_RD);

    // 读数据通道
    assign s.rvalid = (rd_state == DATA_RD);
    assign s.rdata   = (rd_state == DATA_RD) ? 
                       ((ar_addr_reg == CLINT_ADDR) ? mtime[31:0] : mtime[63:32]) : 
                       32'h0;
    assign s.rresp = (rd_state == DATA_RD) ? (!addr_match_ar) : 1'b0;

    // 写地址通道
    assign s.awready = (wr_state == IDLE_WR || wr_state == ADDR_WR);

    // 写数据通道
    assign s.wready = (wr_state == IDLE_WR || wr_state == DATA_WR);

    // 写回复通道 - CLINT的mtime是只读的，写操作返回错误
    assign s.bvalid = (wr_state == RESP_WR);
    assign s.bresp = 2'b10;  // SLVERR - mtime是只读寄存器

endmodule
