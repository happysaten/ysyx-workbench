/* verilator lint_off DECLFILENAME */

// AXI4-Lite Crossbar (1 master to N slaves)
// 根据地址范围选择从设备

module xbar #(
    parameter int NUM_SLAVES = 2,
    parameter logic [NUM_SLAVES-1:0][31:0] SLAVE_BASE = {32'ha00003f8, 32'h80000000},
    parameter logic [NUM_SLAVES-1:0][31:0] SLAVE_SIZE = {32'h00000004, 32'h08000000}
) (
    input logic              clk,
    input logic              reset,
          axi_lite_if.slave  m,                 // 1 个 Master (从crossbar角度看是slave接口)
          axi_lite_if.master s    [NUM_SLAVES]  // N 个 Slave 接口
);

    // 定义读状态
    typedef enum logic [1:0] {
        IDLE_RD,
        WAIT_RRESP
    } rd_state_t;

    rd_state_t rd_state, next_rd_state;

    // 定义写状态
    typedef enum logic [1:0] {
        IDLE_WR,
        WAIT_WRESP
    } wr_state_t;

    wr_state_t wr_state, next_wr_state;

    // 地址匹配信号
    logic [NUM_SLAVES-1:0] addr_match_rd;
    logic [NUM_SLAVES-1:0] addr_match_wr;

    // 选中的slave索引(one-hot编码)
    logic [NUM_SLAVES-1:0] rd_slave_sel;
    logic [NUM_SLAVES-1:0] wr_slave_sel;

    // 生成地址匹配逻辑
    genvar i;
    // generate
    //     for (i = 0; i < NUM_SLAVES; i++) begin : gen_addr_match
    //         assign addr_match_rd[i] = (m.araddr >= SLAVE_BASE[i]) && 
    //                                  (m.araddr < (SLAVE_BASE[i] + SLAVE_SIZE[i]));
    //         assign addr_match_wr[i] = (m.awaddr >= SLAVE_BASE[i]) && 
    //                                  (m.awaddr < (SLAVE_BASE[i] + SLAVE_SIZE[i]));
    //     end
    // endgenerate

    // 生成地址匹配逻辑（独热编码）
    always_comb begin
        addr_match_rd = '0;
        for (int j = 0; j < NUM_SLAVES; j++) begin
            if ((m.araddr >= SLAVE_BASE[j]) && (m.araddr < (SLAVE_BASE[j] + SLAVE_SIZE[j]))) begin
                addr_match_rd[j] = 1'b1;
                break;
            end
        end
        addr_match_wr = '0;
        for (int j = 0; j < NUM_SLAVES; j++) begin
            if ((m.awaddr >= SLAVE_BASE[j]) && (m.awaddr < (SLAVE_BASE[j] + SLAVE_SIZE[j]))) begin
                addr_match_wr[j] = 1'b1;
                break;
            end
        end
    end

    // ============ 读通道状态机 ============
    always_ff @(posedge clk) begin
        if (reset) begin
            rd_state <= IDLE_RD;
            rd_slave_sel <= '0;
        end else begin
            rd_state <= next_rd_state;
            if (rd_state == IDLE_RD && m.arvalid && m.arready) begin
                rd_slave_sel <= addr_match_rd;
            end
        end
    end

    // ============ 写通道状态机 ============
    always_ff @(posedge clk) begin
        if (reset) begin
            wr_state <= IDLE_WR;
            wr_slave_sel <= '0;
        end else begin
            wr_state <= next_wr_state;
            if (wr_state == IDLE_WR && m.awvalid && m.awready) begin
                wr_slave_sel <= addr_match_wr;
            end
        end
    end

    // 读状态机next逻辑

    always_comb begin
        case (rd_state)
            IDLE_RD: begin
                if (m.arvalid && |addr_match_rd) begin
                    next_rd_state = WAIT_RRESP;
                end else begin
                    next_rd_state = IDLE_RD;
                end
            end
            WAIT_RRESP: begin
                next_rd_state = (m.rvalid && m.rready) ? IDLE_RD : WAIT_RRESP;
            end
            default: next_rd_state = IDLE_RD;
        endcase
    end

    // 写状态机next逻辑
    logic wr_addr_fire, wr_resp_fire;
    assign wr_addr_fire = m.awvalid && m.awready;
    assign wr_resp_fire = m.bvalid && m.bready;

    always_comb begin
        case (wr_state)
            IDLE_WR: begin
                if (wr_addr_fire && |addr_match_wr) begin
                    next_wr_state = WAIT_WRESP;
                end else begin
                    next_wr_state = IDLE_WR;
                end
            end
            WAIT_WRESP: begin
                next_wr_state = wr_resp_fire ? IDLE_WR : WAIT_WRESP;
            end
            default: next_wr_state = IDLE_WR;
        endcase
    end

    // ============ 读通道连接 ============
    // 读地址通道
    logic [NUM_SLAVES-1:0] s_arready_vec;
    generate
        for (i = 0; i < NUM_SLAVES; i++) begin : gen_read_addr
            assign s[i].arvalid = (rd_state == IDLE_RD) && m.arvalid && addr_match_rd[i];
            assign s[i].araddr = m.araddr;
            assign s_arready_vec[i] = addr_match_rd[i] && s[i].arready;
        end
    endgenerate
    assign m.arready = (rd_state == IDLE_RD) && |s_arready_vec;

    // 读数据通道
    logic [NUM_SLAVES-1:0] s_rvalid_vec;

    generate
        for (i = 0; i < NUM_SLAVES; i++) begin : gen_read_data
            assign s[i].rready = (rd_state == WAIT_RRESP) && rd_slave_sel[i] && m.rready;
            assign s_rvalid_vec[i] = rd_slave_sel[i] && s[i].rvalid;
        end
    endgenerate

    assign m.rvalid = |s_rvalid_vec;

    logic [NUM_SLAVES-1:0][31:0] rdata_array;
    logic [NUM_SLAVES-1:0][ 1:0] rresp_array;
    generate
        for (i = 0; i < NUM_SLAVES; i++) begin : gen_rdata_array
            assign rdata_array[i] = s[i].rdata;
            assign rresp_array[i] = s[i].rresp;
        end
    endgenerate

    always_comb begin
        m.rdata = '0;
        m.rresp = 2'b00;
        for (int j = 0; j < NUM_SLAVES; j++) begin
            if (rd_slave_sel[j]) begin
                m.rdata = rdata_array[j];
                m.rresp = rresp_array[j];
            end
        end
    end

    // ============ 写通道连接 ============
    // 写地址通道
    logic [NUM_SLAVES-1:0] s_awready_vec;
    generate
        for (i = 0; i < NUM_SLAVES; i++) begin : gen_write_addr
            assign s[i].awvalid = (wr_state == IDLE_WR) && m.awvalid && addr_match_wr[i];
            assign s[i].awaddr = m.awaddr;
            assign s_awready_vec[i] = addr_match_wr[i] && s[i].awready;
        end
    endgenerate
    assign m.awready = (wr_state == IDLE_WR) && |s_awready_vec;

    // 写数据通道
    logic [NUM_SLAVES-1:0] s_wready_vec;
    generate
        for (i = 0; i < NUM_SLAVES; i++) begin : gen_write_data
            assign s[i].wvalid = m.wvalid && ((wr_state == IDLE_WR) ? addr_match_wr[i] : wr_slave_sel[i]);
            assign s[i].wdata = m.wdata;
            assign s[i].wmask = m.wmask;
            assign s_wready_vec[i] = ((wr_state == IDLE_WR) ? addr_match_wr[i] : wr_slave_sel[i]) && 
                                     s[i].wready;
        end
    endgenerate
    assign m.wready = |s_wready_vec;

    // 写回复通道
    logic [NUM_SLAVES-1:0] s_bvalid_vec;

    generate
        for (i = 0; i < NUM_SLAVES; i++) begin : gen_write_resp
            assign s[i].bready = (wr_state == WAIT_WRESP) && wr_slave_sel[i] && m.bready;
            assign s_bvalid_vec[i] = wr_slave_sel[i] && s[i].bvalid;
        end
    endgenerate

    assign m.bvalid = |s_bvalid_vec;

    logic [NUM_SLAVES-1:0][1:0] bresp_array;
    generate
        for (i = 0; i < NUM_SLAVES; i++) begin : gen_bresp_array
            assign bresp_array[i] = s[i].bresp;
        end
    endgenerate

    always_comb begin
        m.bresp = 2'b00;
        for (int j = 0; j < NUM_SLAVES; j++) begin
            if (wr_slave_sel[j]) begin
                m.bresp = bresp_array[j];
            end
        end
    end

endmodule
