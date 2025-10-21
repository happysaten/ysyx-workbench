/* verilator lint_off DECLFILENAME */

// AXI4-Lite Crossbar (1 master to n slaves)
// 根据地址范围选择从设备

module xbar #(
    parameter int SLAVE_NUM = 2,
    // 地址范围数组
    parameter logic [31:0] SLAVE_BASE[SLAVE_NUM] = '{32'ha00003f8, 32'h80000000},
    parameter logic [31:0] SLAVE_SIZE[SLAVE_NUM] = '{32'h00000004, 32'h08000000}
) (
    input logic              clk,
    input logic              reset,
          axi_lite_if.slave  m,                // 1 个 Master (从crossbar角度看是slave接口)
          axi_lite_if.master s    [SLAVE_NUM]  // n 个 Slave (从crossbar角度看是master接口)
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

    // 地址匹配逻辑 - 参数化
    logic [SLAVE_NUM-1:0] sel_slave_ar;
    logic [SLAVE_NUM-1:0] sel_slave_aw;
    logic addr_err_ar, addr_err_aw;

    // 地址匹配 - 使用 generate
    genvar j;
    generate
        for (j = 0; j < SLAVE_NUM; j++) begin : addr_match_gen
            assign sel_slave_ar[j] = (m.araddr >= SLAVE_BASE[j]) &&
                                    (m.araddr < (SLAVE_BASE[j] + SLAVE_SIZE[j]));
            assign sel_slave_aw[j] = (m.awaddr >= SLAVE_BASE[j]) &&
                                    (m.awaddr < (SLAVE_BASE[j] + SLAVE_SIZE[j]));
        end
    endgenerate

    assign addr_err_ar = !(|sel_slave_ar);
    assign addr_err_aw = !(|sel_slave_aw);

    // 保存路由选择 - 参数化
    logic [SLAVE_NUM-1:0] rd_sel_slave;
    logic [SLAVE_NUM-1:0] wr_sel_slave;
    logic rd_addr_err;
    logic wr_addr_err;

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
            IDLE_RD:  next_rd_state = (m.arvalid && m.arready) ? ROUTE_RD : IDLE_RD;
            ROUTE_RD: next_rd_state = DATA_RD;
            DATA_RD:  next_rd_state = (m.rvalid && m.rready) ? IDLE_RD : DATA_RD;
            default:  next_rd_state = IDLE_RD;
        endcase
    end

    // 写状态机next逻辑
    always_comb begin
        unique case (wr_state)
            IDLE_WR:  next_wr_state = (m.awvalid && m.awready) ? ROUTE_WR : IDLE_WR;
            ROUTE_WR: next_wr_state = (m.wvalid && m.wready) ? RESP_WR : ROUTE_WR;
            RESP_WR:  next_wr_state = (m.bvalid && m.bready) ? IDLE_WR : RESP_WR;
            default:  next_wr_state = IDLE_WR;
        endcase
    end

    // 保存读路由选择
    always_ff @(posedge clk) begin
        if (reset) begin
            rd_sel_slave <= '0;
            rd_addr_err  <= 1'b0;
        end else if (m.arvalid && m.arready) begin
            rd_sel_slave <= sel_slave_ar;
            rd_addr_err  <= addr_err_ar;
        end
    end

    // 保存写路由选择
    always_ff @(posedge clk) begin
        if (reset) begin
            wr_sel_slave <= '0;
            wr_addr_err  <= 1'b0;
        end else if (m.awvalid && m.awready) begin
            wr_sel_slave <= sel_slave_aw;
            wr_addr_err  <= addr_err_aw;
        end
    end

    // Master读地址通道 - 参数化
    logic [SLAVE_NUM-1:0] slave_ar_ready_vec;
    generate
        for (j = 0; j < SLAVE_NUM; j++) begin : ar_ready_gen
            assign slave_ar_ready_vec[j] = sel_slave_ar[j] && s[j].arready;
        end
    endgenerate
    assign m.arready = (rd_state == IDLE_RD) && ((|slave_ar_ready_vec) || addr_err_ar);

    // Master读数据通道 - 参数化
    logic [SLAVE_NUM-1:0] slave_r_valid_vec;
    logic [31:0] slave_rdata_vec[SLAVE_NUM];
    logic slave_rresp_vec[SLAVE_NUM];

    generate
        for (j = 0; j < SLAVE_NUM; j++) begin : r_data_gen
            assign slave_r_valid_vec[j] = rd_sel_slave[j] && s[j].rvalid;
            assign slave_rdata_vec[j]   = rd_sel_slave[j] ? s[j].rdata : 32'h0;
            assign slave_rresp_vec[j]   = rd_sel_slave[j] ? s[j].rresp : 1'b0;
        end
    endgenerate

    always_comb begin
        m.rdata = 32'h0;
        m.rresp = 1'b1;
        for (int i = 0; i < SLAVE_NUM; i++) begin
            m.rdata |= slave_rdata_vec[i];
            if (rd_sel_slave[i]) begin
                m.rresp = slave_rresp_vec[i];
            end
        end
    end
    assign m.rvalid = (rd_state == DATA_RD) && ((|slave_r_valid_vec) || rd_addr_err);

    // Master写地址通道 - 参数化
    logic [SLAVE_NUM-1:0] slave_aw_ready_vec;
    generate
        for (j = 0; j < SLAVE_NUM; j++) begin : aw_ready_gen
            assign slave_aw_ready_vec[j] = sel_slave_aw[j] && s[j].awready;
        end
    endgenerate
    assign m.awready = (wr_state == IDLE_WR) && ((|slave_aw_ready_vec) || addr_err_aw);

    // Master写数据通道 - 参数化
    logic [SLAVE_NUM-1:0] slave_w_ready_vec;
    generate
        for (j = 0; j < SLAVE_NUM; j++) begin : w_ready_gen
            assign slave_w_ready_vec[j] = wr_sel_slave[j] && s[j].wready;
        end
    endgenerate
    assign m.wready = (wr_state == ROUTE_WR) && ((|slave_w_ready_vec) || wr_addr_err);

    // Master写回复通道 - 参数化
    logic [SLAVE_NUM-1:0] slave_b_valid_vec;
    logic slave_bresp_vec[SLAVE_NUM];

    generate
        for (j = 0; j < SLAVE_NUM; j++) begin : b_resp_gen
            assign slave_b_valid_vec[j] = wr_sel_slave[j] && s[j].bvalid;
            assign slave_bresp_vec[j]   = wr_sel_slave[j] ? s[j].bresp : 1'b0;
        end
    endgenerate

    always_comb begin
        m.bresp = 1'b1;
        for (int i = 0; i < SLAVE_NUM; i++) begin
            if (wr_sel_slave[i]) begin
                m.bresp = slave_bresp_vec[i];
            end
        end
    end
    assign m.bvalid = (wr_state == RESP_WR) && ((|slave_b_valid_vec) || wr_addr_err);

    // Slave接口 - 参数化
    genvar i;
    generate
        for (i = 0; i < SLAVE_NUM; i++) begin : slave_gen
            // 读地址通道
            assign s[i].arvalid = (rd_state == IDLE_RD) && m.arvalid && sel_slave_ar[i];
            assign s[i].araddr  = m.araddr;

            // 读数据通道
            assign s[i].rready  = (rd_state == DATA_RD) && m.rready && rd_sel_slave[i];

            // 写地址通道
            assign s[i].awvalid = (wr_state == IDLE_WR) && m.awvalid && sel_slave_aw[i];
            assign s[i].awaddr  = m.awaddr;

            // 写数据通道
            assign s[i].wvalid  = (wr_state == ROUTE_WR) && m.wvalid && wr_sel_slave[i];
            assign s[i].wdata   = m.wdata;
            assign s[i].wmask   = m.wmask;

            // 写回复通道
            assign s[i].bready  = (wr_state == RESP_WR) && m.bready && wr_sel_slave[i];
        end
    endgenerate

endmodule
