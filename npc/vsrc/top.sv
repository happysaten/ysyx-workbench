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

module top (
    input clk,  // 时钟信号
    input reset,  // 复位信号
    output logic npc_req_ready,
    output logic npc_resp_valid,
    output logic npc_error
);

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
    // assign reset_sync = reset;

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

    logic ifu_error, gpr_error, csr_error, lsu_error;
    assign npc_error = ifu_error | gpr_error | csr_error | lsu_error;

    // IMEM接口信号
    logic imem_arvalid, imem_arready, imem_rvalid, imem_rready;
    logic imem_awvalid, imem_awready, imem_wvalid, imem_wready, imem_bvalid, imem_bready;
    logic [31:0] imem_araddr, imem_awaddr, imem_wdata, imem_rdata;
    logic [7:0] imem_wmask;
    logic imem_rresp, imem_bresp;

    // DMEM接口信号
    logic dmem_arvalid, dmem_arready, dmem_rvalid, dmem_rready;
    logic dmem_awvalid, dmem_awready, dmem_wvalid, dmem_wready, dmem_bvalid, dmem_bready;
    logic [31:0] dmem_araddr, dmem_awaddr, dmem_wdata, dmem_rdata;
    logic [7:0] dmem_wmask;
    logic dmem_rresp, dmem_bresp;

    // 实例化IMEM模块
    IMEM u_imem (
        .clk         (clk),
        .reset       (reset_sync),
        .imem_arvalid(imem_arvalid),
        .imem_arready(imem_arready),
        .imem_araddr (imem_araddr),
        .imem_rvalid (imem_rvalid),
        .imem_rready (imem_rready),
        .imem_rdata  (imem_rdata),
        .imem_rresp  (imem_rresp),
        .imem_awvalid(imem_awvalid),
        .imem_awready(imem_awready),
        .imem_awaddr (imem_awaddr),
        .imem_wvalid (imem_wvalid),
        .imem_wready (imem_wready),
        .imem_wdata  (imem_wdata),
        .imem_wmask  (imem_wmask),
        .imem_bvalid (imem_bvalid),
        .imem_bready (imem_bready),
        .imem_bresp  (imem_bresp)
    );

    // 实例化DMEM模块
    DMEM u_dmem (
        .clk         (clk),
        .reset       (reset_sync),
        .dmem_arvalid(dmem_arvalid),
        .dmem_arready(dmem_arready),
        .dmem_araddr (dmem_araddr),
        .dmem_rvalid (dmem_rvalid),
        .dmem_rready (dmem_rready),
        .dmem_rdata  (dmem_rdata),
        .dmem_rresp  (dmem_rresp),
        .dmem_awvalid(dmem_awvalid),
        .dmem_awready(dmem_awready),
        .dmem_awaddr (dmem_awaddr),
        .dmem_wvalid (dmem_wvalid),
        .dmem_wready (dmem_wready),
        .dmem_wdata  (dmem_wdata),
        .dmem_wmask  (dmem_wmask),
        .dmem_bvalid (dmem_bvalid),
        .dmem_bready (dmem_bready),
        .dmem_bresp  (dmem_bresp)
    );

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
    // assign reset_sync = reset;

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

    logic ifu_error, gpr_error, csr_error, lsu_error;
    assign npc_error = ifu_error | gpr_error | csr_error | lsu_error;

    IFU u_ifu (
        .clk(clk),
        .reset(reset_sync),
        .ifu_req_valid(npc_req_valid),
        .ifu_req_ready(ifu_req_ready),
        .jump_en(jump_en),
        .jump_target(jump_target),
        .ifu_rdata(inst),
        .ifu_resp_valid(ifu_resp_valid),
        .ifu_resp_ready(lsu_req_ready),
        .pc(pc),
        .snpc(snpc),
        .dnpc(dnpc),
        .ifu_error(ifu_error),
        // IMEM接口
        .imem_arvalid(imem_arvalid),
        .imem_arready(imem_arready),
        .imem_araddr (imem_araddr),
        .imem_rvalid (imem_rvalid),
        .imem_rready (imem_rready),
        .imem_rdata  (imem_rdata),
        .imem_rresp  (imem_rresp),
        .imem_awvalid(imem_awvalid),
        .imem_awaddr (imem_awaddr),
        .imem_wvalid (imem_wvalid),
        .imem_wdata  (imem_wdata),
        .imem_wmask  (imem_wmask),
        .imem_bready (imem_bready)
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
        .clk(clk),
        .reset(reset_sync),
        .lsu_req_valid(ifu_resp_valid),
        .lsu_req_ready(lsu_req_ready),
        .inst_type(inst_type),
        .opcode(opcode),
        .funct3(funct3),
        .pc(pc),
        .alu_result(alu_result),
        .gpr_rdata2(gpr_rdata2),
        .lsu_resp_valid(lsu_resp_valid),
        .lsu_resp_ready(gpr_req_ready && csr_req_ready),
        .lsu_rdata(lsu_rdata),
        .lsu_error(lsu_error),
        // DMEM接口
        .dmem_arvalid(dmem_arvalid),
        .dmem_arready(dmem_arready),
        .dmem_araddr (dmem_araddr),
        .dmem_rvalid (dmem_rvalid),
        .dmem_rready (dmem_rready),
        .dmem_rdata  (dmem_rdata),
        .dmem_rresp  (dmem_rresp),
        .dmem_awvalid(dmem_awvalid),
        .dmem_awready(dmem_awready),
        .dmem_awaddr (dmem_awaddr),
        .dmem_wvalid (dmem_wvalid),
        .dmem_wready (dmem_wready),
        .dmem_wdata  (dmem_wdata),
        .dmem_wmask  (dmem_wmask),
        .dmem_bvalid (dmem_bvalid),
        .dmem_bready (dmem_bready),
        .dmem_bresp  (dmem_bresp)
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
    input               clk,
    input               reset,
    input               ifu_req_valid,
    output logic        ifu_req_ready,
    input               jump_en,
    input        [31:0] jump_target,
    output logic [31:0] ifu_rdata,
    output logic        ifu_resp_valid,
    input               ifu_resp_ready,
    output logic [31:0] pc,
    output logic [31:0] snpc,
    output logic [31:0] dnpc,
    output logic        ifu_error,
    // IMEM接口
    output logic        imem_arvalid,
    input               imem_arready,
    output logic [31:0] imem_araddr,
    input               imem_rvalid,
    output logic        imem_rready,
    input        [31:0] imem_rdata,
    input               imem_rresp,
    output logic        imem_awvalid,
    output logic [31:0] imem_awaddr,
    output logic        imem_wvalid,
    output logic [31:0] imem_wdata,
    output logic [ 7:0] imem_wmask,
    output logic        imem_bready
);

    // snpc / dnpc 选择逻辑
    assign snpc = pc + 4;
    assign dnpc = jump_en ? jump_target : snpc;

    // IMEM访问控制 - 只使用读通道
    assign imem_araddr = dnpc;
    assign imem_arvalid = ifu_req_valid;
    assign imem_rready = ifu_resp_ready;
    
    // 写通道全部置为无效
    assign imem_awvalid = 1'b0;
    assign imem_awaddr = 32'h0;
    assign imem_wvalid = 1'b0;
    assign imem_wdata = 32'h0;
    assign imem_wmask = 8'h0;
    assign imem_bready = 1'b0;

    // IFU握手逻辑
    assign ifu_req_ready = imem_arready;
    assign ifu_resp_valid = imem_rvalid;
    assign ifu_rdata = imem_rdata;
    assign ifu_error = imem_rresp;

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

