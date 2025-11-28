/* verilator lint_off DECLFILENAME */

// AXI4-Lite仲裁器模块
// 支持2个master和1个slave。
module axi4_lite_arbiter (
    input logic               clk,
    input logic               reset,
          axi4_lite_if.slave  m0,     // Master 0 接口
          axi4_lite_if.slave  m1,     // Master 1 接口
          axi4_lite_if.master s       // Slave 接口
);

    // 读通道select信号（one-hot编码）
    logic [1:0] rd_select, rd_select_reg;  // [1]: m1, [0]: m0

    // 写通道select信号（one-hot编码）
    logic [1:0] aw_select, wr_select, wr_select_reg;  // [1]: m1, [0]: m0

    // 读select逻辑（仲裁：m1优先）
    assign rd_select = m1.arvalid ? 2'b10 : 2'b01;
    always @(posedge clk) begin
        if (reset) rd_select_reg <= 2'b00;
        else if (s.arvalid && s.arready) rd_select_reg <= rd_select;
    end

    // 写select逻辑（仲裁：m1优先）
    assign wr_select = (m1.awvalid && s.awready || m1.wvalid && s.wready) ? 2'b10 : 2'b01;
    assign aw_select = m1.awvalid ? 2'b10 : 2'b01;
    always @(posedge clk) begin
        if (reset) wr_select_reg <= 2'b00;
        else if (s.awready && s.awvalid || s.wready && s.wvalid) wr_select_reg <= wr_select;
    end

    // 定义读状态枚举（解耦后只有IDLE和WAIT）
    typedef enum logic {
        IDLE_RD,
        WAIT_RRESP
    } rd_state_t;

    rd_state_t rd_state, next_rd_state;

    // 定义写状态枚举（解耦后只有IDLE和WAIT）
    typedef enum logic {
        IDLE_WR,
        WAIT_WRESP
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
        case (rd_state)
            IDLE_RD: next_rd_state = (s.arvalid && s.arready) ? WAIT_RRESP : IDLE_RD;
            WAIT_RRESP: next_rd_state = (s.rvalid && s.rready) ? IDLE_RD : WAIT_RRESP;
            default: next_rd_state = IDLE_RD;
        endcase
    end

    // 写状态机next逻辑
    // logic m0_wr_fire, m1_wr_fire;
    // assign m0_wr_fire = (m0.awvalid && m0.awready) || (m0.wvalid && m0.wready);
    // assign m1_wr_fire = (m1.awvalid && m1.awready) || (m1.wvalid && m1.wready);

    always_comb begin
        case (wr_state)
            IDLE_WR:
            next_wr_state = (s.awvalid && s.awready || s.wvalid && s.wready) ? WAIT_WRESP : IDLE_WR;
            WAIT_WRESP: next_wr_state = (s.bvalid && s.bready) ? IDLE_WR : WAIT_WRESP;
            default: next_wr_state = IDLE_WR;
        endcase
    end

    // 读地址通道
    assign s.arvalid = (rd_state == IDLE_RD) && (m0.arvalid || m1.arvalid);
    assign s.araddr = rd_select[1] ? m1.araddr : m0.araddr;
    // assign s.araddr = (m1.arvalid && m1.arready) ? m1.araddr : m0.araddr;
    // assign s.araddr = (m0.arvalid && m0.arready) ? m0.araddr : m1.araddr;
    assign m0.arready = (rd_state == IDLE_RD) && s.arready && rd_select[0];
    assign m1.arready = (rd_state == IDLE_RD) && s.arready && rd_select[1];

    // 读数据通道
    assign m0.rvalid = (rd_state == WAIT_RRESP) && s.rvalid && rd_select_reg[0];
    assign m1.rvalid = (rd_state == WAIT_RRESP) && s.rvalid && rd_select_reg[1];
    assign s.rready = (rd_state == WAIT_RRESP) && ((m0.rready && rd_select_reg[0]) || (m1.rready && rd_select_reg[1]));
    assign m0.rdata = s.rdata;
    assign m1.rdata = s.rdata;
    assign m0.rresp = s.rresp;
    assign m1.rresp = s.rresp;

    // 写地址通道
    assign s.awvalid = (wr_state == IDLE_WR) ? (m0.awvalid || m1.awvalid) :
                       ((m0.awvalid && wr_select_reg[0]) || (m1.awvalid && wr_select_reg[1]));
    assign s.awaddr = (wr_state == IDLE_WR) ? (aw_select[1] ? m1.awaddr : m0.awaddr) :
                      (wr_select_reg[1] ? m1.awaddr : m0.awaddr);
    // assign s.awaddr = (m1.awvalid && m1.awready) ? m1.awaddr : m0.awaddr;
    // assign s.awaddr = (m0.awvalid && m0.awready) ? m0.awaddr : m1.awaddr;
    assign m0.awready = (wr_state == IDLE_WR && aw_select[0]) || (wr_state == WAIT_WRESP && wr_select_reg[0]) && s.awready;
    assign m1.awready = (wr_state == IDLE_WR && aw_select[1]) || (wr_state == WAIT_WRESP && wr_select_reg[1]) && s.awready;

    // 写数据通道
    assign s.wvalid = (wr_state == IDLE_WR) ? (m0.wvalid || m1.wvalid) :
                      ((m0.wvalid && wr_select_reg[0]) || (m1.wvalid && wr_select_reg[1]));
    // assign {s.wdata,s.wstrb} = (wr_state == IDLE_WR) ? (wr_select[1] ? {m1.wdata,m1.wstrb} : {m0.wdata,m0.wstrb}) :
    //                  (wr_select_reg[1] ? {m1.wdata,m1.wstrb} : {m0.wdata,m0.wstrb});
    assign {s.wdata,s.wstrb} = (m1.wvalid && m1.wready) ? {m1.wdata,m1.wstrb} : {m0.wdata,m0.wstrb};
    assign m0.wready = (wr_state == IDLE_WR && wr_select[0]) || (wr_state == WAIT_WRESP && wr_select_reg[0]) && s.wready;
    assign m1.wready = (wr_state == IDLE_WR && wr_select[1]) || (wr_state == WAIT_WRESP && wr_select_reg[1]) && s.wready;

    // 写回复通道
    assign m0.bvalid = (wr_state == WAIT_WRESP) && s.bvalid && wr_select_reg[0];
    assign m1.bvalid = (wr_state == WAIT_WRESP) && s.bvalid && wr_select_reg[1];
    assign s.bready = (wr_state == WAIT_WRESP) && ((m0.bready && wr_select_reg[0]) || (m1.bready && wr_select_reg[1]));
    assign m0.bresp = s.bresp;
    assign m1.bresp = s.bresp;

endmodule

