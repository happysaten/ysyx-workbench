/* verilator lint_off DECLFILENAME */

// AXI4-Lite Crossbar (1 master to 2 slaves)
// 根据地址范围选择从设备
// 假定写地址和写数据同时到达

module xbar #(
    parameter int SLAVE0_BASE = 32'ha00003f8,  // 串口位置
    parameter int SLAVE0_SIZE = 32'h00000004,  // 4B
    parameter int SLAVE1_BASE = 32'h80000000,
    parameter int SLAVE1_SIZE = 32'h08000000   // 128MB
) (
    input logic              clk,
    input logic              reset,
          axi_lite_if.slave  m,      // 1 个 Master (从crossbar角度看是slave接口)
          axi_lite_if.master s0,     // Slave 0 接口
          axi_lite_if.master s1      // Slave 1 接口
);

    // 定义读状态枚举
    typedef enum logic [1:0] {
        IDLE_RD,
        S0_RD,
        S1_RD
    } rd_state_t;

    rd_state_t rd_state, next_rd_state;

    // 定义写状态枚举
    typedef enum logic [2:0] {
        IDLE_WR,
        S0_WAIT_WDATA,
        S0_WAIT_WRESP,
        S1_WAIT_WDATA,
        S1_WAIT_WRESP
    } wr_state_t;

    wr_state_t wr_state, next_wr_state;

    // ============ 读通道状态机 ============
    always_ff @(posedge clk) begin
        if (reset) rd_state <= IDLE_RD;
        else rd_state <= next_rd_state;

    end

    // ============ 写通道状态机 ============
    always_ff @(posedge clk) begin
        if (reset) wr_state <= IDLE_WR;
        else wr_state <= next_wr_state;
    end

    logic [1:0] addr_match_rd, addr_match_wr;
    assign addr_match_rd[0] = (m.araddr >= SLAVE0_BASE) && (m.araddr < (SLAVE0_BASE + SLAVE0_SIZE));
    assign addr_match_rd[1] = (m.araddr >= SLAVE1_BASE) && (m.araddr < (SLAVE1_BASE + SLAVE1_SIZE));
    assign addr_match_wr[0] = (m.awaddr >= SLAVE0_BASE) && (m.awaddr < (SLAVE0_BASE + SLAVE0_SIZE));
    assign addr_match_wr[1] = (m.awaddr >= SLAVE1_BASE) && (m.awaddr < (SLAVE1_BASE + SLAVE1_SIZE));

    // 读状态机next逻辑
    always_comb begin
        unique case (rd_state)
            IDLE_RD:
            next_rd_state = (m.arvalid && m.arready) ?
                (addr_match_rd[0] ? S0_RD :
                 addr_match_rd[1] ? S1_RD : IDLE_RD) : IDLE_RD;
            S0_RD: next_rd_state = (s0.rvalid && m.rready) ? IDLE_RD : S0_RD;
            S1_RD: next_rd_state = (s1.rvalid && m.rready) ? IDLE_RD : S1_RD;
        endcase
    end

    // 写状态机next逻辑
    always_comb begin
        unique case (wr_state)
            IDLE_WR:
            next_wr_state = (m.awvalid && m.awready) ?
                (addr_match_wr[0] ?
                    (m.wvalid && m.wready ? S0_WAIT_WRESP : S0_WAIT_WDATA) :
                 addr_match_wr[1] ?
                    (m.wvalid && m.wready ? S1_WAIT_WRESP : S1_WAIT_WDATA) : IDLE_WR) : IDLE_WR;
            S0_WAIT_WDATA: next_wr_state = (m.wvalid && m.wready) ? S0_WAIT_WRESP : S0_WAIT_WDATA;
            S0_WAIT_WRESP: next_wr_state = (s0.bvalid && m.bready) ? IDLE_WR : S0_WAIT_WRESP;
            S1_WAIT_WDATA: next_wr_state = (m.wvalid && m.wready) ? S1_WAIT_WRESP : S1_WAIT_WDATA;
            S1_WAIT_WRESP: next_wr_state = (s1.bvalid && m.bready) ? IDLE_WR : S1_WAIT_WRESP;
            default: next_wr_state = IDLE_WR;
        endcase
    end

    // 读地址通道
    assign s0.arvalid = (rd_state == IDLE_RD) && m.arvalid && addr_match_rd[0];
    assign s1.arvalid = (rd_state == IDLE_RD) && m.arvalid && addr_match_rd[1];
    assign s0.araddr = m.araddr;
    assign s1.araddr = m.araddr;
    assign m.arready = (rd_state == IDLE_RD) && ((addr_match_rd[0] && s0.arready) || (addr_match_rd[1] && s1.arready));

    // 读数据通道
    assign m.rvalid = (rd_state == S0_RD) ? s0.rvalid : (rd_state == S1_RD) ? s1.rvalid : 1'b0;
    assign s0.rready = (rd_state == S0_RD) ? m.rready : 1'b0;
    assign s1.rready = (rd_state == S1_RD) ? m.rready : 1'b0;
    assign m.rdata = (rd_state == S0_RD) ? s0.rdata : s1.rdata;
    assign m.rresp = (rd_state == S0_RD) ? s0.rresp : (rd_state == S1_RD) ? s1.rresp : 1'b0;

    // 写地址通道
    assign s0.awvalid = (wr_state == IDLE_WR) && m.awvalid && addr_match_wr[0];
    assign s1.awvalid = (wr_state == IDLE_WR) && m.awvalid && addr_match_wr[1];
    assign s0.awaddr = m.awaddr;
    assign s1.awaddr = m.awaddr;
    assign m.awready = (wr_state == IDLE_WR) && ((addr_match_wr[0] && s0.awready) || (addr_match_wr[1] && s1.awready));

    // 写数据通道
    assign s0.wvalid = (wr_state == IDLE_WR || wr_state == S0_WAIT_WDATA) && m.wvalid && (addr_match_wr[0] || wr_state == S0_WAIT_WDATA);
    assign s1.wvalid = (wr_state == IDLE_WR || wr_state == S1_WAIT_WDATA) && m.wvalid && (addr_match_wr[1] || wr_state == S1_WAIT_WDATA);
    assign s0.wdata = m.wdata;
    assign s1.wdata = m.wdata;
    assign s0.wmask = m.wmask;
    assign s1.wmask = m.wmask;
    assign m.wready = ((wr_state == IDLE_WR || wr_state == S0_WAIT_WDATA) && s0.wready && (addr_match_wr[0] || wr_state == S0_WAIT_WDATA)) ||
                      ((wr_state == IDLE_WR || wr_state == S1_WAIT_WDATA) && s1.wready && (addr_match_wr[1] || wr_state == S1_WAIT_WDATA));

    // 写回复通道
    assign m.bvalid = (wr_state == S0_WAIT_WRESP) ? s0.bvalid : (wr_state == S1_WAIT_WRESP) ? s1.bvalid : 1'b0;
    assign s0.bready = (wr_state == S0_WAIT_WRESP) ? m.bready : 1'b0;
    assign s1.bready = (wr_state == S1_WAIT_WRESP) ? m.bready : 1'b0;
    assign m.bresp = (wr_state == S0_WAIT_WRESP) ? s0.bresp : (wr_state == S1_WAIT_WRESP) ? s1.bresp : 1'b0;

endmodule
