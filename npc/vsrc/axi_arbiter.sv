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
        IDLE_READ,
        M0_READ,
        M1_READ
    } read_state_t;

    read_state_t read_state, next_read_state;

    // 定义写状态枚举
    typedef enum logic [2:0] {
        IDLE_WRITE,
        M0_WRITE,
        M0_AWRITE,
        M1_AWRITE,
        M1_WRITE
    } write_state_t;

    write_state_t write_state, next_write_state;

    // 状态机更新逻辑
    always_ff @(posedge clk) begin
        if (reset) begin
            read_state  <= IDLE_READ;
            write_state <= IDLE_WRITE;
        end else begin
            read_state  <= next_read_state;
            write_state <= next_write_state;
        end
    end

    // 读状态机next逻辑
    always_comb begin
        unique case (read_state)
            IDLE_READ:
            next_read_state = (m0_arvalid&&m0_arready) ? M0_READ :
                                         (m1_arvalid&&m1_arready) ? M1_READ : IDLE_READ;
            M0_READ: next_read_state = (s_rvalid && m0_rready) ? IDLE_READ : M0_READ;
            M1_READ: next_read_state = (s_rvalid && m1_rready) ? IDLE_READ : M1_READ;
        endcase
    end

    // 写状态机next逻辑
    always_comb begin
        unique case (write_state)
            IDLE_WRITE:
            next_write_state = (m0_awvalid && m0_awready) ? (m0_wvalid && m0_wready ? M0_WRITE : M0_AWRITE) :
                                         (m1_awvalid && m1_awready) ? (m1_wvalid && m1_wready ? M1_WRITE : M1_AWRITE) : IDLE_WRITE;
            M0_AWRITE: next_write_state = (m0_wvalid && m0_wready) ? M0_WRITE : M0_AWRITE;
            M0_WRITE: next_write_state = (s_bvalid && m0_bready) ? IDLE_WRITE : M0_WRITE;
            M1_AWRITE: next_write_state = (m1_wvalid && m1_wready) ? M1_WRITE : M1_AWRITE;
            M1_WRITE: next_write_state = (s_bvalid && m1_bready) ? IDLE_WRITE : M1_WRITE;
        endcase
    end


    // 读地址通道
    assign s_arvalid = (read_state == IDLE_READ) && (m0_arvalid || m1_arvalid);
    assign s_araddr = m0_arvalid ? m0_araddr : m1_araddr;
    assign m0_arready = (read_state == IDLE_READ) ? s_arready : 1'b0;
    assign m1_arready = (read_state == IDLE_READ) ? (s_arready && !m0_arvalid) : 1'b0;

    // 读数据通道
    assign m0_rvalid = (read_state == M0_READ) ? s_rvalid : 1'b0;
    assign m1_rvalid = (read_state == M1_READ) ? s_rvalid : 1'b0;
    assign s_rready  = (read_state == M0_READ) ? m0_rready :
                       (read_state == M1_READ) ? m1_rready : 1'b0;
    assign m0_rdata = s_rdata;
    assign m1_rdata = s_rdata;
    assign m0_rresp = (read_state == M0_READ) ? s_rresp : 1'b0;
    assign m1_rresp = (read_state == M1_READ) ? s_rresp : 1'b0;

    // 写地址通道
    assign s_awvalid = (write_state == IDLE_WRITE) && (m0_awvalid || m1_awvalid);
    assign s_awaddr = m0_awvalid ? m0_awaddr : m1_awaddr;
    assign m0_awready = (write_state == IDLE_WRITE) ? s_awready : 1'b0;
    assign m1_awready = (write_state == IDLE_WRITE) ? (s_awready && !m0_awvalid) : 1'b0;

    // 写数据通道
    assign s_wvalid = (write_state == IDLE_WRITE || write_state == M0_AWRITE || write_state == M1_AWRITE) && (m0_wvalid || m1_wvalid);
    assign s_wdata = m0_wvalid ? m0_wdata : m1_wdata;
    assign s_wmask = m0_wvalid ? m0_wmask : m1_wmask;
    assign m0_wready = (write_state == IDLE_WRITE || write_state == M0_AWRITE || write_state == M1_AWRITE) ? s_wready : 1'b0;
    assign m1_wready = (write_state == IDLE_WRITE || write_state == M0_AWRITE || write_state == M1_AWRITE) ? (s_wready && !m0_wvalid) : 1'b0;

    // 写回复通道
    assign m0_bvalid = (write_state == M0_WRITE) ? s_bvalid : 1'b0;
    assign m1_bvalid = (write_state == M1_WRITE) ? s_bvalid : 1'b0;
    assign s_bready  = (write_state == M0_WRITE) ? m0_bready :
                       (write_state == M1_WRITE) ? m1_bready : 1'b0;
    assign m0_bresp = (write_state == M0_WRITE) ? s_bresp : 1'b0;
    assign m1_bresp = (write_state == M1_WRITE) ? s_bresp : 1'b0;

endmodule

