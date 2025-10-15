package alu_pkgs;

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

endpackage : alu_pkgs