// IMEM(Instruction Memory) 负责指令内存的读访问
module IMEM (
    input               clk,
    input               reset,
    // 读地址通道(AR)
    input               imem_arvalid,
    output logic        imem_arready,
    input        [31:0] imem_araddr,
    // 读数据通道(R)
    output logic        imem_rvalid,
    input               imem_rready,
    output logic [31:0] imem_rdata,
    output logic        imem_rresp,
    // 写地址通道(AW)
    input               imem_awvalid,
    output logic        imem_awready,
    input        [31:0] imem_awaddr,
    // 写数据通道(W)
    input               imem_wvalid,
    output logic        imem_wready,
    input        [31:0] imem_wdata,
    input        [ 7:0] imem_wmask,
    // 写回复通道(B)
    output logic        imem_bvalid,
    input               imem_bready,
    output logic        imem_bresp
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
            IDLE: begin
                if (req_fire) next_state = resp_data_ready ? RESP : WAIT;
                else next_state = IDLE;
            end
            WAIT:    next_state = resp_data_ready ? RESP : WAIT;
            RESP:    next_state = resp_fire ? IDLE : RESP;
            default: next_state = IDLE;
        endcase
    end

    logic random_bit0, random_bit1;
    lfsr8 #(
        .TAPS(8'b10101010)
    ) u_imem_req_lfsr (
        .clk  (clk),
        .reset(reset),
        .en   (1'b1),
        .out  (random_bit0)
    );
    lfsr8 #(
        .TAPS(8'b10111010)
    ) u_imem_resp_lfsr (
        .clk  (clk),
        .reset(reset),
        .en   (1'b1),
        .out  (random_bit1)
    );

    assign imem_rvalid = (state == RESP);
    assign imem_arready = (state == IDLE) && random_bit0;
    assign resp_data_ready = random_bit1;
    assign req_fire = imem_arvalid && imem_arready;
    assign resp_fire = imem_rvalid && imem_rready;

    // 写通道保持无效
    assign imem_awready = 1'b0;
    assign imem_wready = 1'b0;
    assign imem_bvalid = 1'b0;
    assign imem_bresp = 1'b0;

    import "DPI-C" function int pmem_read_npc(input int raddr);

    always @(posedge clk) begin
        if (req_fire) imem_rdata <= pmem_read_npc(imem_araddr);
    end

    assign imem_rresp = 0;

    // 断言：确保写通道信号始终为0
    always @(posedge clk) begin
        assert (imem_awvalid == 1'b0) else $fatal("IMEM: imem_awvalid should always be 0");
        assert (imem_wvalid == 1'b0) else $fatal("IMEM: imem_wvalid should always be 0");
        assert (imem_bready == 1'b0) else $fatal("IMEM: imem_bready should always be 0");
    end

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
        end
        // write_gpr_npc(waddr, wdata);  // 更新DPI-C接口寄存器
    end


    import "DPI-C" function void write_gpr_npc(
        input logic [ 4:0] idx,
        input logic [31:0] data
    );
    always_comb begin
        if (gpr_wen && state != RESP && next_state == RESP) write_gpr_npc(gpr_waddr, gpr_wdata);
    end

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


module lfsr8 #(
    parameter logic [7:0] TAPS = 8'b10111000 // 默认抽头：位7,5,4,3，对应x^8 + x^6 + x^5 + x^4 + 1
) (
    input clk,
    input reset,
    input en,
    output logic out
);

    logic [7:0] lfsr;
    logic feedback;
    assign feedback = ^(lfsr & TAPS);  // 参数化反馈计算

    // 8-bit maximal-length LFSR polynomial: x^8 + x^6 + x^5 + x^4 + 1
    always_ff @(posedge clk) begin
        if (reset) lfsr <= 8'h1;  // 初始值不能为0
        else if (en) lfsr <= {lfsr[6:0], feedback};
    end

    assign out = (lfsr[1:0] == 2'b10);

endmodule


// DMEM(Data Memory) 负责数据内存的读写访问
module DMEM (
    input               clk,
    input               reset,
    // 读地址通道(AR)
    input               dmem_arvalid,
    output logic        dmem_arready,
    input        [31:0] dmem_araddr,
    // 读数据通道(R)
    output logic        dmem_rvalid,
    input               dmem_rready,
    output logic [31:0] dmem_rdata,
    output logic        dmem_rresp,
    // 写地址通道(AW)
    input               dmem_awvalid,
    output logic        dmem_awready,
    input        [31:0] dmem_awaddr,
    // 写数据通道(W)
    input               dmem_wvalid,
    output logic        dmem_wready,
    input        [31:0] dmem_wdata,
    input        [ 7:0] dmem_wmask,
    // 写回复通道(B)
    output logic        dmem_bvalid,
    input               dmem_bready,
    output logic        dmem_bresp
);
    typedef enum logic [2:0] {
        IDLE,
        RWAIT,  // 读等待
        RRESP,  // 读响应
        WWAIT,  // 写等待
        WRESP   // 写响应
    } state_t;
    state_t state, next_state;

    always @(posedge clk) begin
        if (reset) state <= IDLE;
        else state <= next_state;
    end

    logic resp_data_ready;

    always_comb begin
        unique case (state)
            IDLE: begin
                if (dmem_awvalid && dmem_awready && dmem_wvalid && dmem_wready)
                    next_state = resp_data_ready ? WRESP : WWAIT;
                else if (dmem_arvalid && dmem_arready) next_state = resp_data_ready ? RRESP : RWAIT;
                else next_state = IDLE;
            end
            RWAIT:   next_state = resp_data_ready ? RRESP : RWAIT;
            RRESP:   next_state = (dmem_rvalid && dmem_rready) ? IDLE : RRESP;
            WWAIT:   next_state = resp_data_ready ? WRESP : WWAIT;
            WRESP:   next_state = (dmem_bvalid && dmem_bready) ? IDLE : WRESP;
            default: next_state = IDLE;
        endcase
    end

    logic random_bit;
    lfsr8 #(
        .TAPS(8'b01010110)
    ) u_dmem_resp_lfsr (
        .clk  (clk),
        .reset(reset),
        .en   (1'b1),
        .out  (random_bit)
    );

    // 读写请求仲裁（写优先）
    assign dmem_rvalid = (state == RRESP);
    assign dmem_bvalid = (state == WRESP);
    assign dmem_arready = (state == IDLE) && !(dmem_awvalid && dmem_wvalid);
    assign dmem_awready = (state == IDLE);
    assign dmem_wready = (state == IDLE);
    assign resp_data_ready = random_bit;

    import "DPI-C" function int pmem_read_npc(input int raddr);
    import "DPI-C" function void pmem_write_npc(
        input int  waddr,
        input int  wdata,
        input byte wmask
    );

    always @(posedge clk) begin
        if ((state == RWAIT && next_state == RRESP) || (state == IDLE && next_state == RRESP)) begin
            dmem_rdata <= pmem_read_npc(dmem_araddr);
        end
        if ((state == WWAIT && next_state == WRESP) || (state == IDLE && next_state == WRESP)) begin
            pmem_write_npc(dmem_awaddr, dmem_wdata, dmem_wmask);
        end
    end

    assign dmem_rresp = 0;
    assign dmem_bresp = 0;

endmodule

// LSU(Load Store Unit) 负责根据控制信号控制存储器, 从存储器中读出数据, 或将数据写入存储器
module LSU (
    input                clk,
    input                reset,
    input                lsu_req_valid,
    output logic         lsu_req_ready,
    input  inst_t        inst_type,
    input         [ 6:0] opcode,
    input         [ 2:0] funct3,
    input         [31:0] pc,
    input         [31:0] alu_result,
    input         [31:0] gpr_rdata2,
    output logic         lsu_resp_valid,
    input                lsu_resp_ready,
    output logic  [31:0] lsu_rdata,
    output logic         lsu_error,
    // DMEM接口
    output logic         dmem_arvalid,
    input                dmem_arready,
    output logic  [31:0] dmem_araddr,
    input                dmem_rvalid,
    output logic         dmem_rready,
    input         [31:0] dmem_rdata,
    input                dmem_rresp,
    output logic         dmem_awvalid,
    input                dmem_awready,
    output logic  [31:0] dmem_awaddr,
    output logic         dmem_wvalid,
    input                dmem_wready,
    output logic  [31:0] dmem_wdata,
    output logic  [ 7:0] dmem_wmask,
    input                dmem_bvalid,
    output logic         dmem_bready,
    input                dmem_bresp
);
    import "DPI-C" function void NPCINV(input int pc);

    // LSU根据指令决定是否访问DMEM
    logic dmem_ren, dmem_wen;
    assign dmem_ren = (inst_type == TYPE_I && opcode == 7'b0000011);
    assign dmem_wen = (inst_type == TYPE_S && opcode == 7'b0100011);

    assign dmem_araddr = alu_result;
    assign dmem_awaddr = alu_result;
    assign dmem_wdata = gpr_rdata2;
    assign dmem_arvalid = lsu_req_valid && dmem_ren;
    assign dmem_awvalid = lsu_req_valid && dmem_wen;
    assign dmem_wvalid = lsu_req_valid && dmem_wen;
    assign dmem_rready = lsu_resp_ready;
    assign dmem_bready = lsu_resp_ready;

    // LSU握手逻辑
    assign lsu_req_ready = dmem_ren ? dmem_arready : (dmem_wen ? (dmem_awready && dmem_wready) : 1'b1);
    assign lsu_resp_valid = dmem_ren ? dmem_rvalid : (dmem_wen ? dmem_bvalid : lsu_req_valid);

    // 写掩码生成
    always_comb begin
        unique case (funct3)
            3'b000: dmem_wmask = 8'h1;  // SB
            3'b001: dmem_wmask = 8'h3;  // SH
            3'b010: dmem_wmask = 8'hF;  // SW
            default: begin
                dmem_wmask = 8'h0;
                if (dmem_wen) NPCINV(pc);
            end
        endcase
    end

    // 读数据扩展
    always_comb begin
        unique case (funct3)
            3'b000: lsu_rdata = {{24{dmem_rdata[7]}}, dmem_rdata[7:0]};  // LB
            3'b010: lsu_rdata = dmem_rdata;  // LW
            3'b001: lsu_rdata = {{16{dmem_rdata[15]}}, dmem_rdata[15:0]};  // LH
            3'b101: lsu_rdata = {16'b0, dmem_rdata[15:0]};  // LHU
            3'b100: lsu_rdata = {24'b0, dmem_rdata[7:0]};  // LBU
            default: begin
                lsu_rdata = 32'h0;
                if (dmem_ren) NPCINV(pc);
            end
        endcase
    end

    assign lsu_error = dmem_rresp | dmem_bresp;

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

