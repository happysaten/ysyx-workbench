/* verilator lint_off DECLFILENAME */

// AXI4-Lite仲裁器模块
// 支持2个master和1个slave。
module axi_arbiter (
    input logic              clk,
    input logic              reset,
          axi_lite_if.slave  m0,     // Master 0 接口
          axi_lite_if.slave  m1,     // Master 1 接口
          axi_lite_if.master s       // Slave 接口
);

    // 定义读状态枚举
    typedef enum logic [1:0] {
        IDLE_RD,
        M0_WAIT_RRESP,
        M1_WAIT_RRESP
    } rd_state_t;

    rd_state_t rd_state, next_rd_state;

    // 定义写状态枚举
    typedef enum logic [1:0] {
        IDLE_WR,
        M0_WAIT_WRESP,
        M1_WAIT_WRESP
    } wr_state_t;

    wr_state_t wr_state, next_wr_state;

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
            IDLE_RD:
            next_rd_state = (m0.arvalid && m0.arready) ? M0_WAIT_RRESP :
                           (m1.arvalid && m1.arready) ? M1_WAIT_RRESP : IDLE_RD;
            M0_WAIT_RRESP: next_rd_state = (s.rvalid && m0.rready) ? IDLE_RD : M0_WAIT_RRESP;
            M1_WAIT_RRESP: next_rd_state = (s.rvalid && m1.rready) ? IDLE_RD : M1_WAIT_RRESP;
        endcase
    end

    // 写状态机next逻辑
    always_comb begin
        unique case (wr_state)
            IDLE_WR:
            next_wr_state = (m0.awvalid && m0.awready) ? M0_WAIT_WRESP :
                           (m1.awvalid && m1.awready) ? M1_WAIT_WRESP : IDLE_WR;
            M0_WAIT_WRESP: next_wr_state = (s.bvalid && m0.bready) ? IDLE_WR : M0_WAIT_WRESP;
            M1_WAIT_WRESP: next_wr_state = (s.bvalid && m1.bready) ? IDLE_WR : M1_WAIT_WRESP;
            default: next_wr_state = IDLE_WR;
        endcase
    end


    // 读地址通道
    assign s.arvalid = (rd_state == IDLE_RD) && (m0.arvalid || m1.arvalid);
    assign s.araddr = m1.arvalid ? m1.araddr : m0.araddr;
    assign m0.arready = (rd_state == IDLE_RD) ? s.arready : 1'b0;
    assign m1.arready = (rd_state == IDLE_RD) ? (s.arready && !m0.arvalid) : 1'b0;

    // 读数据通道
    assign m0.rvalid = (rd_state == M0_WAIT_RRESP) ? s.rvalid : 1'b0;
    assign m1.rvalid = (rd_state == M1_WAIT_RRESP) ? s.rvalid : 1'b0;
    assign s.rready = (rd_state == M0_WAIT_RRESP) ? m0.rready : (rd_state == M1_WAIT_RRESP) ? m1.rready : 1'b0;
    assign m0.rdata = s.rdata;
    assign m1.rdata = s.rdata;
    assign m0.rresp = (rd_state == M0_WAIT_RRESP) ? s.rresp : 2'b00;
    assign m1.rresp = (rd_state == M1_WAIT_RRESP) ? s.rresp : 2'b00;

    // 写地址通道
    assign s.awvalid = (wr_state == IDLE_WR) && (m0.awvalid || m1.awvalid);
    assign s.awaddr = m1.awvalid ? m1.awaddr : m0.awaddr;
    assign m0.awready = (wr_state == IDLE_WR) ? s.awready : 1'b0;
    assign m1.awready = (wr_state == IDLE_WR) ? (s.awready && !m0.awvalid) : 1'b0;

    // 写数据通道
    assign s.wvalid = (wr_state == IDLE_WR || wr_state == M0_WAIT_WRESP || wr_state == M1_WAIT_WRESP) && (m0.wvalid || m1.wvalid);
    assign s.wdata = (m0.wvalid && m0.wready) ? m0.wdata : m1.wdata;
    assign s.wmask = (m0.wvalid && m0.wready) ? m0.wmask : m1.wmask;
    assign m0.wready = (wr_state == IDLE_WR || wr_state == M0_WAIT_WRESP) ? s.wready : 1'b0;
    assign m1.wready = (wr_state == IDLE_WR || wr_state == M1_WAIT_WRESP) ? (s.wready && !m0.wvalid) : 1'b0;

    // 写回复通道
    assign m0.bvalid = (wr_state == M0_WAIT_WRESP) ? s.bvalid : 1'b0;
    assign m1.bvalid = (wr_state == M1_WAIT_WRESP) ? s.bvalid : 1'b0;
    assign s.bready = (wr_state == M0_WAIT_WRESP) ? m0.bready : (wr_state == M1_WAIT_WRESP) ? m1.bready : 1'b0;
    assign m0.bresp = (wr_state == M0_WAIT_WRESP) ? s.bresp : 2'b00;
    assign m1.bresp = (wr_state == M1_WAIT_WRESP) ? s.bresp : 2'b00;

endmodule

