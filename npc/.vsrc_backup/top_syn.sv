// 此代码仅为yosys可综合使用

// 为了yosys可综合，请作出如下修改：
// 1. IFU例化一个256x32的ROM作为数据存储器(初始化一些指令)
// 2. LSU例化一个256x32的packed array触发器作为数据存储器
// 3. 删除所有DPI-C接口
// 4.增加一些输出端口以免顶层模块无输出被优化掉


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
    // input [31:0] inst,  // 移除外部inst输入端口
    output logic [31:0] pc_out,  // 新增输出端口：程序计数器
    output logic [31:0] alu_result_out,  // 新增输出端口：ALU结果
    output logic [31:0] load_data_out  // 新增输出端口：加载数据
);

    // IFU：负责 PC 和取指
    logic [31:0] pc, snpc, jump_target;  // pc renamed to ifu_raddr, snpc, 跳转目标地址
    logic        jump_en;
    logic [31:0] dnpc;  // 新增dnpc信号，从PCU输出
    logic        ifu_resp_valid;  // 新增ifu_resp_valid信号，表示指令响应有效
    logic [31:0] ifu_rdata;  // 新增ifu_rdata信号

    IFU u_ifu (
        .clk(clk),
        .reset(reset),
        .jump_target(jump_target),
        .jump_en(jump_en),
        .pc(pc),
        .snpc(snpc),
        .dnpc(dnpc),
        .ifu_rdata(ifu_rdata),  // 新增输出连接
        .ifu_resp_valid(ifu_resp_valid)  // 新增输出
    );

    // IDU：负责指令解码
    // 解码信号
    logic [ 6:0] opcode;
    logic [ 2:0] funct3;
    logic [31:0] imm;
    logic [4:0] rs1, rs2, rd;
    inst_t   inst_type;
    alu_op_t alu_op;  // 新增alu_op信号
    IDU u_idu (
        .inst(ifu_rdata),  // 修改为使用ifu_rdata
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
        .reset(reset),
        .gpr_we(gpr_we && (|rd)),
        .gpr_waddr(rd),
        .gpr_wdata(gpr_wdata),
        .gpr_raddr1(rs1),
        .gpr_raddr2(rs2),
        .gpr_rdata1(gpr_rdata1),
        .gpr_rdata2(gpr_rdata2),
        .gpr_req_valid(ifu_resp_valid)  // 新增输入
    );

    // CSR：控制状态寄存器
    logic [3:0][31:0] csr_wdata, csr_rdata;  // CSR写数据，读数据
    logic [3:0] csr_we;
    CSR u_csr (
        .clk(clk),
        .reset(reset),
        .csr_we(csr_we),
        .csr_wdata(csr_wdata),
        .csr_rdata(csr_rdata)
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
    logic [31:0] load_data;  // 加载数据
    LSU u_lsu (
        .clk          (clk),
        .reset        (reset),
        .inst_type    (inst_type),
        .opcode       (opcode),
        .funct3       (funct3),
        .pc           (pc),
        .alu_result   (alu_result),
        .gpr_rdata2   (gpr_rdata2),
        .load_data    (load_data),
        .lsu_req_valid(ifu_resp_valid)  // 新增输入
    );

    // WBU：负责写回GPR
    WBU u_wbu (
        .inst_type    (inst_type),
        .opcode       (opcode),
        .funct3       (funct3),
        .pc           (pc),
        .snpc         (snpc),
        .alu_result   (alu_result),
        .load_data    (load_data),
        .csr_read_data(csr_read_data),
        .gpr_wdata    (gpr_wdata),
        .gpr_we       (gpr_we)
    );

    // 新增输出连接
    assign pc_out = pc;
    assign alu_result_out = alu_result;
    assign load_data_out = load_data;

endmodule

// IFU(Instruction Fetch Unit) 负责根据当前PC从存储器中取出一条指令，并管理PC更新
module IFU (
    input               clk,
    input               reset,
    input        [31:0] jump_target,
    input               jump_en,
    // input        [31:0] inst,           // 移除外部inst输入端口
    output logic [31:0] pc,
    output logic [31:0] snpc,
    output logic [31:0] dnpc,
    output logic [31:0] ifu_rdata,      // 输出指令
    output logic        ifu_resp_valid  // 新增输出，表示指令响应有效
);
    // 例化256x32的ROM作为inst来源
    logic [31:0] rom[256];  // ROM存储器

    // ✅ 在 initial 块中直接写内容
    initial begin
        rom[0] = 32'h00000093;  // addi x1, x0, 0
        rom[1] = 32'h00100113;  // addi x2, x0, 1
        rom[2] = 32'h002081B3;  // add  x3, x1, x2
        rom[3] = 32'h00310233;  // add  x4, x2, x3
        rom[4] = 32'h0000006F;  // jal  x0, 0
        // 其余可设为 0
        integer i;
        for (i = 5; i < 256; i = i + 1) rom[i] = 32'h00000000;
    end

    localparam int RESET_PC = 32'h80000000;
    logic reset_sync;

    // 同步复位信号
    always_ff @(posedge clk) begin
        if (reset) reset_sync <= 1'b1;
        else reset_sync <= 1'b0;
    end
    // assign reset_sync = reset;

    // PC 寄存器更新
    always_ff @(posedge clk) begin
        if (reset_sync) pc <= RESET_PC;
        else pc <= dnpc;
    end

    // snpc / dnpc 选择逻辑
    assign snpc = pc + 4;
    assign dnpc = jump_en ? jump_target : snpc;

    // 从ROM读取指令
    assign ifu_rdata = rom[pc[9:2]];
    assign ifu_resp_valid = 1'b1;  // 假设当前所有指令响应有效
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
    output alu_op_t alu_op  // 新增输出ALU操作码
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

    // 新增alu_op解析（从EXU移来）
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
    input               clk,            // 时钟信号
    input               reset,          // 复位信号
    input               gpr_we,         // 写使能信号
    input        [ 4:0] gpr_waddr,      // 写寄存器地址
    input        [31:0] gpr_wdata,      // 写数据
    input        [ 4:0] gpr_raddr1,     // 读寄存器1地址
    input        [ 4:0] gpr_raddr2,     // 读寄存器2地址
    input               gpr_req_valid,  // 新增输入，指令响应有效性
    output logic [31:0] gpr_rdata1,     // 读寄存器1数据
    output logic [31:0] gpr_rdata2      // 读寄存器2数据
);
    logic [31:0] regfile[32];  // 寄存器文件

    // import "DPI-C" function void output_gprs(input [31:0] gprs[]);
    // always_comb output_gprs(regfile);  // 输出寄存器状态到DPI-C

    always_ff @(posedge clk) begin
        if (reset) begin
            for (int i = 0; i < 32; i++) regfile[i] <= 32'h0;  // 复位时清零所有寄存器
        end else if (gpr_we && gpr_req_valid) begin  // 修改：添加valid条件
            regfile[gpr_waddr] <= gpr_wdata;
        end
        // write_gpr_npc(waddr, wdata);  // 更新DPI-C接口寄存器
    end


    // 移除DPI接口

    always_comb begin
        gpr_rdata1 = (gpr_raddr1 == 5'b0) ? 32'h0 : regfile[gpr_raddr1];
        gpr_rdata2 = (gpr_raddr2 == 5'b0) ? 32'h0 : regfile[gpr_raddr2];
    end
endmodule

// CSR(Control and Status Register) 负责控制和状态寄存器的读写
module CSR #(
    localparam int N = 4  // CSR寄存器数量
) (
    input clk,  // 时钟信号
    input reset,  // 复位信号
    input [N-1:0] csr_we,
    input [N-1:0][31:0] csr_wdata,
    output logic [N-1:0][31:0] csr_rdata
);
    always_ff @(posedge clk) begin
        if (reset) csr_rdata <= '0;  // 复位时清零所有CSR寄存器
        else begin
            for (int i = 0; i < N; i++) if (csr_we[i]) csr_rdata[i] <= csr_wdata[i];
        end
    end

    // 移除DPI接口

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
    // 移除DPI接口

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
            12'h305: csr_addr_to_idx = 2'd0;
            12'h341: csr_addr_to_idx = 2'd1;
            12'h300: csr_addr_to_idx = 2'd2;
            12'h342: csr_addr_to_idx = 2'd3;
            default: csr_addr_to_idx = 2'd0;
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
                        // 移除NPCTRAP调用
                    end else begin
                        // 不支持的系统指令保持兼容旧行为
                        // 移除NPCINV调用
                    end
                end
                3'b001: begin
                    // CSRRW
                    csr_we[csr_idx]    = 1'b1;
                    csr_wdata[csr_idx] = gpr_rdata1;
                end
                3'b010: begin
                    // CSRRR
                    csr_read_data = csr_rdata[csr_idx];
                end
                default: ;  // 移除NPCINV调用
            endcase
        end
    end

endmodule

// LSU(Load Store Unit) 负责根据控制信号控制存储器, 从存储器中读出数据, 或将数据写入存储器
module LSU (
    input                clk,
    input                reset,
    input  inst_t        inst_type,
    input         [ 6:0] opcode,
    input         [ 2:0] funct3,
    input         [31:0] pc,
    input         [31:0] alu_result,
    input         [31:0] gpr_rdata2,
    input                lsu_req_valid,  // 新增输入，指令响应有效性
    output logic  [31:0] load_data
);
    // 移除DPI接口

    // 例化256x32的packed array触发器作为数据存储器
    logic [255:0][31:0] pmem;

    // 初始化存储器（复位时清零）
    always_ff @(posedge clk) begin
        if (reset) begin
            for (int i = 0; i < 256; i++) pmem[i] <= 32'h0;
        end else if (inst_type == TYPE_S && opcode == 7'b0100011 && lsu_req_valid) begin
            // 存储逻辑：写入mem（假设地址对齐，alu_result[9:2]作为索引）
            unique case (funct3)
                3'b000:  pmem[alu_result[9:2]][7:0] <= gpr_rdata2[7:0];  // SB
                3'b001:  pmem[alu_result[9:2]][15:0] <= gpr_rdata2[15:0];  // SH
                3'b010:  pmem[alu_result[9:2]] <= gpr_rdata2;  // SW
                default: ;  // 不支持的 store 类型保持兼容旧行为
            endcase
        end
    end

    int mem_rdata_raw;

    // 加载逻辑
    always_comb begin
        load_data     = 32'h0;
        mem_rdata_raw = 0;
        if (inst_type == TYPE_I && opcode == 7'b0000011) begin
            mem_rdata_raw = pmem[alu_result[9:2]];  // 修复：从mem改为pmem
            unique case (funct3)
                3'b000: load_data = {{24{mem_rdata_raw[7]}}, mem_rdata_raw[7:0]};  // LB
                3'b010: load_data = mem_rdata_raw;  // LW
                3'b001: load_data = {{16{mem_rdata_raw[15]}}, mem_rdata_raw[15:0]};  // LH
                3'b101: load_data = {16'b0, mem_rdata_raw[15:0]};  // LHU
                3'b100: load_data = {24'b0, mem_rdata_raw[7:0]};  // LBU
                default: begin
                    load_data = 32'h0;
                    // 移除NPCINV调用
                end
            endcase
        end
    end

endmodule

// WBU(WriteBack Unit): 将数据写入寄存器
module WBU (
    input  inst_t        inst_type,
    input         [ 6:0] opcode,
    input         [ 2:0] funct3,
    input         [31:0] pc,
    input         [31:0] snpc,
    input         [31:0] alu_result,
    input         [31:0] load_data,
    input         [31:0] csr_read_data,
    output logic  [31:0] gpr_wdata,
    output logic         gpr_we
);
    // 移除DPI接口

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
                    gpr_wdata = load_data;
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
                        default: ;  // 移除NPCINV调用
                    endcase
                end
            end
            TYPE_R: begin
                unique case (funct3)
                    3'b000, 3'b001, 3'b010, 3'b011, 3'b100, 3'b101, 3'b110, 3'b111: begin
                        gpr_wdata = alu_result;
                        gpr_we = 1'b1;
                    end
                    default: ;  // 移除NPCINV调用
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

