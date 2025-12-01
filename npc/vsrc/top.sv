// import alu_pkg::*;

// 指令类型枚举
typedef enum logic [2:0] {
    TYPE_R,  // R型指令，寄存器间操作
    TYPE_I,  // I型指令，带有立即数和寄存器操作数
    TYPE_S,  // S型指令，存储指令
    TYPE_B,  // B型指令，分支指令
    TYPE_U,  // U型指令，带有高位立即数
    TYPE_J,  // J型指令，JAL跳转指令
    TYPE_N   // none，无操作数类型
} inst_t;

/* verilator lint_off DECLFILENAME */

module ysyx_25050142 (
    // input clk,  // 时钟信号
    // input reset,  // 复位信号
    // output logic npc_req_ready,
    // output logic npc_resp_valid,
    // output logic npc_error,
    input clock,        // 时钟（原 clk）
    input reset,        // 复位（高电平有效）
    input io_interrupt, // 外部中断

    // AXI4 Master 总线（CPU 作为 master 对外）
    input         io_master_awready,
    output        io_master_awvalid,
    output [31:0] io_master_awaddr,
    output [ 3:0] io_master_awid,
    output [ 7:0] io_master_awlen,
    output [ 2:0] io_master_awsize,
    output [ 1:0] io_master_awburst,

    input         io_master_wready,
    output        io_master_wvalid,
    output [31:0] io_master_wdata,
    output [ 3:0] io_master_wstrb,
    output        io_master_wlast,

    output       io_master_bready,
    input        io_master_bvalid,
    input  [1:0] io_master_bresp,
    input  [3:0] io_master_bid,

    input         io_master_arready,
    output        io_master_arvalid,
    output [31:0] io_master_araddr,
    output [ 3:0] io_master_arid,
    output [ 7:0] io_master_arlen,
    output [ 2:0] io_master_arsize,
    output [ 1:0] io_master_arburst,

    output        io_master_rready,
    input         io_master_rvalid,
    input  [ 1:0] io_master_rresp,
    input  [31:0] io_master_rdata,
    input         io_master_rlast,
    input  [ 3:0] io_master_rid,

    // AXI4 Slave 总线（可忽略）
    output        io_slave_awready,
    input         io_slave_awvalid,
    input  [31:0] io_slave_awaddr,
    input  [ 3:0] io_slave_awid,
    input  [ 7:0] io_slave_awlen,
    input  [ 2:0] io_slave_awsize,
    input  [ 1:0] io_slave_awburst,

    output        io_slave_wready,
    input         io_slave_wvalid,
    input  [31:0] io_slave_wdata,
    input  [ 3:0] io_slave_wstrb,
    input         io_slave_wlast,

    input        io_slave_bready,
    output       io_slave_bvalid,
    output [1:0] io_slave_bresp,
    output [3:0] io_slave_bid,

    output        io_slave_arready,
    input         io_slave_arvalid,
    input  [31:0] io_slave_araddr,
    input  [ 3:0] io_slave_arid,
    input  [ 7:0] io_slave_arlen,
    input  [ 2:0] io_slave_arsize,
    input  [ 1:0] io_slave_arburst,

    input         io_slave_rready,
    output        io_slave_rvalid,
    output [ 1:0] io_slave_rresp,
    output [31:0] io_slave_rdata,
    output        io_slave_rlast,
    output [ 3:0] io_slave_rid
);

    logic clk;
    assign clk = clock;

    logic npc_req_ready, npc_resp_valid, npc_error;

    // IFU：负责 PC 和取指
    logic [31:0] pc, snpc, jump_target;  // pc renamed to ifu_raddr, snpc, 跳转目标地址
    logic        jump_en;
    logic [31:0] inst;  // 当前指令, inst renamed to ifu_rdata
    logic [31:0] dnpc;  // dnpc信号，从PCU输出

    logic        reset_sync;
    // 同步复位信号
    always_ff @(posedge clk or posedge reset) begin
        if (reset) reset_sync <= 1'b1;
        else reset_sync <= 1'b0;
    end

    logic ifu_resp_valid, lsu_resp_valid, gpr_resp_valid, csr_resp_valid;
    logic ifu_req_ready, lsu_req_ready, gpr_req_ready, csr_req_ready;
    assign npc_resp_valid = gpr_resp_valid || csr_resp_valid;

    logic npc_req_valid, npc_req_valid_init;
    always @(posedge clk) begin
        if (reset_sync) npc_req_valid_init <= 1'b1;
        else if (ifu_req_ready) npc_req_valid_init <= 1'b0;
    end
    assign npc_req_ready = ifu_req_ready;
    assign npc_req_valid = npc_req_valid_init || npc_resp_valid;

    import "DPI-C" function void report_done_npc(input logic done);
    always_comb report_done_npc(npc_req_ready && npc_resp_valid);

    logic ifu_error, gpr_error, csr_error, lsu_error;
    assign npc_error = ifu_error | gpr_error | csr_error | lsu_error;

    // Crossbar 参数定义
    // localparam int XBAR_NUM_SLAVES = 3;
    // localparam logic [XBAR_NUM_SLAVES-1:0][31:0] XBAR_SLAVE_BASE = {
    //     32'ha0000048, 32'ha00003f8, 32'h80000000
    // };
    // localparam logic [XBAR_NUM_SLAVES-1:0][31:0] XBAR_SLAVE_SIZE = {32'h8, 32'h8, 32'h08000000};
    localparam int XBAR_NUM_SLAVES = 2;
    localparam logic [XBAR_NUM_SLAVES-1:0][31:0] XBAR_SLAVE_BASE = {32'ha0000048, 32'h80000000};
    localparam logic [XBAR_NUM_SLAVES-1:0][31:0] XBAR_SLAVE_SIZE = {32'h8, 32'h08000000};
    // 创建AXI接口实例
    axi4_if imem_if ();  // IFU(取指)接口
    axi4_if dmem_if ();  // LSU(访存)接口
    axi4_if arbiter_if ();  // 仲裁器输出接口
    // axi4_lite_if uart_if ();  // UART接口
    // axi4_lite_if mem_if ();  // 统一内存接口
    axi4_if xbar_if[XBAR_NUM_SLAVES] ();

    assign xbar_if[0].awready = io_master_awready;
    assign io_master_awvalid = xbar_if[0].awvalid;
    assign io_master_awaddr = xbar_if[0].awaddr;
    assign io_master_awid = xbar_if[0].awid;
    assign io_master_awlen = xbar_if[0].awlen;
    assign io_master_awsize = xbar_if[0].awsize;
    assign io_master_awburst = xbar_if[0].awburst;

    assign xbar_if[0].wready = io_master_wready;
    assign io_master_wvalid = xbar_if[0].wvalid;
    assign io_master_wdata = xbar_if[0].wdata;
    assign io_master_wstrb = xbar_if[0].wstrb;
    assign io_master_wlast = xbar_if[0].wlast;
    assign io_master_bready = xbar_if[0].bready;
    assign xbar_if[0].bvalid = io_master_bvalid;
    assign xbar_if[0].bresp = io_master_bresp;
    assign xbar_if[0].bid = io_master_bid;

    assign xbar_if[0].arready = io_master_arready;
    assign io_master_arvalid = xbar_if[0].arvalid;
    assign io_master_araddr = xbar_if[0].araddr;
    assign io_master_arid = xbar_if[0].arid;
    assign io_master_arlen = xbar_if[0].arlen;
    assign io_master_arsize = xbar_if[0].arsize;
    assign io_master_arburst = xbar_if[0].arburst;

    assign io_master_rready = xbar_if[0].rready;
    assign xbar_if[0].rvalid = io_master_rvalid;
    assign xbar_if[0].rresp = io_master_rresp;
    assign xbar_if[0].rdata = io_master_rdata;
    assign xbar_if[0].rlast = io_master_rlast;
    assign xbar_if[0].rid = io_master_rid;

    assign {io_slave_awready,io_slave_wready,io_slave_bvalid,io_slave_bresp,io_slave_bid,
            io_slave_arready,io_slave_rvalid,io_slave_rresp,io_slave_rdata,io_slave_rlast,io_slave_rid} =
           '0;



    // 实例化AXI仲裁器
    axi_arbiter u_arbiter (
        .clk  (clk),
        .reset(reset_sync),
        .m0   (imem_if.slave),     // Master 0: IFU (取指)
        .m1   (dmem_if.slave),     // Master 1: LSU (访存)
        .s    (arbiter_if.master)  // 输出到xbar
    );

    // 实例化Crossbar
    xbar #(
        .NUM_SLAVES(XBAR_NUM_SLAVES),
        .SLAVE_BASE(XBAR_SLAVE_BASE),
        .SLAVE_SIZE(XBAR_SLAVE_SIZE)
    ) u_xbar (
        .clk  (clk),
        .reset(reset_sync),
        .m    (arbiter_if.slave),
        // .s0   (uart_if.master),    // Slave 0: UART
        // .s1   (mem_if.master)      // Slave 1: 统一内存
        .s    (xbar_if.master)
    );

    // // 实例化统一内存模块
    // MEM u_mem (
    //     .clk(clk),
    //     .reset(reset_sync),
    //     // .s  (mem_if.slave)
    //     .s(xbar_if[0].slave)
    // );

    // // 实例化UART模块
    // uart #(
    //     .UART_ADDR(XBAR_SLAVE_BASE[1]),
    //     .UART_SIZE(XBAR_SLAVE_SIZE[1])

    // ) u_uart (
    //     .clk  (clk),
    //     .reset(reset_sync),
    //     // .s    (uart_if.slave)
    //     .s    (xbar_if[1].slave)
    // );

    clint #(
        .MTIME_ADDR(XBAR_SLAVE_BASE[1]),
        .MTIME_SIZE(XBAR_SLAVE_SIZE[1])
    ) u_clint (
        .clk  (clk),
        .reset(reset_sync),
        .s    (xbar_if[1].slave)
    );

    IFU u_ifu (
        .clk           (clk),
        .reset         (reset_sync),
        .ifu_req_valid (npc_req_valid),
        .ifu_req_ready (ifu_req_ready),
        .jump_en       (jump_en),
        .jump_target   (jump_target),
        .ifu_rdata     (inst),
        .ifu_resp_valid(ifu_resp_valid),
        .ifu_resp_ready(lsu_req_ready),
        .pc            (pc),
        .snpc          (snpc),
        .dnpc          (dnpc),
        .ifu_error     (ifu_error),
        // IMEM接口 - 使用interface
        .imem          (imem_if.master)
    );

    // IDU：负责指令解码
    // 解码信号
    logic [ 6:0] opcode;
    logic [ 2:0] funct3;
    logic [31:0] imm;
    logic [4:0] rs1, rs2, rd;
    inst_t   inst_type;
    alu_op_t alu_op;
    IDU u_idu (
        .inst(inst),
        .opcode(opcode),
        .funct3(funct3),
        .rd(rd),
        .rs1(rs1),
        .rs2(rs2),
        .imm(imm),
        .inst_type(inst_type),
        .alu_op(alu_op)
    );

    // GPR：通用寄存器组
    logic [31:0]
        gpr_rdata1,
        gpr_rdata2,
        gpr_wdata;  // 读寄存器组数据1、2(src1, src2)，写寄存器数据
    logic gpr_we;
    GPR u_gpr (
        .clk(clk),
        .reset(reset_sync),
        .gpr_req_valid(lsu_resp_valid),
        .gpr_req_ready(gpr_req_ready),
        .gpr_wen(gpr_we && (|rd)),
        .gpr_waddr(rd),
        .gpr_wdata(gpr_wdata),
        .gpr_raddr1(rs1),
        .gpr_raddr2(rs2),
        .gpr_resp_valid(gpr_resp_valid),
        .gpr_resp_ready(ifu_req_ready),
        .gpr_rdata1(gpr_rdata1),
        .gpr_rdata2(gpr_rdata2),
        .gpr_error(gpr_error)
    );

    // CSR：控制状态寄存器
    logic [3:0][31:0] csr_wdata, csr_rdata;  // CSR写数据，读数据
    logic [3:0] csr_we;
    CSR u_csr (
        .clk(clk),
        .reset(reset_sync),
        .csr_req_valid(lsu_resp_valid),
        .csr_req_ready(csr_req_ready),
        .csr_wen(csr_we),
        .csr_wdata(csr_wdata),
        .csr_resp_valid(csr_resp_valid),
        .csr_resp_ready(ifu_req_ready),
        .csr_rdata(csr_rdata),
        .csr_error(csr_error)
    );


    // EXU：负责根据控制信号来进行运算和跳转
    logic [31:0] alu_result;  // ALU计算结果
    logic [31:0] csr_read_data;  // CSR读取的数据
    EXU u_exu (
        .opcode       (opcode),
        .funct3       (funct3),
        .gpr_rdata1   (gpr_rdata1),
        .gpr_rdata2   (gpr_rdata2),
        .imm          (imm),
        .pc           (pc),
        .snpc         (snpc),
        .inst_type    (inst_type),
        .csr_rdata    (csr_rdata),
        .alu_op       (alu_op),
        .alu_result   (alu_result),
        .jump_target  (jump_target),
        .jump_en      (jump_en),
        .csr_we       (csr_we),
        .csr_wdata    (csr_wdata),
        .csr_read_data(csr_read_data)
    );

    // LSU：负责加载和存储指令的内存访问
    logic [31:0] lsu_rdata;  // 加载数据
    LSU u_lsu (
        .clk           (clk),
        .reset         (reset_sync),
        .lsu_req_valid (ifu_resp_valid),
        .lsu_req_ready (lsu_req_ready),
        .inst_type     (inst_type),
        .opcode        (opcode),
        .funct3        (funct3),
        .pc            (pc),
        .alu_result    (alu_result),
        .gpr_rdata2    (gpr_rdata2),
        .lsu_resp_valid(lsu_resp_valid),
        .lsu_resp_ready(gpr_req_ready && csr_req_ready),
        .lsu_rdata     (lsu_rdata),
        .lsu_error     (lsu_error),
        // DMEM接口 - 使用interface
        .dmem          (dmem_if.master)
    );

    // WBU：负责写回GPR
    WBU u_wbu (
        .inst_type    (inst_type),
        .opcode       (opcode),
        .funct3       (funct3),
        .pc           (pc),
        .snpc         (snpc),
        .alu_result   (alu_result),
        .lsu_rdata    (lsu_rdata),
        .csr_read_data(csr_read_data),
        .gpr_wdata    (gpr_wdata),
        .gpr_we       (gpr_we)
    );

