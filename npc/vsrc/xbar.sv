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
    generate
        for (genvar j = 0; j < SLAVE_NUM; j++) begin : gen_addr_match
            assign sel_slave_ar[j] = (m.araddr >= SLAVE_BASE[j]) &&
                                    (m.araddr < (SLAVE_BASE[j] + SLAVE_SIZE[j]));
            assign sel_slave_aw[j] = (m.awaddr >= SLAVE_BASE[j]) &&
                                    (m.awaddr < (SLAVE_BASE[j] + SLAVE_SIZE[j]));
        end
    endgenerate

    assign addr_err_ar = !(|sel_slave_ar);
    assign addr_err_aw = !(|sel_slave_aw);

    // ============ 读通道分发 ============
    logic [SLAVE_NUM-1:0] slaves_arready, slaves_rvalid, slaves_rresp;
    logic [SLAVE_NUM-1:0][31:0] slaves_rdata;
    generate
        for (genvar i = 0; i < SLAVE_NUM; i++) begin : gen_rd_xbar
            assign s[i].arvalid = m.arvalid && sel_slave_ar[i];
            assign slaves_arready[i] = s[i].arready;
            assign s[i].araddr = m.araddr;
            assign slaves_rvalid[i] = s[i].rvalid;
            assign s[i].rready = m.rready && sel_slave_ar[i];
            assign slaves_rdata[i] = s[i].rdata;
            assign slaves_rresp[i] = s[i].rresp;
        end
    endgenerate


    assign m.arready = |(sel_slave_ar & (slaves_arready));
    assign m.rvalid  = |(sel_slave_ar & (slaves_rvalid));
    assign m.rdata   = |(sel_slave_ar & (slaves_rdata));
    assign m.rresp   = |(sel_slave_ar & (slaves_rresp));


    // ============ 写通道分发 ============
    logic [SLAVE_NUM-1:0] slaves_awready, slaves_wready, slaves_bvalid, slaves_bresp;
    generate
        for (genvar i = 0; i < SLAVE_NUM; i++) begin : gen_wr_xbar
            assign s[i].awvalid = m.awvalid && sel_slave_aw[i];
            assign slaves_awready[i] = s[i].awready;
            assign s[i].awaddr = m.awaddr;
            assign s[i].wvalid = m.wvalid && sel_slave_aw[i];
            assign slaves_wready[i] = s[i].wready;
            assign s[i].wdata = m.wdata;
            assign slaves_bvalid[i] = s[i].bvalid;
            assign s[i].bready = m.bready && sel_slave_aw[i];
            assign slaves_bresp[i] = s[i].bresp;
        end
    endgenerate

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
            RESP_RD: next_rd_state = (m.rvalid && m.rready) ? IDLE_RD : RESP_RD;
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
            next_wr_state = ((m.awvalid && m.awready) && (m.wvalid && m.wready)) ? RESP_WR :
                           (m.awvalid || m.wvalid) ? WAIT_WR : IDLE_WR;
            WAIT_WR: next_wr_state = (m.wvalid && m.wready) ? RESP_WR : WAIT_WR;
            RESP_WR: next_wr_state = (m.bvalid && m.bready) ? IDLE_WR : RESP_WR;
        endcase
    end

endmodule
