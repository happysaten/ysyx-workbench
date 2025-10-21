/* verilator lint_off DECLFILENAME */

// AXI4-Lite仲裁器模块
// 支持2个master和1个slave。
import axi_lite_if::*;
module axi_arbiter (
    input logic       clk,
    input logic       reset,
    axi_if.slave      m0,  // Master 0 接口
    axi_if.slave      m1,  // Master 1 接口
    axi_if.master     s    // Slave 接口
);

    // 定义读状态枚举
    typedef enum logic [1:0] {
        IDLE_RD,
        M0_RD,
        M1_RD
    } rd_state_t;

    rd_state_t rd_state, next_rd_state;

    // 定义写状态枚举
    typedef enum logic [2:0] {
        IDLE_WR,
        M0_WR,
        M0_AWR,
        M1_AWR,
        M1_WR
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
            next_rd_state = (m0.arvalid && m0.arready) ? M0_RD :
                           (m1.arvalid && m1.arready) ? M1_RD : IDLE_RD;
            M0_RD: next_rd_state = (s.rvalid && m0.rready) ? IDLE_RD : M0_RD;
            M1_RD: next_rd_state = (s.rvalid && m1.rready) ? IDLE_RD : M1_RD;
        endcase
    end

    // 写状态机next逻辑
    always_comb begin
        unique case (wr_state)
            IDLE_WR:
            next_wr_state = (m0.awvalid && m0.awready) ? (m0.wvalid && m0.wready ? M0_WR : M0_AWR) :
                           (m1.awvalid && m1.awready) ? (m1.wvalid && m1.wready ? M1_WR : M1_AWR) : IDLE_WR;
            M0_AWR: next_wr_state = (m0.wvalid && m0.wready) ? M0_WR : M0_AWR;
            M0_WR: next_wr_state = (s.bvalid && m0.bready) ? IDLE_WR : M0_WR;
            M1_AWR: next_wr_state = (m1.wvalid && m1.wready) ? M1_WR : M1_AWR;
            M1_WR: next_wr_state = (s.bvalid && m1.bready) ? IDLE_WR : M1_WR;
            default: next_wr_state = IDLE_WR;
        endcase
    end


    // 读地址通道
    assign s.arvalid = (rd_state == IDLE_RD) && (m0.arvalid || m1.arvalid);
    assign s.araddr = m1.arvalid ? m1.araddr : m0.araddr;
    assign m0.arready = (rd_state == IDLE_RD) ? s.arready : 1'b0;
    assign m1.arready = (rd_state == IDLE_RD) ? (s.arready && !m0.arvalid) : 1'b0;

    // 读数据通道
    assign m0.rvalid = (rd_state == M0_RD) ? s.rvalid : 1'b0;
    assign m1.rvalid = (rd_state == M1_RD) ? s.rvalid : 1'b0;
    assign s.rready = (rd_state == M0_RD) ? m0.rready : (rd_state == M1_RD) ? m1.rready : 1'b0;
    assign m0.rdata = s.rdata;
    assign m1.rdata = s.rdata;
    assign m0.rresp = (rd_state == M0_RD) ? s.rresp : 1'b0;
    assign m1.rresp = (rd_state == M1_RD) ? s.rresp : 1'b0;

    // 写地址通道
    assign s.awvalid = (wr_state == IDLE_WR) && (m0.awvalid || m1.awvalid);
    assign s.awaddr = m1.awvalid ? m1.awaddr : m0.awaddr;
    assign m0.awready = (wr_state == IDLE_WR) ? s.awready : 1'b0;
    assign m1.awready = (wr_state == IDLE_WR) ? (s.awready && !m0.awvalid) : 1'b0;

    // 写数据通道
    assign s.wvalid = (wr_state == IDLE_WR || wr_state == M0_AWR || wr_state == M1_AWR) && (m0.wvalid || m1.wvalid);
    assign s.wdata = m0.wvalid ? m0.wdata : m1.wdata;
    assign s.wmask = m0.wvalid ? m0.wmask : m1.wmask;
    assign m0.wready = (wr_state == IDLE_WR || wr_state == M0_AWR || wr_state == M1_AWR) ? s.wready : 1'b0;
    assign m1.wready = (wr_state == IDLE_WR || wr_state == M0_AWR || wr_state == M1_AWR) ? (s.wready && !m0.wvalid) : 1'b0;

    // 写回复通道
    assign m0.bvalid = (wr_state == M0_WR) ? s.bvalid : 1'b0;
    assign m1.bvalid = (wr_state == M1_WR) ? s.bvalid : 1'b0;
    assign s.bready = (wr_state == M0_WR) ? m0.bready : (wr_state == M1_WR) ? m1.bready : 1'b0;
    assign m0.bresp = (wr_state == M0_WR) ? s.bresp : 1'b0;
    assign m1.bresp = (wr_state == M1_WR) ? s.bresp : 1'b0;

endmodule

