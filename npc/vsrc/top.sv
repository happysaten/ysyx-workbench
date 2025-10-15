import alu_pkg::*;

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
    input clk,   // 时钟信号
    input reset  // 复位信号
    // input [31:0] inst,  // 输入指令
    // output logic [31:0] pc  // 程序计数器输出
);
    // 寄存器堆信号
    logic we;
    logic [4:0] rs1, rs2, rd;
    logic [31:0] rdata1, rdata2, wdata;
    logic        gpr_we;
    logic [31:0] alu_result;
    logic [31:0] load_data;
    logic [31:0] pc_next;

    // 解码信号
    logic [6:0] opcode, funct7;
    logic [2:0] funct3;
    logic [31:0] imm;

    // 指令类型信号
    inst_t inst_type;

    // snpc: 顺序下一个pc，dnpc: 实际下一个pc
    logic [31:0] pc;  // 程序计数器（由 ifu 输出）
    logic [31:0] snpc;
    logic [31:0] inst;

    // IFU 实例化：负责 PC 和取指
    ifu u_ifu (
        .clk     (clk),
        .reset   (reset),
        .next_pc (pc_next),
        .pc      (pc),
        .snpc    (snpc),
        .inst    (inst)
    );

    // 指令解码实例
    idu u_idu (
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
    logic [31:0] jump_target_exu;
    logic        jump_en_exu;

    exu u_exu (
        .opcode     (opcode),
        .funct3     (funct3),
        .funct7     (funct7),
        .src1       (rdata1),
        .src2       (rdata2),
        .imm        (imm),
        .pc         (pc),
        .snpc       (snpc),
        .inst_type  (inst_type),
        .alu_result (alu_result),
        .jump_target(jump_target_exu),
        .jump_en    (jump_en_exu)
    );

    lsu u_lsu (
        .inst_type (inst_type),
        .opcode    (opcode),
        .funct3    (funct3),
        .pc        (pc),
        .addr      (alu_result),
        .store_data(rdata2),
        .load_data (load_data)
    );

    wbu u_wbu (
        .inst_type      (inst_type),
        .opcode         (opcode),
        .funct3         (funct3),
        .imm            (imm),
        .pc             (pc),
        .snpc           (snpc),
        .alu_result     (alu_result),
        .load_data      (load_data),
        .src1           (rdata1),
        .csr_rdata      (csr_rdata),
        .jump_target_exu(jump_target_exu),
        .jump_en_exu    (jump_en_exu),
        .wdata          (wdata),
        .gpr_we         (gpr_we),
        .csr_we         (csr_we),
        .csr_wdata      (csr_wdata),
        .next_pc        (pc_next)
    );

    assign we = gpr_we && (|rd);

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

endmodule

// 新增：IFU 模块，负责 PC 管理与指令读取
module ifu (
    input  logic        clk,
    input  logic        reset,
    input  logic [31:0] next_pc,
    output logic [31:0] pc,
    output logic [31:0] snpc,
    output logic [31:0] inst
);
    localparam int RESET_PC = 32'h80000000;
    logic reset_sync;
    logic [31:0] inst_local;

    // DPI 接口：从内存读取指令并上报 instruction + next pc
    import "DPI-C" function int pmem_read_npc(input int raddr);
    import "DPI-C" function void update_inst_npc(
        input int inst,
        input int dnpc
    );

    // 同步复位信号（保留原逻辑）
    always_ff @(posedge clk) begin
        if (reset) reset_sync <= 1'b1;
        else reset_sync <= 1'b0;
    end

    // 读取当前 PC 指令并通知 DPI
    always_comb begin
        inst_local = pmem_read_npc(pc);
        inst       = inst_local;
        update_inst_npc(inst_local, next_pc);
    end

    // PC 寄存器更新
    always_ff @(posedge clk) begin
        if (reset_sync) pc <= RESET_PC;
        else pc <= next_pc;
    end

    assign snpc = pc + 4;
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

module idu (
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

module exu (
    input         [ 6:0]       opcode,
    input         [ 2:0]       funct3,
    input         [ 6:0]       funct7,
    input         [31:0]       src1,
    input         [31:0]       src2,
    input         [31:0]       imm,
    input         [31:0]       pc,
    input         [31:0]       snpc,
    input  inst_t              inst_type,
    output logic  [31:0]       alu_result,
    output logic  [31:0]       jump_target,
    output logic               jump_en
);
    logic [31:0] alu_a, alu_b;
    alu_op_t alu_op;

    alu #(
        .WIDTH(32)
    ) u_alu (
        .A(alu_a),
        .B(alu_b),
        .ALUop(alu_op),
        .Result(alu_result)
    );

    always_comb begin
        jump_target = 32'h0;
        jump_en     = 1'b0;
        unique case (inst_type)
            TYPE_I: begin
                alu_a = src1;
                alu_b = imm;
                if (opcode == 7'b1100111) begin
                    jump_target = {alu_result[31:1], 1'b0};
                    jump_en     = 1'b1;
                end
            end
            TYPE_R: begin
                alu_a = src1;
                alu_b = src2;
            end
            TYPE_U: begin
                alu_a = (opcode == 7'b0110111) ? 32'h0 : pc;
                alu_b = imm;
            end
            TYPE_J: begin
                alu_a = pc;
                alu_b = imm;
                jump_target = alu_result;
                jump_en     = 1'b1;
            end
            TYPE_B: begin
                alu_a = pc;
                alu_b = imm;
                jump_target = alu_result;
                unique case (funct3)
                    3'b000: jump_en = (src1 == src2);
                    3'b001: jump_en = (src1 != src2);
                    3'b100: jump_en = ($signed(src1) <  $signed(src2));
                    3'b101: jump_en = ($signed(src1) >= $signed(src2));
                    3'b110: jump_en = (src1 <  src2);
                    3'b111: jump_en = (src1 >= src2);
                    default: jump_en = 1'b0;
                endcase
            end
            TYPE_S: begin
                alu_a = src1;
                alu_b = imm;
            end
            default: begin
                alu_a = src1;
                alu_b = src2;
            end
        endcase
    end

    always_comb begin
        alu_op = ALU_ADD;
        if (opcode == 7'b0010011 || opcode == 7'b0110011) begin
            unique case (funct3)
                3'b000: begin
                    if (inst_type == TYPE_R && funct7 == 7'b0100000) alu_op = ALU_SUB;
                end
                3'b001: alu_op = ALU_SLL;
                3'b010: alu_op = ALU_SLT;
                3'b011: alu_op = ALU_SLTU;
                3'b100: alu_op = ALU_XOR;
                3'b101: alu_op = (funct7[5] == 1'b1) ? ALU_SRA : ALU_SRL;
                3'b110: alu_op = ALU_OR;
                3'b111: alu_op = ALU_AND;
                default: alu_op = ALU_ADD;
            endcase
        end
    end
endmodule

module lsu (
    input  inst_t       inst_type,
    input  logic [6:0]  opcode,
    input  logic [2:0]  funct3,
    input  logic [31:0] pc,
    input  logic [31:0] addr,
    input  logic [31:0] store_data,
    output logic [31:0] load_data
);
    import "DPI-C" function int pmem_read_npc(input int raddr);
    import "DPI-C" function void pmem_write_npc(
        input int  waddr,
        input int  wdata,
        input byte wmask
    );
    import "DPI-C" function void NPCINV(input int pc);

    int mem_rdata_raw;

    always_comb begin
        load_data     = 32'h0;
        mem_rdata_raw = 0;
        if (inst_type == TYPE_I && opcode == 7'b0000011) begin
            mem_rdata_raw = pmem_read_npc(addr);
            unique case (funct3)
                3'b000: load_data = {{24{mem_rdata_raw[7]}},  mem_rdata_raw[7:0]};
                3'b010: load_data = mem_rdata_raw;
                3'b001: load_data = {{16{mem_rdata_raw[15]}}, mem_rdata_raw[15:0]};
                3'b101: load_data = {16'b0, mem_rdata_raw[15:0]};
                3'b100: load_data = {24'b0, mem_rdata_raw[7:0]};
                default: begin
                    load_data = 32'h0;
                    NPCINV(pc);
                end
            endcase
        end

        if (inst_type == TYPE_S && opcode == 7'b0100011) begin
            unique case (funct3)
                3'b000: pmem_write_npc(addr, store_data, 8'h1);
                3'b001: pmem_write_npc(addr, store_data, 8'h3);
                3'b010: pmem_write_npc(addr, store_data, 8'hf);
                default: ; // 不支持的 store 类型保持兼容旧行为
            endcase
        end
    end
endmodule

module wbu (
    input  inst_t              inst_type,
    input  logic [6:0]         opcode,
    input  logic [2:0]         funct3,
    input  logic [31:0]        imm,
    input  logic [31:0]        pc,
    input  logic [31:0]     snpc,
    input  logic [31:0]        alu_result,
    input  logic [31:0]        load_data,
    input  logic [31:0]        src1,
    input  logic [ 3:0][31:0]  csr_rdata,
    input  logic [31:0]        jump_target_exu,
    input  logic               jump_en_exu,
    output logic [31:0]        wdata,
    output logic               gpr_we,
    output logic [ 3:0]        csr_we,
    output logic [ 3:0][31:0]  csr_wdata,
    output logic [31:0]        next_pc
);
    import "DPI-C" function void NPCINV(input int pc);
    import "DPI-C" function void NPCTRAP();

    function automatic logic [1:0] csr_addr_to_idx(input logic [11:0] addr);
        unique case (addr)
            12'h305: return 2'd0;
            12'h341: return 2'd1;
            12'h300: return 2'd2;
            12'h342: return 2'd3;
            default: return 2'd0;
        endcase
    endfunction

    logic [31:0] sys_jump_target;
    logic        sys_jump_en;
    wire [1:0] csr_idx = csr_addr_to_idx(imm[11:0]);
    logic [31:0] mstatus_ecall;
    logic [31:0] mstatus_mret;

    always_comb begin
        wdata           = 32'h0;
        gpr_we          = 1'b0;
        csr_we          = '0;
        csr_wdata       = '0;
        sys_jump_target = 32'h0;
        sys_jump_en     = 1'b0;
        next_pc         = snpc;

        mstatus_ecall = csr_rdata[2];
        mstatus_mret  = csr_rdata[2];

        unique case (inst_type)
            TYPE_I: begin
                if (opcode == 7'b1100111) begin
                    wdata  = snpc;
                    gpr_we = 1'b1;
                end else if (opcode == 7'b0000011) begin
                    wdata  = load_data;
                    gpr_we = 1'b1;
                end else if (opcode == 7'b1110011) begin
                    unique case (funct3)
                        3'b000: begin
                            if (imm == 32'h0) begin
                                csr_we = 4'b1110;
                                csr_wdata[1] = pc;
                                csr_wdata[3] = 32'd11;
                                mstatus_ecall[7]     = mstatus_ecall[3];
                                mstatus_ecall[3]     = 1'b0;
                                mstatus_ecall[12:11] = 2'b11;
                                csr_wdata[2]         = mstatus_ecall;
                                sys_jump_target      = csr_rdata[0];
                                sys_jump_en          = 1'b1;
                            end else if (imm == 32'h1) begin
                                NPCTRAP();
                            end else if (imm == 32'h302) begin
                                csr_we[2]            = 1'b1;
                                mstatus_mret[3]      = mstatus_mret[7];
                                mstatus_mret[7]      = 1'b1;
                                mstatus_mret[12:11]  = 2'b00;
                                csr_wdata[2]         = mstatus_mret;
                                sys_jump_target      = csr_rdata[1];
                                sys_jump_en          = 1'b1;
                            end else begin
                                NPCINV(pc);
                            end
                        end
                        3'b001: begin
                            csr_we[csr_idx]    = 1'b1;
                            csr_wdata[csr_idx] = src1;
                        end
                        3'b010: begin
                            wdata  = csr_rdata[csr_idx];
                            gpr_we = 1'b1;
                        end
                        default: NPCINV(pc);
                    endcase
                end else begin
                    unique case (funct3)
                        3'b000, 3'b001, 3'b010, 3'b011,
                        3'b100, 3'b101, 3'b110, 3'b111: begin
                            wdata  = alu_result;
                            gpr_we = 1'b1;
                        end
                        default: NPCINV(pc);
                    endcase
                end
            end
            TYPE_R: begin
                unique case (funct3)
                    3'b000, 3'b001, 3'b010, 3'b011,
                    3'b100, 3'b101, 3'b110, 3'b111: begin
                        wdata  = alu_result;
                        gpr_we = 1'b1;
                    end
                    default: NPCINV(pc);
                endcase
            end
            TYPE_U: begin
                wdata  = alu_result;
                gpr_we = 1'b1;
            end
            TYPE_J: begin
                wdata  = snpc;
                gpr_we = 1'b1;
            end
            default: ; // TYPE_B / TYPE_S / TYPE_N 不写回寄存器
        endcase

        if (sys_jump_en) next_pc = sys_jump_target;
        else if (jump_en_exu) next_pc = jump_target_exu;
    end
endmodule

