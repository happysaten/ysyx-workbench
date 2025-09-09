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

// ALU操作码枚举
typedef enum logic [3:0] {
    ALU_ADD  = 4'd0,   // 加法，值: 0
    ALU_SUB  = 4'd1,   // 减法，值: 1
    ALU_NOT  = 4'd2,   // 逻辑非，值: 2
    ALU_AND  = 4'd3,   // 逻辑与，值: 3
    ALU_OR   = 4'd4,   // 逻辑或，值: 4
    ALU_XOR  = 4'd5,   // 逻辑异或，值: 5
    ALU_EQ   = 4'd6,   // 等于比较，值: 6
    ALU_SLT  = 4'd7,   // 小于比较，值: 7
    ALU_SLTU = 4'd8,   // 无符号小于比较，值: 8
    ALU_SLL  = 4'd9,   // 左移，值: 9
    ALU_SRL  = 4'd10,  // 逻辑右移，值: 10
    ALU_SRA  = 4'd11   // 算术右移，值: 11
} alu_op_t;

/* verilator lint_off DECLFILENAME */

module top (
    input clk,   // 时钟信号
    input reset  // 复位信号
    // input [31:0] inst,  // 输入指令
    // output logic [31:0] pc  // 程序计数器输出
);
    logic reset_sync;  // 复位信号
    // assign reset_sync = reset;
    always @(posedge clk) begin
        if (reset) reset_sync <= 1'b1;
        else reset_sync <= 1'b0;
    end


    // 寄存器堆信号
    logic we;
    logic [4:0] rs1, rs2, rd;
    logic [31:0] rdata1, rdata2, wdata;

    // 解码信号
    logic [6:0] opcode, funct7;
    logic [2:0] funct3;
    logic [31:0] imm;

    // 指令类型信号
    inst_t inst_type;

    // snpc: 顺序下一个pc，dnpc: 实际下一个pc
    localparam int RESET_PC = 32'h80000000;  // 复位时PC的初始值
    logic [31:0] pc;  // 程序计数器
    logic [31:0] snpc, dnpc;

    // 总是读取地址为`raddr & ~0x3u`的4字节返回
    import "DPI-C" function int pmem_read_npc(input int raddr);
    // 更新DPI-C接口的指令和下一个PC
    import "DPI-C" function void update_inst_npc(
        input int inst,
        input int dnpc
    );
    logic [31:0] inst;
    always_comb inst = pmem_read_npc(pc);  // 读取当前PC地址的指令
    always_comb update_inst_npc(inst, dnpc);

    always @(posedge clk) begin
        if (reset_sync) pc <= RESET_PC;  // 复位时PC初始化为0x80000000
        else pc <= dnpc;
    end


    // 指令解码实例
    inst_decode u_decode (
        .inst(inst),
        .opcode(opcode),
        .funct3(funct3),
        .funct7(funct7),
        .rd(rd),
        .rs1(rs1),
        .rs2(rs2),
        .imm(imm),
        .inst_type(inst_type)  // 输出指令类型
    );

    // CSR 相关信号
    logic [3:0] csr_we;
    logic [3:0][31:0] csr_wdata, csr_rdata;

    // CSR 模块实例
    csr u_csr (
        .clk (clk),
        .we  (csr_we),
        .din (csr_wdata),
        .dout(csr_rdata)
    );

    // 执行单元实例
    logic [31:0] jump_target;  // 新增：跳转目标
    logic        jump_en;  // 新增：跳转使能

    exe u_exe (
        .opcode     (opcode),
        .funct3     (funct3),
        .funct7     (funct7),
        .src1       (rdata1),
        .src2       (rdata2),
        .imm        (imm),
        .pc         (pc),
        .snpc       (snpc),
        .inst_type  (inst_type),
        .dest       (wdata),
        .jump_target(jump_target),
        .jump_en    (jump_en),
        .csr_we     (csr_we),
        .csr_wdata  (csr_wdata),
        .csr_rdata  (csr_rdata)
    );

    assign we = (|rd) && (inst_type == TYPE_I || inst_type == TYPE_R || inst_type == TYPE_U || inst_type == TYPE_J);  // 写使能信号，waddr不为0且是I/R/U/J型指令时使能写

    // 寄存器堆实例
    gpr u_gpr (
        .clk(clk),
        .we(we),
        .waddr(rd),
        .wdata(wdata),
        .raddr1(rs1),
        .raddr2(rs2),
        .rdata1(rdata1),
        .rdata2(rdata2)
    );


    // PC选择逻辑
    always_comb begin
        snpc = pc + 4;
        if (jump_en) dnpc = jump_target;
        else dnpc = snpc;
    end