endmodule

// IFU(Instruction Fetch Unit) 负责PC管理和取指
module IFU (
    input                        clk,
    input                        reset,
    input                        ifu_req_valid,
    output logic                 ifu_req_ready,
    input                        jump_en,
    input                 [31:0] jump_target,
    output logic          [31:0] ifu_rdata,
    output logic                 ifu_resp_valid,
    input                        ifu_resp_ready,
    output logic          [31:0] pc,
    output logic          [31:0] snpc,
    output logic          [31:0] dnpc,
    output logic                 ifu_error,
    // IMEM接口 - 使用interface
           axi4_if.master        imem
);

    // snpc / dnpc 选择逻辑
    assign snpc          = pc + 4;
    assign dnpc          = jump_en ? jump_target : snpc;

    // IMEM访问控制 - 只使用读通道
    assign imem.araddr   = dnpc;
    assign imem.arvalid  = ifu_req_valid;
    assign imem.arid     = 4'b0;
    assign imem.arlen    = 8'b0;  // Burst length = 1
    assign imem.arsize   = 3'b010;  // 4 bytes
    assign imem.arburst  = 2'b01;  // INCR
    assign imem.rready   = ifu_resp_ready;

    // 写通道全部置为无效
    assign imem.awvalid  = 1'b0;
    assign imem.awaddr   = 32'h0;
    assign imem.awid     = 4'b0;
    assign imem.awlen    = 8'b0;
    assign imem.awsize   = 3'b000;
    assign imem.awburst  = 2'b00;
    assign imem.wvalid   = 1'b0;
    assign imem.wdata    = 32'h0;
    assign imem.wstrb    = 4'h0;
    assign imem.wlast    = 1'b0;
    assign imem.bready   = 1'b0;

    assign ifu_req_ready = imem.arready;
    always @(posedge clk) ifu_resp_valid <= imem.rvalid;
    always @(posedge clk) begin
        if (reset) begin
            ifu_rdata <= 32'h0;
            ifu_error <= 1'b0;
        end else if (imem.rvalid) begin
            ifu_rdata <= imem.rdata;
            ifu_error <= |imem.rresp;  // 修改：检查2位resp是否非零
        end
    end

    import "DPI-C" function void update_inst_npc(
        input int inst,
        input int dnpc
    );
    localparam int RESET_PC = 32'h80000000;

    // PC 寄存器更新 - 握手成功时更新
    always_ff @(posedge clk) begin
        if (reset) pc <= RESET_PC - 4;
        else if (ifu_req_valid && ifu_req_ready) pc <= dnpc;
    end
    always_comb if (ifu_resp_valid) update_inst_npc(ifu_rdata, dnpc);

