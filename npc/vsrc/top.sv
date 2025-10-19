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
    output logic npc_resp_valid  // 指令响应有效输出
);

    // IFU：负责 PC 和取指
    logic [31:0] pc, snpc, jump_target;  // pc renamed to ifu_raddr, snpc, 跳转目标地址
    logic        jump_en;
    logic [31:0] inst;  // 当前指令, inst renamed to ifu_rdata
    logic [31:0] dnpc;  // 新增dnpc信号，从PCU输出

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
    assign npc_req_valid = npc_req_valid_init || npc_resp_valid;

    IFU u_ifu (
        .clk(clk),
        .reset(reset_sync),
        .jump_target(jump_target),
        .jump_en(jump_en),
        .pc(pc),
        .snpc(snpc),
        .dnpc(dnpc),
        .ifu_rdata(inst),
        .ifu_req_valid(npc_req_valid),
        .ifu_req_ready(ifu_req_ready),
        .ifu_resp_valid(ifu_resp_valid),
        .ifu_resp_ready(lsu_req_ready)
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
        .gpr_wen(gpr_we && (|rd)),
        .gpr_waddr(rd),
        .gpr_wdata(gpr_wdata),
        .gpr_raddr1(rs1),
        .gpr_raddr2(rs2),
        .gpr_rdata1(gpr_rdata1),
        .gpr_rdata2(gpr_rdata2),
        .gpr_req_valid(lsu_resp_valid),
        .gpr_req_ready(gpr_req_ready),
        .gpr_resp_valid(gpr_resp_valid),
        .gpr_resp_ready(ifu_req_ready)
    );

    // CSR：控制状态寄存器
    logic [3:0][31:0] csr_wdata, csr_rdata;  // CSR写数据，读数据
    logic [3:0] csr_we;
    CSR u_csr (
        .clk(clk),
        .reset(reset_sync),
        .csr_wen(csr_we),
        .csr_wdata(csr_wdata),
        .csr_rdata(csr_rdata),
        .csr_req_valid(lsu_resp_valid),
        .csr_req_ready(csr_req_ready),
        .csr_resp_valid(csr_resp_valid),
        .csr_resp_ready(ifu_req_ready)
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
        .inst_type     (inst_type),
        .opcode        (opcode),
        .funct3        (funct3),
        .pc            (pc),
        .alu_result    (alu_result),
        .gpr_rdata2    (gpr_rdata2),
        .lsu_rdata     (lsu_rdata),
        .lsu_req_valid (ifu_resp_valid),
        .lsu_req_ready (lsu_req_ready),
        .lsu_resp_valid(lsu_resp_valid),
        .lsu_resp_ready(gpr_req_ready && csr_req_ready)
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
    input        [31:0] jump_target,
    input               jump_en,
    input               ifu_req_valid,
    input               ifu_resp_ready,
    output logic [31:0] pc,
    output logic [31:0] snpc,
    output logic [31:0] dnpc,
    output logic [31:0] ifu_rdata,
    output logic        ifu_resp_valid,
    output logic        ifu_req_ready
);
    // DPI 接口：从内存读取指令并上报 instruction + next pc
    import "DPI-C" function int pmem_read_npc(input int raddr);
    import "DPI-C" function void update_inst_npc(
        input int inst,
        input int dnpc
    );

    localparam int RESET_PC = 32'h80000000;
    typedef enum logic {
        IDLE,
        WAIT
    } state_t;
    state_t state, next_state;

    // PC 寄存器更新
    always_ff @(posedge clk) begin
        if (reset) pc <= RESET_PC;
        else if (ifu_resp_valid) pc <= dnpc;
    end

    // snpc / dnpc 选择逻辑
    assign snpc = pc + 4;
    assign dnpc = jump_en ? jump_target : snpc;

    always @(posedge clk) begin
        if (reset) state <= IDLE;
        else state <= next_state;
    end

    always_comb begin
        unique case (state)
            IDLE: next_state = ifu_req_valid && ifu_req_ready ? WAIT : IDLE;
            WAIT: next_state = ifu_resp_valid && ifu_resp_ready ? IDLE : WAIT;
            default: next_state = IDLE;
        endcase
    end

    logic ifu_resp_valid_d;

    always @(posedge clk) ifu_resp_valid <= ifu_resp_valid_d;
    always @(posedge clk) if (ifu_resp_valid_d) ifu_rdata <= pmem_read_npc(pc);
    // always_comb ifu_rdata = pmem_read_npc(pc);
    always_comb if (ifu_resp_valid) update_inst_npc(ifu_rdata, dnpc);

    logic ifu_req_ready_rand;
    lfsr8 #(
        .TAPS(8'b10111010)
    ) u_ifu_req_lfsr (
        .clk  (clk),
        .reset(reset),
        .en   (1'b1),
        .out  (ifu_req_ready_rand)
    );
    // delay_line #(
    //     .N(10),
    //     .WIDTH(1)
    // ) u_delay_line (
    //     .clk  (clk),
    //     .reset(reset),
    //     .din  (1'b1),
    //     .dout (ifu_req_ready_rand)
    // );
    assign ifu_req_ready = ~ifu_req_ready_rand && (state == IDLE) && !reset;
    logic ifu_resp_valid_rand;
    lfsr8 #(
        .TAPS(8'b10111010)
    ) u_ifu_resp_lfsr (
        .clk  (clk),
        .reset(reset),
        .en   (1'b1),
        .out  (ifu_resp_valid_rand)
    );
    // delay_line #(
    //     .N(3),
    //     .WIDTH(1)
    // ) u_delay_line (
    //     .clk  (clk),
    //     .reset(reset),
    //     .din  (ifu_req_valid),
    //     .dout (ifu_req_valid_q)
    // );
    assign ifu_resp_valid_d = 1'b1 && (next_state == WAIT) && !reset;
    // assign ifu_resp_valid_d = !reset && ifu_req_valid;
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
    input               clk,             // 时钟信号
    input               reset,           // 复位信号
    input               gpr_wen,         // 写使能信号
    input        [ 4:0] gpr_waddr,       // 写寄存器地址
    input        [31:0] gpr_wdata,       // 写数据
    input        [ 4:0] gpr_raddr1,      // 读寄存器1地址
    input        [ 4:0] gpr_raddr2,      // 读寄存器2地址
    input               gpr_req_valid,   // 请求有效信号
    input               gpr_resp_ready,  // 响应准备信号
    output logic        gpr_req_ready,   // 请求准备信号
    output logic [31:0] gpr_rdata1,      // 读寄存器1数据
    output logic [31:0] gpr_rdata2,      // 读寄存器2数据
    output logic        gpr_resp_valid   // 响应有效信号
);
    typedef enum logic {
        IDLE,
        WAIT
    } state_t;
    state_t state, next_state;

    always @(posedge clk) begin
        if (reset) state <= IDLE;
        else state <= next_state;
    end

    always_comb begin
        unique case (state)
            IDLE: next_state = gpr_req_valid && gpr_req_ready ? WAIT : IDLE;
            WAIT: next_state = gpr_resp_valid && gpr_resp_ready ? IDLE : WAIT;
            default: next_state = IDLE;
        endcase
    end

    assign gpr_resp_valid = state == WAIT;
    assign gpr_req_ready  = (state == IDLE) && !reset;

    logic [31:0] regfile[32];  // 寄存器文件

    // import "DPI-C" function void output_gprs(input [31:0] gprs[]);
    // always_comb output_gprs(regfile);  // 输出寄存器状态到DPI-C

    always_ff @(posedge clk) begin
        if (reset) begin
            for (int i = 0; i < 32; i++) regfile[i] <= 32'h0;  // 复位时清零所有寄存器
        end else if (gpr_wen && gpr_req_valid && gpr_req_ready) begin  // 修改：添加valid条件
            regfile[gpr_waddr] <= gpr_wdata;
        end
        // write_gpr_npc(waddr, wdata);  // 更新DPI-C接口寄存器
    end


    import "DPI-C" function void write_gpr_npc(
        input logic [ 4:0] idx,
        input logic [31:0] data
    );
    always_comb begin
        if (gpr_wen && gpr_req_valid && gpr_req_ready) write_gpr_npc(gpr_waddr, gpr_wdata);
    end

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
    input [N-1:0] csr_wen,  // 写使能信号
    input [N-1:0][31:0] csr_wdata,  // 写数据
    input csr_req_valid,  // 读请求有效信号
    input csr_resp_ready,  // 响应准备信号
    output logic csr_req_ready,  // 请求准备信号
    output logic [N-1:0][31:0] csr_rdata,  // 读数据
    output logic csr_resp_valid  // 读响应有效信号
);
    typedef enum logic {
        IDLE,
        WAIT
    } state_t;
    state_t state, next_state;

    always @(posedge clk) begin
        if (reset) state <= IDLE;
        else state <= next_state;
    end

    always_comb begin
        unique case (state)
            IDLE: next_state = csr_req_valid && csr_req_ready ? WAIT : IDLE;
            WAIT: next_state = csr_resp_valid && csr_resp_ready ? IDLE : WAIT;
            default: next_state = IDLE;
        endcase
    end

    assign csr_resp_valid = state == WAIT;
    assign csr_req_ready  = (state == IDLE) && !reset;

    always_ff @(posedge clk) begin
        if (reset) csr_rdata <= '0;  // 复位时清零所有CSR寄存器
        else begin
            for (int i = 0; i < N; i++)
            if (csr_req_valid && csr_wen[i]) csr_rdata[i] <= csr_wdata[i];
        end
    end

    import "DPI-C" function void write_csr_npc(
        input logic [ 1:0] idx,
        input logic [31:0] data
    );

    always_comb begin
        for (int i = 0; i < N; i++)
        if (csr_req_valid && csr_wen[i]) write_csr_npc(i[1:0], csr_wdata[i]);
    end

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
    input                lsu_req_valid,
    input                lsu_resp_ready,
    output logic  [31:0] lsu_rdata,
    output logic         lsu_resp_valid,
    output logic         lsu_req_ready
);
    import "DPI-C" function int pmem_read_npc(input int raddr);
    import "DPI-C" function void pmem_write_npc(
        input int  waddr,
        input int  wdata,
        input byte wmask
    );
    import "DPI-C" function void NPCINV(input int pc);

    typedef enum logic {
        IDLE,
        WAIT
    } state_t;
    state_t state, next_state;

    always @(posedge clk) begin
        if (reset) state <= IDLE;
        else state <= next_state;
    end

    always_comb begin
        unique case (state)
            IDLE: next_state = lsu_req_valid && lsu_req_ready && pmem_req ? WAIT : IDLE;
            WAIT: next_state = lsu_resp_valid && lsu_resp_ready ? IDLE : WAIT;
            default: next_state = IDLE;
        endcase
    end

    logic lsu_resp_valid_d, lsu_resp_valid_q;
    always @(posedge clk) lsu_resp_valid_q <= lsu_resp_valid_d;

    // 内存接口信号
    logic [31:0] pmem_addr, pmem_rdata, pmem_wdata;
    logic [7:0] pmem_wmask;
    logic pmem_ren, pmem_wen, pmem_req, pmem_idle;
    assign pmem_req = pmem_ren || pmem_wen;
    assign lsu_resp_valid = pmem_req ? lsu_resp_valid_q : lsu_req_valid;
    always @(posedge clk) begin
        if (pmem_ren && lsu_resp_valid_d) pmem_rdata <= pmem_read_npc(pmem_addr);
    end
    // always_comb pmem_rdata = (pmem_ren && lsu_req_valid) ? pmem_read_npc(pmem_addr) : 32'b0;
    always_comb if (pmem_wen && lsu_resp_valid_q) pmem_write_npc(pmem_addr, pmem_wdata, pmem_wmask);

    // delay_line #(
    //     .N(5),
    //     .WIDTH(1)
    // ) u_delay_line (
    //     .clk  (clk),
    //     .reset(reset),
    //     .din  (pmem_idle&&lsu_req_valid && pmem_req),
    //     .dout (lsu_req_valid_q)
    // );
    logic lsu_req_valid_q;
    lfsr8 lsu_lfsr8 (
        .clk  (clk),
        .reset(reset),
        .en   (1'b1),
        .out  (lsu_req_valid_q)
    );
    assign lsu_resp_valid_d = lsu_req_valid_q && (next_state == WAIT) && !reset;
    assign lsu_req_ready = state == IDLE && !reset;

    // 指令逻辑
    assign pmem_ren = (inst_type == TYPE_I && opcode == 7'b0000011);
    assign pmem_wen = (inst_type == TYPE_S && opcode == 7'b0100011);
    assign pmem_addr = alu_result;
    assign pmem_wdata = gpr_rdata2;
    always_comb begin
        unique case (funct3)
            3'b000: lsu_rdata = {{24{pmem_rdata[7]}}, pmem_rdata[7:0]};  // LB
            3'b010: lsu_rdata = pmem_rdata;  // LW
            3'b001: lsu_rdata = {{16{pmem_rdata[15]}}, pmem_rdata[15:0]};  // LH
            3'b101: lsu_rdata = {16'b0, pmem_rdata[15:0]};  // LHU
            3'b100: lsu_rdata = {24'b0, pmem_rdata[7:0]};  // LBU
            default: begin
                lsu_rdata = 32'h0;
                if (pmem_ren) NPCINV(pc);
            end
        endcase
    end

    always_comb begin
        unique case (funct3)
            3'b000: pmem_wmask = 8'h1;  // SB
            3'b001: pmem_wmask = 8'h3;  // SH
            3'b010: pmem_wmask = 8'hF;  // SW
            default: begin
                pmem_wmask = 8'h0;
                if (pmem_wen) NPCINV(pc);
            end
        endcase
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

