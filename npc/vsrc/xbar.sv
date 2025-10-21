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
    typedef enum logic {
        IDLE_RD,
        RESP_RD
    } rd_state_t;

    rd_state_t rd_state, next_rd_state;

    // 定义写状态枚举
    typedef enum logic [1:0] {
        IDLE_WR,
        WAIT_WR,
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

    // ============ 读通道状态机 ============
    always_ff @(posedge clk) begin
        if (reset) begin
            rd_state <= IDLE_RD;
        end else begin
            rd_state <= next_rd_state;
        end
    end

    always_comb begin
        unique case (rd_state)
            IDLE_RD: next_rd_state = (m.arvalid && m.arready) ? RESP_RD : IDLE_RD;
            RESP_RD: next_rd_state = (m.rready && m.rvalid) ? IDLE_RD : RESP_RD;
        endcase
    end

    // ============ 写通道状态机 ============
    always_ff @(posedge clk) begin
        if (reset) begin
            wr_state <= IDLE_WR;
        end else begin
            wr_state <= next_wr_state;
        end
    end

    always_comb begin
        unique case (wr_state)
            IDLE_WR:
            next_wr_state = (m.awvalid && m.wvalid && m.awready && m.wready) ? RESP_WR :
                           (m.awvalid || m.wvalid) ? WAIT_WR : IDLE_WR;
            WAIT_WR:
            next_wr_state = (m.awvalid && m.wvalid && m.awready && m.wready) ? RESP_WR : WAIT_WR;
            RESP_WR: next_wr_state = (m.bready && m.bvalid) ? IDLE_WR : RESP_WR;
        endcase
    end

    // ============ 读地址通道路由 ============
    always_comb begin
        m.arready = (rd_state == IDLE_RD && m.arvalid) ? 
                    (addr_err_ar ? 1'b1 : |(sel_slave_ar & {SLAVE_NUM{1'b1}})) : 1'b0;
    end

    generate
        for (j = 0; j < SLAVE_NUM; j++) begin : ar_route_gen
            always_comb begin
                s[j].arvalid = (rd_state == IDLE_RD && m.arvalid && !addr_err_ar && sel_slave_ar[j]) ? 1'b1 : 1'b0;
                s[j].araddr = m.araddr;
                if (rd_state == IDLE_RD && m.arvalid && !addr_err_ar && sel_slave_ar[j]) begin
                    m.arready = s[j].arready;
                end
            end
        end
    endgenerate

    // ============ 读数据通道路由 ============
    logic [SLAVE_NUM-1:0] rd_slave_sel_reg;
    logic rd_addr_err_reg;

    always_ff @(posedge clk) begin
        if (reset) begin
            rd_slave_sel_reg <= '0;
            rd_addr_err_reg  <= 1'b0;
        end else if (rd_state == IDLE_RD && m.arvalid && m.arready) begin
            rd_slave_sel_reg <= sel_slave_ar;
            rd_addr_err_reg  <= addr_err_ar;
        end
    end

    always_comb begin
        m.rvalid = (rd_state == RESP_RD && rd_addr_err_reg) ? 1'b1 : 1'b0;
        m.rdata  = 32'h0;
        m.rresp  = (rd_state == RESP_RD && rd_addr_err_reg) ? 2'b11 : 2'b00;
    end

    generate
        for (j = 0; j < SLAVE_NUM; j++) begin : r_route_gen
            always_comb begin
                s[j].rready = (rd_state == RESP_RD && !rd_addr_err_reg && rd_slave_sel_reg[j]) ? m.rready : 1'b0;
                if (rd_state == RESP_RD && !rd_addr_err_reg && rd_slave_sel_reg[j]) begin
                    m.rvalid = s[j].rvalid;
                    m.rdata  = s[j].rdata;
                    m.rresp  = s[j].rresp;
                end
            end
        end
    endgenerate

    // ============ 写地址通道路由 ============
    always_comb begin
        m.awready = ((wr_state == IDLE_WR || wr_state == WAIT_WR) && m.awvalid && m.wvalid) ? 
                    (addr_err_aw ? 1'b1 : |(sel_slave_aw & {SLAVE_NUM{1'b1}})) : 1'b0;
    end

    generate
        for (j = 0; j < SLAVE_NUM; j++) begin : aw_route_gen
            always_comb begin
                s[j].awvalid = ((wr_state == IDLE_WR || wr_state == WAIT_WR) && m.awvalid && m.wvalid && !addr_err_aw && sel_slave_aw[j]) ? 1'b1 : 1'b0;
                s[j].awaddr = m.awaddr;
                if ((wr_state == IDLE_WR || wr_state == WAIT_WR) && m.awvalid && m.wvalid && !addr_err_aw && sel_slave_aw[j]) begin
                    m.awready = s[j].awready;
                end
            end
        end
    endgenerate

    // ============ 写数据通道路由 ============
    always_comb begin
        m.wready = ((wr_state == IDLE_WR || wr_state == WAIT_WR) && m.awvalid && m.wvalid) ? 
                   (addr_err_aw ? 1'b1 : |(sel_slave_aw & {SLAVE_NUM{1'b1}})) : 1'b0;
    end

    generate
        for (j = 0; j < SLAVE_NUM; j++) begin : w_route_gen
            always_comb begin
                s[j].wvalid = ((wr_state == IDLE_WR || wr_state == WAIT_WR) && m.awvalid && m.wvalid && !addr_err_aw && sel_slave_aw[j]) ? 1'b1 : 1'b0;
                s[j].wdata = m.wdata;
                s[j].wstrb = m.wstrb;
                if ((wr_state == IDLE_WR || wr_state == WAIT_WR) && m.awvalid && m.wvalid && !addr_err_aw && sel_slave_aw[j]) begin
                    m.wready = s[j].wready;
                end
            end
        end
    endgenerate

    // ============ 写响应通道路由 ============
    logic [SLAVE_NUM-1:0] wr_slave_sel_reg;
    logic wr_addr_err_reg;

    always_ff @(posedge clk) begin
        if (reset) begin
            wr_slave_sel_reg <= '0;
            wr_addr_err_reg  <= 1'b0;
        end else if ((wr_state == IDLE_WR || wr_state == WAIT_WR) && m.awvalid && m.wvalid && m.awready && m.wready) begin
            wr_slave_sel_reg <= sel_slave_aw;
            wr_addr_err_reg  <= addr_err_aw;
        end
    end

    always_comb begin
        m.bvalid = (wr_state == RESP_WR && wr_addr_err_reg) ? 1'b1 : 1'b0;
        m.bresp  = (wr_state == RESP_WR && wr_addr_err_reg) ? 2'b11 : 2'b00;
    end

    generate
        for (j = 0; j < SLAVE_NUM; j++) begin : b_route_gen
            always_comb begin
                s[j].bready = (wr_state == RESP_WR && !wr_addr_err_reg && wr_slave_sel_reg[j]) ? m.bready : 1'b0;
                if (wr_state == RESP_WR && !wr_addr_err_reg && wr_slave_sel_reg[j]) begin
                    m.bvalid = s[j].bvalid;
                    m.bresp  = s[j].bresp;
                end
            end
        end
    endgenerate

endmodule
