/* verilator lint_off DECLFILENAME */

// AXI4-Lite Crossbar (1 master to 2 slaves)
// 根据地址范围选择从设备
module xbar #(
    parameter int SLAVE0_BASE = 32'ha00003f8,  // 串口位置
    parameter int SLAVE0_SIZE = 32'h00000004,  // 4B
    parameter int SLAVE1_BASE = 32'h80000000,
    parameter int SLAVE1_SIZE = 32'h08000000   // 128MB
) (
    input clk,
    input reset,

    // Master接口 - 读地址通道(AR)
    input               m_arvalid,
    output logic        m_arready,
    input        [31:0] m_araddr,
    // Master接口 - 读数据通道(R)
    output logic        m_rvalid,
    input               m_rready,
    output logic [31:0] m_rdata,
    output logic        m_rresp,
    // Master接口 - 写地址通道(AW)
    input               m_awvalid,
    output logic        m_awready,
    input        [31:0] m_awaddr,
    // Master接口 - 写数据通道(W)
    input               m_wvalid,
    output logic        m_wready,
    input        [31:0] m_wdata,
    input        [ 7:0] m_wmask,
    // Master接口 - 写回复通道(B)
    output logic        m_bvalid,
    input               m_bready,
    output logic        m_bresp,

    // Slave0接口 - 读地址通道(AR)
    output logic        s0_arvalid,
    input               s0_arready,
    output logic [31:0] s0_araddr,
    // Slave0接口 - 读数据通道(R)
    input               s0_rvalid,
    output logic        s0_rready,
    input        [31:0] s0_rdata,
    input               s0_rresp,
    // Slave0接口 - 写地址通道(AW)
    output logic        s0_awvalid,
    input               s0_awready,
    output logic [31:0] s0_awaddr,
    // Slave0接口 - 写数据通道(W)
    output logic        s0_wvalid,
    input               s0_wready,
    output logic [31:0] s0_wdata,
    output logic [ 7:0] s0_wmask,
    // Slave0接口 - 写回复通道(B)
    input               s0_bvalid,
    output logic        s0_bready,
    input               s0_bresp,

    // Slave1接口 - 读地址通道(AR)
    output logic        s1_arvalid,
    input               s1_arready,
    output logic [31:0] s1_araddr,
    // Slave1接口 - 读数据通道(R)
    input               s1_rvalid,
    output logic        s1_rready,
    input        [31:0] s1_rdata,
    input               s1_rresp,
    // Slave1接口 - 写地址通道(AW)
    output logic        s1_awvalid,
    input               s1_awready,
    output logic [31:0] s1_awaddr,
    // Slave1接口 - 写数据通道(W)
    output logic        s1_wvalid,
    input               s1_wready,
    output logic [31:0] s1_wdata,
    output logic [ 7:0] s1_wmask,
    // Slave1接口 - 写回复通道(B)
    input               s1_bvalid,
    output logic        s1_bready,
    input               s1_bresp
);

    // 定义读状态枚举
    typedef enum logic [1:0] {
        IDLE_RD,
        ROUTE_RD,
        DATA_RD
    } rd_state_t;

    rd_state_t rd_state, next_rd_state;

    // 定义写状态枚举
    typedef enum logic [1:0] {
        IDLE_WR,
        ROUTE_WR,
        RESP_WR
    } wr_state_t;

    wr_state_t wr_state, next_wr_state;

    // 地址匹配逻辑
    logic sel_slave0_ar, sel_slave1_ar;
    logic sel_slave0_aw, sel_slave1_aw;
    logic addr_err_ar, addr_err_aw;

    assign sel_slave0_ar = (m_araddr >= SLAVE0_BASE) && (m_araddr < (SLAVE0_BASE + SLAVE0_SIZE));
    assign sel_slave1_ar = (m_araddr >= SLAVE1_BASE) && (m_araddr < (SLAVE1_BASE + SLAVE1_SIZE));
    assign addr_err_ar   = !(sel_slave0_ar || sel_slave1_ar);

    assign sel_slave0_aw = (m_awaddr >= SLAVE0_BASE) && (m_awaddr < (SLAVE0_BASE + SLAVE0_SIZE));
    assign sel_slave1_aw = (m_awaddr >= SLAVE1_BASE) && (m_awaddr < (SLAVE1_BASE + SLAVE1_SIZE));
    assign addr_err_aw   = !(sel_slave0_aw || sel_slave1_aw);

    // 保存路由选择
    logic rd_sel_slave0, rd_sel_slave1, rd_addr_err;
    logic wr_sel_slave0, wr_sel_slave1, wr_addr_err;

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
            IDLE_RD:  next_rd_state = (m_arvalid && m_arready) ? ROUTE_RD : IDLE_RD;
            ROUTE_RD: next_rd_state = DATA_RD;
            DATA_RD:  next_rd_state = (m_rvalid && m_rready) ? IDLE_RD : DATA_RD;
            default:  next_rd_state = IDLE_RD;
        endcase
    end

    // 写状态机next逻辑
    always_comb begin
        unique case (wr_state)
            IDLE_WR:  next_wr_state = (m_awvalid && m_awready) ? ROUTE_WR : IDLE_WR;
            ROUTE_WR: next_wr_state = (m_wvalid && m_wready) ? RESP_WR : ROUTE_WR;
            RESP_WR:  next_wr_state = (m_bvalid && m_bready) ? IDLE_WR : RESP_WR;
            default:  next_wr_state = IDLE_WR;
        endcase
    end

    // 保存读路由选择
    always_ff @(posedge clk) begin
        if (reset) begin
            rd_sel_slave0 <= 1'b0;
            rd_sel_slave1 <= 1'b0;
            rd_addr_err   <= 1'b0;
        end else if (m_arvalid && m_arready) begin
            rd_sel_slave0 <= sel_slave0_ar;
            rd_sel_slave1 <= sel_slave1_ar;
            rd_addr_err   <= addr_err_ar;
        end
    end

    // 保存写路由选择
    always_ff @(posedge clk) begin
        if (reset) begin
            wr_sel_slave0 <= 1'b0;
            wr_sel_slave1 <= 1'b0;
            wr_addr_err   <= 1'b0;
        end else if (m_awvalid && m_awready) begin
            wr_sel_slave0 <= sel_slave0_aw;
            wr_sel_slave1 <= sel_slave1_aw;
            wr_addr_err   <= addr_err_aw;
        end
    end

    // Master读地址通道
    assign m_arready = (rd_state == IDLE_RD) && 
                       ((sel_slave0_ar && s0_arready) || 
                        (sel_slave1_ar && s1_arready) || 
                        addr_err_ar);

    // Master读数据通道
    assign m_rvalid = (rd_state == DATA_RD) && 
                      ((rd_sel_slave0 && s0_rvalid) || 
                       (rd_sel_slave1 && s1_rvalid) || 
                       rd_addr_err);
    assign m_rdata = rd_sel_slave0 ? s0_rdata : rd_sel_slave1 ? s1_rdata : 32'h0;
    assign m_rresp = rd_sel_slave0 ? s0_rresp : rd_sel_slave1 ? s1_rresp : 1'b1;

    // Master写地址通道
    assign m_awready = (wr_state == IDLE_WR) && 
                       ((sel_slave0_aw && s0_awready) || 
                        (sel_slave1_aw && s1_awready) || 
                        addr_err_aw);

    // Master写数据通道
    assign m_wready = (wr_state == ROUTE_WR) && 
                      ((wr_sel_slave0 && s0_wready) || 
                       (wr_sel_slave1 && s1_wready) || 
                       wr_addr_err);

    // Master写回复通道
    assign m_bvalid = (wr_state == RESP_WR) && 
                      ((wr_sel_slave0 && s0_bvalid) || 
                       (wr_sel_slave1 && s1_bvalid) || 
                       wr_addr_err);
    assign m_bresp = wr_sel_slave0 ? s0_bresp : wr_sel_slave1 ? s1_bresp : 1'b1;

    // Slave0读地址通道
    assign s0_arvalid = (rd_state == IDLE_RD) && m_arvalid && sel_slave0_ar;
    assign s0_araddr = m_araddr;

    // Slave0读数据通道
    assign s0_rready = (rd_state == DATA_RD) && m_rready && rd_sel_slave0;

    // Slave0写地址通道
    assign s0_awvalid = (wr_state == IDLE_WR) && m_awvalid && sel_slave0_aw;
    assign s0_awaddr = m_awaddr;

    // Slave0写数据通道
    assign s0_wvalid = (wr_state == ROUTE_WR) && m_wvalid && wr_sel_slave0;
    assign s0_wdata = m_wdata;
    assign s0_wmask = m_wmask;

    // Slave0写回复通道
    assign s0_bready = (wr_state == RESP_WR) && m_bready && wr_sel_slave0;

    // Slave1读地址通道
    assign s1_arvalid = (rd_state == IDLE_RD) && m_arvalid && sel_slave1_ar;
    assign s1_araddr = m_araddr;

    // Slave1读数据通道
    assign s1_rready = (rd_state == DATA_RD) && m_rready && rd_sel_slave1;

    // Slave1写地址通道
    assign s1_awvalid = (wr_state == IDLE_WR) && m_awvalid && sel_slave1_aw;
    assign s1_awaddr = m_awaddr;

    // Slave1写数据通道
    assign s1_wvalid = (wr_state == ROUTE_WR) && m_wvalid && wr_sel_slave1;
    assign s1_wdata = m_wdata;
    assign s1_wmask = m_wmask;

    // Slave1写回复通道
    assign s1_bready = (wr_state == RESP_WR) && m_bready && wr_sel_slave1;

endmodule
