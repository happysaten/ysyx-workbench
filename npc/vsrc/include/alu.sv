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