endmodule

// IDU(Instruction Decode Unit) 负责对当前指令进行译码, 准备执行阶段需要使用的数据和控制信号
module IDU (
    input [31:0] inst,  // 输入指令
    output [6:0] opcode,  // 操作码
    output [2:0] funct3,  // 功能码
    output [4:0] rd,  // 目的寄存器编号
    output [4:0] rs1,  // 源寄存器1编号
    output [4:0] rs2,  // 源寄存器2编号
    output logic [31:0] imm,  // 立即数
    output inst_t inst_type,  // 指令类型输出
    output alu_op_t alu_op  // 输出ALU操作码
);
    logic [6:0] funct7;  // 功能码扩展
    assign opcode = inst[6:0];
    assign funct3 = inst[14:12];
    assign funct7 = inst[31:25];
    assign rd = inst[11:7];
    assign rs1 = inst[19:15];
    assign rs2 = inst[24:20];

    always_comb begin
        unique case (opcode)
            7'b0110011: inst_type = TYPE_R;  // 寄存器类型
            7'b0010011, 7'b0000011, 7'b1100111, 7'b1110011: inst_type = TYPE_I;  // 立即数类型
            7'b0100011: inst_type = TYPE_S;  // 存储类型
            7'b1100011: inst_type = TYPE_B;  // 分支类型
            7'b0110111, 7'b0010111: inst_type = TYPE_U;  // 上位立即数类型
            7'b1101111: inst_type = TYPE_J;  // 跳转类型
            default: inst_type = TYPE_N;  // 无操作数类型
        endcase
    end
    always_comb begin
        unique case (inst_type)
            TYPE_I: imm = {{20{inst[31]}}, inst[31:20]};  // 符号扩展I型立即数
            TYPE_S: imm = {{20{inst[31]}}, inst[31:25], inst[11:7]};  // 符号扩展S型立即数
            TYPE_B:
            imm = {
                {19{inst[31]}}, inst[31], inst[7], inst[30:25], inst[11:8], 1'b0
            };  // 符号扩展B型立即数
            TYPE_U: imm = {inst[31:12], 12'b0};  // U型立即数（高20位）
            TYPE_J:
            imm = {
                {11{inst[31]}}, inst[31], inst[19:12], inst[20], inst[30:21], 1'b0
            };  // 符号扩展J型立即数
            default: imm = 32'h0;
        endcase
    end

    // alu_op解析
    always_comb begin
        alu_op = ALU_ADD;
        if (opcode == 7'b0010011 || opcode == 7'b0110011) begin
            unique case (funct3)
                3'b000: begin  // ADD/ADDI 或 SUB
                    if (inst_type == TYPE_R && funct7 == 7'b0100000) alu_op = ALU_SUB;
                end
                3'b001:  alu_op = ALU_SLL;  // SLLI/SLL
                3'b010:  alu_op = ALU_SLT;  // SLTI/SLT
                3'b011:  alu_op = ALU_SLTU;  // SLTIU/SLTU
                3'b100:  alu_op = ALU_XOR;  // XORI/XOR
                3'b101:  alu_op = (funct7[5] == 1'b1) ? ALU_SRA : ALU_SRL;  // SRAI/SRA 或 SRLI/SRL
                3'b110:  alu_op = ALU_OR;  // ORI/OR
                3'b111:  alu_op = ALU_AND;  // ANDI/AND
                default: alu_op = ALU_ADD;
            endcase
        end
    end
endmodule

// GPR(General Purpose Register) 负责通用寄存器的读写
module GPR (
    input               clk,
    input               reset,
    input               gpr_req_valid,
    output logic        gpr_req_ready,
    input               gpr_wen,
    input        [ 4:0] gpr_waddr,
    input        [31:0] gpr_wdata,
    input        [ 4:0] gpr_raddr1,
    input        [ 4:0] gpr_raddr2,
    output logic        gpr_resp_valid,
    input               gpr_resp_ready,
    output logic [31:0] gpr_rdata1,
    output logic [31:0] gpr_rdata2,
    output logic        gpr_error        // error输出
);
    typedef enum logic [1:0] {
        IDLE,
        WAIT,
        RESP
    } state_t;
    state_t state, next_state;

    always @(posedge clk) begin
        if (reset) state <= IDLE;
        else state <= next_state;
    end

    logic resp_data_ready, req_fire, resp_fire;
    always_comb begin
        unique case (state)
            IDLE: next_state = req_fire ? (resp_data_ready ? RESP : WAIT) : IDLE;
            WAIT: next_state = resp_data_ready ? RESP : WAIT;
            RESP: next_state = resp_fire ? IDLE : RESP;
            default: next_state = IDLE;
        endcase
    end

    assign gpr_resp_valid  = state == RESP;
    assign gpr_req_ready   = state == IDLE;
    assign resp_data_ready = 1'b1;
    assign req_fire        = gpr_req_valid && gpr_req_ready;
    assign resp_fire       = gpr_resp_valid && gpr_resp_ready;

    logic [31:0] regfile[32];  // 寄存器文件

    // import "DPI-C" function void output_gprs(input [31:0] gprs[]);
    // always_comb output_gprs(regfile);  // 输出寄存器状态到DPI-C

    always_ff @(posedge clk) begin
        if (reset) begin
            for (int i = 0; i < 32; i++) regfile[i] <= 32'h0;  // 复位时清零所有寄存器
        end else if (gpr_wen && state != RESP && next_state == RESP) begin  // 修改：添加valid条件
            regfile[gpr_waddr] <= gpr_wdata;
            write_gpr_npc(gpr_waddr, gpr_wdata);
        end
    end


    import "DPI-C" function void write_gpr_npc(
        input logic [ 4:0] idx,
        input logic [31:0] data
    );
    // always_comb begin
    //     if (gpr_wen && state != RESP && next_state == RESP) write_gpr_npc(gpr_waddr, gpr_wdata);
    // end

    always_comb begin
        gpr_rdata1 = (gpr_raddr1 == 5'b0) ? 32'h0 : regfile[gpr_raddr1];
        gpr_rdata2 = (gpr_raddr2 == 5'b0) ? 32'h0 : regfile[gpr_raddr2];
    end

    assign gpr_error = 0;  // 赋值为0

endmodule

// CSR(Control and Status Register) 负责控制和状态寄存器的读写
module CSR #(
    localparam int N = 4
) (
    input                      clk,
    input                      reset,
    input                      csr_req_valid,
    output logic               csr_req_ready,
    input        [N-1:0]       csr_wen,
    input        [N-1:0][31:0] csr_wdata,
    output logic               csr_resp_valid,
    input                      csr_resp_ready,
    output logic [N-1:0][31:0] csr_rdata,
    output logic               csr_error
);
    typedef enum logic [1:0] {
        IDLE,
        WAIT,
        RESP
    } state_t;
    state_t state, next_state;

    always @(posedge clk) begin
        if (reset) state <= IDLE;
        else state <= next_state;
    end

    logic resp_data_ready, req_fire, resp_fire;
    always_comb begin
        unique case (state)
            IDLE: next_state = req_fire ? (resp_data_ready ? RESP : WAIT) : IDLE;
            WAIT: next_state = resp_data_ready ? RESP : WAIT;
            RESP: next_state = resp_fire ? IDLE : RESP;
            default: next_state = IDLE;
        endcase
    end

    assign csr_resp_valid  = state == RESP;
    assign csr_req_ready   = state == IDLE && !reset;
    assign resp_data_ready = 1'b1;
    assign req_fire        = csr_req_valid && csr_req_ready;
    assign resp_fire       = csr_resp_valid && csr_resp_ready;

    always_ff @(posedge clk) begin
        if (reset) csr_rdata <= '0;  // 复位时清零所有CSR寄存器
        else begin
            for (int i = 0; i < N; i++)
            if (csr_req_valid && csr_wen[i] && state != RESP && next_state == RESP)
                csr_rdata[i] <= csr_wdata[i];
        end
    end

    import "DPI-C" function void write_csr_npc(
        input logic [ 1:0] idx,
        input logic [31:0] data
    );

    always_comb begin
        for (int i = 0; i < N; i++)
        if (csr_req_valid && csr_wen[i] && state != RESP && next_state == RESP)
            write_csr_npc(i[1:0], csr_wdata[i]);
    end

    assign csr_error = 0;  // 赋值为0

endmodule

// typedef enum logic [11:0] {
//     CSR_MTVEC   = 12'h305,
//     CSR_MEPC    = 12'h341,
//     CSR_MSTATUS = 12'h300,
//     CSR_MCAUSE  = 12'h342
// } csr_addr_t;

// EXU(Execute Unit) 负责根据控制信号控制ALU, 对数据进行计算
module EXU (
    input           [ 6:0]       opcode,
    input           [ 2:0]       funct3,
    input           [31:0]       gpr_rdata1,
    input           [31:0]       gpr_rdata2,
    input           [31:0]       imm,
    input           [31:0]       pc,
    input           [31:0]       snpc,
    input  inst_t                inst_type,
    input           [ 3:0][31:0] csr_rdata,
    input  alu_op_t              alu_op,
    output logic    [31:0]       alu_result,
    output logic    [31:0]       jump_target,
    output logic                 jump_en,
    output logic    [ 3:0]       csr_we,
    output logic    [ 3:0][31:0] csr_wdata,
    output logic    [31:0]       csr_read_data
);
    import "DPI-C" function void NPCINV(input int pc);
    import "DPI-C" function void NPCTRAP();

    logic [31:0] alu_a, alu_b;

    alu #(
        .WIDTH(32)
    ) u_alu (
        .A(alu_a),
        .B(alu_b),
        .ALUop(alu_op),
        .Result(alu_result)
    );

    function automatic logic [1:0] csr_addr_to_idx(input logic [11:0] addr);
        unique case (addr)
            12'h305: return 2'd0;
            12'h341: return 2'd1;
            12'h300: return 2'd2;
            12'h342: return 2'd3;
            default: return 2'd0;
        endcase
    endfunction

    always_comb begin
        jump_target = 32'h0;
        jump_en     = 1'b0;
        unique case (inst_type)
            TYPE_I: begin
                alu_a = gpr_rdata1;
                alu_b = imm;
                if (opcode == 7'b1100111) begin  // JALR
                    jump_target = {alu_result[31:1], 1'b0};
                    jump_en     = 1'b1;
                end else if (opcode == 7'b1110011 && funct3 == 3'b000) begin  // SYSTEM
                    unique case (imm)
                        32'h0: begin
                            // ECALL
                            jump_target = csr_rdata[0];
                            jump_en     = 1'b1;
                        end
                        32'h302: begin
                            // MRET
                            jump_target = csr_rdata[1];
                            jump_en     = 1'b1;
                        end
                        default: ;
                    endcase
                end
            end
            TYPE_R: begin
                alu_a = gpr_rdata1;
                alu_b = gpr_rdata2;
            end
            TYPE_U: begin
                alu_a = (opcode == 7'b0110111) ? 32'h0 : pc;
                alu_b = imm;
            end
            TYPE_J: begin
                alu_a       = pc;
                alu_b       = imm;
                jump_target = alu_result;
                jump_en     = 1'b1;
            end
            TYPE_B: begin
                alu_a = pc;
                alu_b = imm;
                jump_target = alu_result;
                unique case (funct3)
                    3'b000:  jump_en = (gpr_rdata1 == gpr_rdata2);
                    3'b001:  jump_en = (gpr_rdata1 != gpr_rdata2);
                    3'b100:  jump_en = ($signed(gpr_rdata1) < $signed(gpr_rdata2));
                    3'b101:  jump_en = ($signed(gpr_rdata1) >= $signed(gpr_rdata2));
                    3'b110:  jump_en = (gpr_rdata1 < gpr_rdata2);
                    3'b111:  jump_en = (gpr_rdata1 >= gpr_rdata2);
                    default: jump_en = 1'b0;
                endcase
            end
            TYPE_S: begin
                alu_a = gpr_rdata1;
                alu_b = imm;
            end
            default: begin
                alu_a = gpr_rdata1;
                alu_b = gpr_rdata2;
            end
        endcase
    end


    wire [1:0] csr_idx = csr_addr_to_idx(imm[11:0]);  // CSR寄存器索引
    logic [31:0] mstatus_ecall, mstatus_mret;  // mstatus临时变量
    always_comb begin
        csr_we        = '0;
        csr_wdata     = '0;
        csr_read_data = 32'h0;

        mstatus_ecall = csr_rdata[2];
        mstatus_mret  = csr_rdata[2];
        if (inst_type == TYPE_I && opcode == 7'b1110011) begin
            // CSR指令执行
            unique case (funct3)
                3'b000: begin
                    if (imm == 32'h0) begin
                        // ECALL
                        csr_we               = 4'b1110;
                        csr_wdata[1]         = pc;
                        csr_wdata[3]         = 32'd11;
                        mstatus_ecall[7]     = mstatus_ecall[3];
                        mstatus_ecall[3]     = 1'b0;
                        mstatus_ecall[12:11] = 2'b11;
                        csr_wdata[2]         = mstatus_ecall;
                    end else if (imm == 32'h302) begin
                        // MRET
                        csr_we[2]           = 1'b1;
                        mstatus_mret[3]     = mstatus_mret[7];
                        mstatus_mret[7]     = 1'b1;
                        mstatus_mret[12:11] = 2'b00;
                        csr_wdata[2]        = mstatus_mret;
                    end else if (imm == 32'h1) begin
                        // EBREAK
                        NPCTRAP();
                    end else begin
                        // 不支持的系统指令保持兼容旧行为
                        NPCINV(pc);
                    end
                end
                3'b001: begin
                    // CSRW
                    csr_we[csr_idx]    = 1'b1;
                    csr_wdata[csr_idx] = gpr_rdata1;
                end
                3'b010: begin
                    // CSRR
                    csr_read_data = csr_rdata[csr_idx];
                end
                default: NPCINV(pc);
            endcase
        end
    end

endmodule

module delay_line #(
    parameter int N     = 4,  // 延迟周期数，可为0
    parameter int WIDTH = 8   // 信号位宽
) (
    input logic clk,
    input logic reset,
    input logic [WIDTH-1:0] din,
    output logic [WIDTH-1:0] dout
);

    generate
        if (N == 0) begin : gen_no_delay
            // N=0时直接透传输入
            assign dout = reset ? '0 : din;
        end else begin : gen_with_delay
            logic [N-1:0][WIDTH-1:0] shift_reg;
            always_ff @(posedge clk) begin
                if (reset) shift_reg <= '0;
                else begin
                    shift_reg[0] <= din;
                    for (int i = 1; i < N; i++) begin
                        shift_reg[i] <= shift_reg[i-1];
                    end
                end
            end
            assign dout = shift_reg[N-1];
        end
    endgenerate

endmodule


// LSU(Load Store Unit) 负责根据控制信号控制存储器, 从存储器中读出数据, 或将数据写入存储器
module LSU (
    input                        clk,
    input                        reset,
    input                        lsu_req_valid,
    output logic                 lsu_req_ready,
    input  inst_t                inst_type,
    input                 [ 6:0] opcode,
    input                 [ 2:0] funct3,
    input                 [31:0] pc,
    input                 [31:0] alu_result,
    input                 [31:0] gpr_rdata2,
    output logic                 lsu_resp_valid,
    input                        lsu_resp_ready,
    output logic          [31:0] lsu_rdata,
    output logic                 lsu_error,
    // DMEM接口 - 使用interface
           axi4_if.master        dmem
);
    import "DPI-C" function void NPCINV(input int pc);

    // LSU根据指令决定是否访问DMEM
    logic dmem_ren, dmem_wen;
    assign dmem_ren = (inst_type == TYPE_I && opcode == 7'b0000011);
    assign dmem_wen = (inst_type == TYPE_S && opcode == 7'b0100011);

    // AXI4 Size logic
    logic [2:0] axsize;
    always_comb begin
        case (funct3[1:0])
            2'b00:   axsize = 3'b000;  // Byte
            2'b01:   axsize = 3'b001;  // Half
            2'b10:   axsize = 3'b010;  // Word
            default: axsize = 3'b010;
        endcase
    end

    // AR Channel
    assign dmem.araddr    = alu_result;
    assign dmem.arvalid   = lsu_req_valid && dmem_ren;
    assign dmem.arid      = 4'b0;
    assign dmem.arlen     = 8'b0;  // Burst length = 1
    assign dmem.arsize    = axsize;
    assign dmem.arburst   = 2'b01;  // INCR

    // R Channel
    assign dmem.rready    = lsu_resp_ready;

    // AW Channel
    assign dmem.awaddr    = alu_result;
    assign dmem.awvalid   = lsu_req_valid && dmem_wen;
    assign dmem.awid      = 4'b0;
    assign dmem.awlen     = 8'b0;
    assign dmem.awsize    = axsize;
    assign dmem.awburst   = 2'b01;

    // W Channel
    assign dmem.wdata     = gpr_rdata2;
    assign dmem.wvalid    = lsu_req_valid && dmem_wen;
    assign dmem.wlast     = 1'b1;  // Single beat

    // B Channel
    assign dmem.bready    = lsu_resp_ready;

    // LSU握手逻辑
    assign lsu_req_ready  = 1'b1;
    assign lsu_resp_valid = dmem_ren ? dmem.rvalid : (dmem_wen ? dmem.bvalid : lsu_req_valid);

    // 写掩码生成
    always_comb begin
        unique case (funct3)
            3'b000: dmem.wstrb = 4'h1;  // SB
            3'b001: dmem.wstrb = 4'h3;  // SH
            3'b010: dmem.wstrb = 4'hF;  // SW
            default: begin
                dmem.wstrb = 4'h0;
                if (dmem_wen) NPCINV(pc);
            end
        endcase
    end

    // 读数据扩展
    always_comb begin
        unique case (funct3)
            3'b000: lsu_rdata = {{24{dmem.rdata[7]}}, dmem.rdata[7:0]};  // LB
            3'b010: lsu_rdata = dmem.rdata;  // LW
            3'b001: lsu_rdata = {{16{dmem.rdata[15]}}, dmem.rdata[15:0]};  // LH
            3'b101: lsu_rdata = {16'b0, dmem.rdata[15:0]};  // LHU
            3'b100: lsu_rdata = {24'b0, dmem.rdata[7:0]};  // LBU
            default: begin
                lsu_rdata = 32'h0;
                if (dmem_ren) NPCINV(pc);
            end
        endcase
    end

    assign lsu_error = |dmem.rresp | |dmem.bresp;  // 修改：检查2位resp是否非零

endmodule

// WBU(WriteBack Unit): 将数据写入寄存器
module WBU (
    input  inst_t        inst_type,
    input         [ 6:0] opcode,
    input         [ 2:0] funct3,
    input         [31:0] pc,
    input         [31:0] snpc,
    input         [31:0] alu_result,
    input         [31:0] lsu_rdata,
    input         [31:0] csr_read_data,
    output logic  [31:0] gpr_wdata,
    output logic         gpr_we
);
    import "DPI-C" function void NPCINV(input int pc);

    always_comb begin
        gpr_wdata = 32'h0;
        gpr_we = 1'b0;

        unique case (inst_type)
            TYPE_I: begin
                if (opcode == 7'b1100111) begin
                    // JALR
                    gpr_wdata = snpc;
                    gpr_we = 1'b1;
                end else if (opcode == 7'b0000011) begin
                    // Load指令: 写回内存数据
                    gpr_wdata = lsu_rdata;
                    gpr_we = 1'b1;
                end else if (opcode == 7'b1110011) begin
                    // CSR指令: 写回CSR读值
                    if (funct3 == 3'b010) begin
                        gpr_wdata = csr_read_data;
                        gpr_we = 1'b1;
                    end
                end else begin
                    // 其他I型指令(算术/逻辑): 写回ALU结果
                    unique case (funct3)
                        3'b000, 3'b001, 3'b010, 3'b011, 3'b100, 3'b101, 3'b110, 3'b111: begin
                            gpr_wdata = alu_result;
                            gpr_we = 1'b1;
                        end
                        default: NPCINV(pc);
                    endcase
                end
            end
            TYPE_R: begin
                unique case (funct3)
                    3'b000, 3'b001, 3'b010, 3'b011, 3'b100, 3'b101, 3'b110, 3'b111: begin
                        gpr_wdata = alu_result;
                        gpr_we = 1'b1;
                    end
                    default: NPCINV(pc);
                endcase
            end
            TYPE_U: begin
                gpr_wdata = alu_result;
                gpr_we = 1'b1;
            end
            TYPE_J: begin
                // JAL
                gpr_wdata = snpc;
                gpr_we = 1'b1;
            end
            default: ;  // TYPE_B / TYPE_S / TYPE_N 不写回寄存器
        endcase
    end
endmodule

