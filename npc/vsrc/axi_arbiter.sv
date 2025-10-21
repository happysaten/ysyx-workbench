/* verilator lint_off DECLFILENAME */

// AXI4-Lite仲裁器模块
// 支持2个master和1个slave。
module axi_arbiter (
    input clk,
    input reset,

    // Master 0 接口
    // 读地址通道(AR)
    input               m0_arvalid,
    output logic        m0_arready,
    input        [31:0] m0_araddr,
    // 读数据通道(R)
    output logic        m0_rvalid,
    input               m0_rready,
    output logic [31:0] m0_rdata,
    output logic        m0_rresp,
    // 写地址通道(AW)
    input               m0_awvalid,
    output logic        m0_awready,
    input        [31:0] m0_awaddr,
    // 写数据通道(W)
    input               m0_wvalid,
    output logic        m0_wready,
    input        [31:0] m0_wdata,
    input        [ 7:0] m0_wmask,
    // 写回复通道(B)
    output logic        m0_bvalid,
    input               m0_bready,
    output logic        m0_bresp,

    // Master 1 接口
    // 读地址通道(AR)
    input               m1_arvalid,
    output logic        m1_arready,
    input        [31:0] m1_araddr,
    // 读数据通道(R)
    output logic        m1_rvalid,
    input               m1_rready,
    output logic [31:0] m1_rdata,
    output logic        m1_rresp,
    // 写地址通道(AW)
    input               m1_awvalid,
    output logic        m1_awready,
    input        [31:0] m1_awaddr,
    // 写数据通道(W)
    input               m1_wvalid,
    output logic        m1_wready,
    input        [31:0] m1_wdata,
    input        [ 7:0] m1_wmask,
    // 写回复通道(B)
    output logic        m1_bvalid,
    input               m1_bready,
    output logic        m1_bresp,

    // Slave 接口
    // 读地址通道(AR)
    output logic        s_arvalid,
    input               s_arready,
    output logic [31:0] s_araddr,
    // 读数据通道(R)
    input               s_rvalid,
    output logic        s_rready,
    input        [31:0] s_rdata,
    input               s_rresp,
    // 写地址通道(AW)
    output logic        s_awvalid,
    input               s_awready,
    output logic [31:0] s_awaddr,
    // 写数据通道(W)
    output logic        s_wvalid,
    input               s_wready,
    output logic [31:0] s_wdata,
    output logic [ 7:0] s_wmask,
    // 写回复通道(B)
    input               s_bvalid,
    output logic        s_bready,
    input               s_bresp
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
            next_rd_state = (m0_arvalid&&m0_arready) ? M0_RD :
                                         (m1_arvalid&&m1_arready) ? M1_RD : IDLE_RD;
            M0_RD: next_rd_state = (s_rvalid && m0_rready) ? IDLE_RD : M0_RD;
            M1_RD: next_rd_state = (s_rvalid && m1_rready) ? IDLE_RD : M1_RD;
        endcase
    end

    // 写状态机next逻辑
    always_comb begin
        unique case (wr_state)
            IDLE_WR:
            next_wr_state = (m0_awvalid && m0_awready) ? (m0_wvalid && m0_wready ? M0_WR : M0_AWR) :
                                         (m1_awvalid && m1_awready) ? (m1_wvalid && m1_wready ? M1_WR : M1_AWR) : IDLE_WR;
            M0_AWR: next_wr_state = (m0_wvalid && m0_wready) ? M0_WR : M0_AWR;
            M0_WR: next_wr_state = (s_bvalid && m0_bready) ? IDLE_WR : M0_WR;
            M1_AWR: next_wr_state = (m1_wvalid && m1_wready) ? M1_WR : M1_AWR;
            M1_WR: next_wr_state = (s_bvalid && m1_bready) ? IDLE_WR : M1_WR;
            default: next_wr_state = IDLE_WR;
        endcase
    end


    // 读地址通道
    assign s_arvalid = (rd_state == IDLE_RD) && (m0_arvalid || m1_arvalid);
    // assign s_araddr = m0_arvalid ? m0_araddr : m1_araddr;
    assign s_araddr = m1_arvalid ? m1_araddr : m0_araddr;
    assign m0_arready = (rd_state == IDLE_RD) ? s_arready : 1'b0;
    assign m1_arready = (rd_state == IDLE_RD) ? (s_arready && !m0_arvalid) : 1'b0;

    // 读数据通道
    assign m0_rvalid = (rd_state == M0_RD) ? s_rvalid : 1'b0;
    assign m1_rvalid = (rd_state == M1_RD) ? s_rvalid : 1'b0;
    assign s_rready = (rd_state == M0_RD) ? m0_rready : (rd_state == M1_RD) ? m1_rready : 1'b0;
    assign m0_rdata = s_rdata;
    assign m1_rdata = s_rdata;
    assign m0_rresp = (rd_state == M0_RD) ? s_rresp : 1'b0;
    assign m1_rresp = (rd_state == M1_RD) ? s_rresp : 1'b0;

    // 写地址通道
    assign s_awvalid = (wr_state == IDLE_WR) && (m0_awvalid || m1_awvalid);
    // assign s_awaddr = m0_awvalid ? m0_awaddr : m1_awaddr;
    assign s_awaddr = m1_awvalid ? m1_awaddr : m0_awaddr;
    assign m0_awready = (wr_state == IDLE_WR) ? s_awready : 1'b0;
    assign m1_awready = (wr_state == IDLE_WR) ? (s_awready && !m0_awvalid) : 1'b0;

    // 写数据通道
    assign s_wvalid = (wr_state == IDLE_WR || wr_state == M0_AWR || wr_state == M1_AWR) && (m0_wvalid || m1_wvalid);
    assign s_wdata = m0_wvalid ? m0_wdata : m1_wdata;
    assign s_wmask = m0_wvalid ? m0_wmask : m1_wmask;
    assign m0_wready = (wr_state == IDLE_WR || wr_state == M0_AWR || wr_state == M1_AWR) ? s_wready : 1'b0;
    assign m1_wready = (wr_state == IDLE_WR || wr_state == M0_AWR || wr_state == M1_AWR) ? (s_wready && !m0_wvalid) : 1'b0;

    // 写回复通道
    assign m0_bvalid = (wr_state == M0_WR) ? s_bvalid : 1'b0;
    assign m1_bvalid = (wr_state == M1_WR) ? s_bvalid : 1'b0;
    assign s_bready = (wr_state == M0_WR) ? m0_bready : (wr_state == M1_WR) ? m1_bready : 1'b0;
    assign m0_bresp = (wr_state == M0_WR) ? s_bresp : 1'b0;
    assign m1_bresp = (wr_state == M1_WR) ? s_bresp : 1'b0;

endmodule