endmodule

module gpr (
    input clk,  // 时钟信号
    input we,  // 写使能信号
    input [4:0] waddr,  // 写寄存器地址
    input [31:0] wdata,  // 写数据
    input [4:0] raddr1,  // 读寄存器1地址
    input [4:0] raddr2,  // 读寄存器2地址
    output logic [31:0] rdata1,  // 读寄存器1数据
    output logic [31:0] rdata2  // 读寄存器2数据
);
    logic [31:0] regfile[32];  // 寄存器文件

    // import "DPI-C" function void output_gprs(input [31:0] gprs[]);
    // always_comb output_gprs(regfile);  // 输出寄存器状态到DPI-C

    always_ff @(posedge clk) begin
        if (we) regfile[waddr] <= wdata;  // 写入数据到指定寄存器
        // write_gpr_npc(waddr, wdata);  // 更新DPI-C接口寄存器
    end


    import "DPI-C" function void write_gpr_npc(
        input logic [ 4:0] idx,
        input logic [31:0] data
    );
    always_comb begin
        if (we) write_gpr_npc(waddr, wdata);  // 更新DPI-C接口寄存器
    end

    always_comb begin
        rdata1 = (raddr1 == 5'b0) ? 32'h0 : regfile[raddr1];  // 如果是x0，返回0
        rdata2 = (raddr2 == 5'b0) ? 32'h0 : regfile[raddr2];  // 如果是x0，返回0
    end
endmodule


module csr #(
    localparam int N = 4  // CSR寄存器数量
) (
    input clk,  // 时钟信号
    input [N-1:0] we,  // 写使能信号
    input [N-1:0][31:0] din,  // CSR寄存器地址
    output logic [N-1:0][31:0] dout  // CSR寄存器数据输出
);
    always @(posedge clk) begin
        for (int i = 0; i < N; i++) if (we[i]) dout[i] <= din[i];
    end

    import "DPI-C" function void write_csr_npc(
        input logic [ 1:0] idx,
        input logic [31:0] data
    );

    always_comb begin
        for (int i = 0; i < N; i++)
        if (we[i]) write_csr_npc(i[1:0], din[i]);  // 更新DPI-C接口CSR寄存器
    end

endmodule

// typedef enum logic [11:0] {
//     CSR_MTVEC   = 12'h305,
//     CSR_MEPC    = 12'h341,
//     CSR_MSTATUS = 12'h300,
//     CSR_MCAUSE  = 12'h342
// } csr_addr_t;

module inst_decode (
    input [31:0] inst,  // 输入指令
    output [6:0] opcode,  // 操作码
    output [2:0] funct3,  // 功能码
    output [6:0] funct7,  // 功能码扩展
    output [4:0] rd,  // 目的寄存器编号
    output [4:0] rs1,  // 源寄存器1编号
    output [4:0] rs2,  // 源寄存器2编号
    output logic [31:0] imm,  // 立即数
    output inst_t inst_type  // 指令类型输出
);
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
endmodule

module exe (
    input         [ 6:0]       opcode,
    input         [ 2:0]       funct3,
    input         [ 6:0]       funct7,
    input         [31:0]       src1,
    input         [31:0]       src2,
    input         [31:0]       imm,
    input         [31:0]       pc,
    input         [31:0]       snpc,
    input  inst_t              inst_type,
    output logic  [31:0]       dest,
    output logic  [31:0]       jump_target,
    output logic               jump_en,
    output logic  [ 3:0]       csr_we,
    output logic  [ 3:0][31:0] csr_wdata,
    input         [ 3:0][31:0] csr_rdata
);
    // ALU 信号
    logic [31:0] alu_a, alu_b, alu_result;
    alu_op_t alu_op;  // ALU操作码

    // ALU 实例
    alu #(
        .WIDTH(32)
    ) u_alu (
        .A(alu_a),
        .B(alu_b),
        .ALUop(alu_op),
        .Result(alu_result)
    );

    // 总是读取地址为`raddr & ~0x3u`的4字节返回
    import "DPI-C" function int pmem_read_npc(input int raddr);

    // 总是往地址为`waddr & ~0x3u`的4字节按写掩码`wmask`写入`wdata`
    import "DPI-C" function void pmem_write_npc(
        input int  waddr,
        input int  wdata,
        input byte wmask
    );

    import "DPI-C" function void NPCINV(input int pc);  // 指令非法
    import "DPI-C" function void NPCTRAP();  // ebreak 指令

    // CSR 地址映射到索引
    function automatic logic [1:0] csr_addr_to_idx(input logic [11:0] addr);
        unique case (addr)
            12'h305: return 2'd0;  // CSR_MTVEC
            12'h341: return 2'd1;  // CSR_MEPC
            12'h300: return 2'd2;  // CSR_MSTATUS
            12'h342: return 2'd3;  // CSR_MCAUSE
            default: return 2'd0;
        endcase
    endfunction

    // 操作数选择：使用 inst_type 和特定操作码判断
    always_comb begin
        jump_target = 32'h0;
        jump_en = 1'b0;
        unique case (inst_type)
            TYPE_I: begin
                alu_a = src1;
                alu_b = imm;
                if (opcode == 7'b1100111) begin  // JALR
                    jump_target = {alu_result[31:1], 1'b0};  // 低位清零
                    jump_en = 1'b1;
                end else if (opcode == 7'b1110011 && funct3 == 3'b000) begin
                    if (imm == 32'h0) begin  // ecall
                        // ecall 跳转到异常处理程序入口地址
                        jump_target = csr_rdata[0];  // CSR_MTVEC index = 0
                        jump_en = 1'b1;
                    end else if (imm == 32'h302) begin  // mret
                        // mret 跳转到异常返回地址
                        jump_target = csr_rdata[1];  // CSR_MEPC index = 1
                        jump_en = 1'b1;
                    end
                end
            end
            TYPE_R: begin
                alu_a = src1;
                alu_b = src2;
            end
            TYPE_U: begin
                alu_a = opcode == 7'b0110111 ? 32'h0 : pc;
                alu_b = imm;
            end
            TYPE_J: begin  // JAL
                alu_a = pc;
                alu_b = imm;
                jump_target = alu_result;
                jump_en = 1'b1;
            end
            TYPE_B: begin
                // B型指令：跳转目标交由ALU计算
                alu_a = pc;
                alu_b = imm;
                jump_target = alu_result;
                // 分支指令判断
                unique case (funct3)
                    3'b000: begin  // beq
                        jump_en = (src1 == src2);
                    end
                    3'b001: begin  // bne
                        jump_en = (src1 != src2);
                    end
                    3'b100: begin  // blt
                        jump_en = ($signed(src1) < $signed(src2));
                    end
                    3'b101: begin  // bge
                        jump_en = ($signed(src1) >= $signed(src2));
                    end
                    3'b110: begin  // bltu
                        jump_en = (src1 < src2);
                    end
                    3'b111: begin  // bgeu
                        jump_en = (src1 >= src2);
                    end
                    default: begin
                        jump_en = 1'b0;
                    end
                endcase
            end
            default: begin
                alu_a = src1;
                alu_b = src2;
            end
        endcase
    end

    // ALU 操作码映射
    always_comb begin
        alu_op = ALU_ADD;  // 默认操作
        if (opcode == 7'b0010011 || opcode == 7'b0110011) begin
            unique case (funct3)
                3'b000: begin  // ADD/ADDI 或 SUB
                    if (inst_type == TYPE_R && funct7 == 7'b0100000) alu_op = ALU_SUB;  // SUB
                end
                3'b001:  alu_op = ALU_SLL;  // SLL/SLLI
                3'b010:  alu_op = ALU_SLT;  // slt/slti
                3'b011:  alu_op = ALU_SLTU;  // sltu/sltiu
                3'b100:  alu_op = ALU_XOR;  // XOR/XORI
                3'b101: begin  // SRL/SRA/SRLI/SRAI
                    alu_op = (funct7[5] == 1'b1) ? ALU_SRA : ALU_SRL;
                end
                3'b110:  alu_op = ALU_OR;  // OR/ORI
                3'b111:  alu_op = ALU_AND;  // AND/ANDI
                default: alu_op = ALU_ADD;
            endcase
        end
        // TYPE_B 分支指令已在上面处理
    end

    int          mem_rdata;
    wire  [ 1:0] csr_idx = csr_addr_to_idx(imm[11:0]);
    logic [31:0] mstatus_ecall;  // 临时存储mstatus的修改值
    logic [31:0] mstatus_mret;  // mret时的mstatus修改值

    // 结果输出处理
    always_comb begin
        mem_rdata = 0;
        // CSR 信号初始化
        csr_we = '0;
        csr_wdata = '0;

        // ecall时的mstatus修改
        mstatus_ecall = csr_rdata[2];
        mstatus_ecall[7] = ~mstatus_ecall[3];  // 保存当前中断使能状态：将MIE复制到MPIE (bit 7)
        mstatus_ecall[12:11] = 2'b11;  // 设置MPP字段为机器模式 (bits 12:11 = 11)
        mstatus_ecall[3] = 1'b0;  // 禁用中断：清空MIE位 (bit 3)

        // mret时的mstatus修改
        mstatus_mret = csr_rdata[2];
        mstatus_mret[3] = mstatus_mret[7];  // 将MPIE的值设置到MIE (bit 3)
        mstatus_mret[7] = 1'b1;  // 设置MPIE = 1 (为下次中断做准备)
        mstatus_mret[12:11] = 2'b00;  // 清空MPP = 0 (设置为用户模式)
        unique case (inst_type)
            TYPE_I: begin
                if (opcode == 7'b1100111)  // JALR
                    dest = snpc;
                else if (opcode == 7'b0000011) begin  // Load指令
                    mem_rdata = pmem_read_npc(src1 + imm);
                    unique case (funct3)
                        3'b000: dest = {{24{mem_rdata[7]}}, mem_rdata[7:0]};  // lb
                        3'b010: dest = mem_rdata;  // lw
                        3'b001: dest = {{16{mem_rdata[15]}}, mem_rdata[15:0]};  // lh
                        3'b101: dest = {16'b0, mem_rdata[15:0]};  // lhu
                        3'b100: dest = {24'b0, mem_rdata[7:0]};  // lbu
                        default: begin
                            dest = 32'h0;
                            NPCINV(pc);  // 不支持的Load类型
                        end
                    endcase
                end else if (opcode == 7'b1110011) begin  // 系统指令
                    unique case (funct3)
                        3'b000: begin
                            if (imm == 32'h0) begin  // ecall: imm字段为0
                                // 实现硬件版本的 isa_raise_intr 逻辑
                                csr_we = 4'b1110;  // 写使能 mstatus, mepc, mcause
                                // 1. 保存异常发生时的PC值到mepc寄存器
                                csr_wdata[1] = pc;
                                // 2. 设置异常原因码到mcause寄存器 (NO=11表示环境调用)
                                csr_wdata[3] = 32'd11;  // 环境调用异常码
                                // 3. 更新mstatus寄存器
                                csr_wdata[2] = mstatus_ecall;
                                dest = 32'h0;  // ecall不写回寄存器
                            end else if (imm == 32'h1) begin  // ebreak: imm字段为1
                                // ebreak 调用 NPCTRAP
                                NPCTRAP();
                                dest = 32'h0;  // ebreak不写回寄存器
                            end else if (imm == 32'h302) begin  // mret: imm字段为0x302
                                // 实现硬件版本的 isa_return_intr 逻辑
                                csr_we[2] = 1'b1;  // 写使能 mstatus
                                csr_wdata[2] = mstatus_mret;  // 恢复mstatus寄存器
                                dest = 32'h0;  // mret不写回寄存器
                            end else begin
                                dest = 32'h0;
                                NPCINV(pc);  // 不支持的系统调用
                            end
                        end
                        3'b001: begin  // csrw
                            // CSR写指令：将src1的值写入CSR
                            csr_we[csr_idx] = 1'b1;
                            csr_wdata[csr_idx] = src1;
                            dest = 32'h0;
                        end
                        3'b010: begin  // csrr
                            // CSR读指令：将CSR的值读入rd
                            dest = csr_rdata[csr_idx];
                        end
                        default: begin
                            dest = 32'h0;
                            NPCINV(pc);  // 不支持的CSR操作
                        end
                    endcase
                end else begin  // I型整数运算/移位指令
                    unique case (funct3)
                        3'b000, 3'b001, 3'b010, 3'b011, 3'b100, 3'b101, 3'b110, 3'b111:
                        dest = alu_result;
                        default: begin
                            dest = 32'h0;
                            NPCINV(pc);  // 不支持的I型运算
                        end
                    endcase
                end
            end
            TYPE_R: begin
                unique case (funct3)
                    3'b000, 3'b001, 3'b010, 3'b011, 3'b100, 3'b101, 3'b110, 3'b111:
                    dest = alu_result;
                    default: begin
                        dest = 32'h0;
                        NPCINV(pc);  // 不支持的R型运算
                    end
                endcase
            end
            TYPE_U: dest = alu_result;
            TYPE_J: dest = snpc;  // JAL写回返回地址
            TYPE_B: begin
                dest = 32'h0;  // 分支指令不写回
            end
            TYPE_S: begin  // Store指令
                // S型指令：sb, sh, sw
                unique case (funct3)
                    3'b000:  pmem_write_npc(src1 + imm, src2, 8'h1);  // sb
                    3'b001:  pmem_write_npc(src1 + imm, src2, 8'h3);  // sh
                    3'b010:  pmem_write_npc(src1 + imm, src2, 8'hf);  // sw
                    default: ;  // 不支持的store类型
                endcase
                dest = 32'h0;
            end
            TYPE_N: begin  // 无操作数类型
                dest = 32'h0;
            end
            default: begin
                dest = 32'h0;
                NPCINV(pc);  // 不支持的指令类型
            end
        endcase
    end
endmodule

// 带有逻辑运算的简单ALU
module alu #(
    parameter int WIDTH = 32,
    parameter int SHIFT_WIDTH = $clog2(WIDTH)  // 计算移位宽度
) (
    input signed [WIDTH-1:0] A,
    input signed [WIDTH-1:0] B,
    alu_op_t ALUop,
    output logic signed [WIDTH-1:0] Result
);
    logic Zero, Overflow;
    logic [WIDTH-1:0] add_res, not_res, and_res, or_res, xor_res, sll_res, srl_res, sra_res;
    logic slt_res, sltu_res, eq_res;
    adder #(
        .WIDTH(WIDTH)
    ) adder_inst (
        .A(A),
        .B(B),
        .is_sub(|ALUop),
        .Carry(),
        .Zero(Zero),
        .Overflow(Overflow),
        .Result(add_res)
    );
    assign not_res = ~A;
    assign and_res = A & B;
    assign or_res  = A | B;
    assign xor_res = A ^ B;
    assign slt_res = add_res[WIDTH-1] ^ Overflow;
    assign eq_res  = Zero;
    always_comb begin
        unique case ({
            A[WIDTH-1], B[WIDTH-1]
        })
            2'b00, 2'b11: sltu_res = add_res[WIDTH-1];
            2'b01: sltu_res = 1'b1;
            2'b10: sltu_res = 1'b0;
        endcase
    end
    assign sll_res = A << B[SHIFT_WIDTH-1:0];
    assign srl_res = A >> B[SHIFT_WIDTH-1:0];
    assign sra_res = A >>> B[SHIFT_WIDTH-1:0];

    always_comb begin
        unique case (ALUop)
            ALU_ADD:  Result = add_res;  // ADD
            ALU_SUB:  Result = add_res;  // SUB
            ALU_NOT:  Result = not_res;  // NOT
            ALU_AND:  Result = and_res;  // AND
            ALU_OR:   Result = or_res;  // OR
            ALU_XOR:  Result = xor_res;  // XOR
            ALU_EQ:   Result = {31'b0, eq_res};  // EQ
            ALU_SLT:  Result = {31'b0, slt_res};  // SLT
            ALU_SLTU: Result = {31'b0, sltu_res};  // SLTU
            ALU_SLL:  Result = sll_res;  // SLL
            ALU_SRL:  Result = srl_res;  // SRL
            ALU_SRA:  Result = sra_res;  // SRA
            default:  Result = '0;  // Default case
        endcase
    end

endmodule

module adder #(
    parameter int WIDTH = 32
) (
    input [WIDTH-1:0] A,
    input [WIDTH-1:0] B,
    input is_sub,
    output logic Carry,
    Zero,
    Overflow,
    output logic [WIDTH-1:0] Result
);
    wire [WIDTH-1:0] B1;
    assign B1 = {WIDTH{is_sub}} ^ B;
    assign {Carry, Result} = A + B1 + is_sub;  //忽略此位宽问题
    assign Overflow = (A[WIDTH-1] == B1[WIDTH-1]) && (Result[WIDTH-1] != A[WIDTH-1]);
    assign Zero = ~(|Result);

endmodule
