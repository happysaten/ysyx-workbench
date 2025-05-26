/***************************************************************************************
 * Copyright (c) 2014-2024 Zihao Yu, Nanjing University
 *
 * NEMU is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan
 *PSL v2. You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY
 *KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
 *NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 *
 * See the Mulan PSL v2 for more details.
 ***************************************************************************************/

#include "local-include/reg.h"
#include <cpu/cpu.h>
#include <cpu/decode.h>
#include <cpu/ifetch.h>

#define R(i) gpr(i)           // 访问通用寄存器
#define Mr vaddr_read         // 读取虚拟地址
#define Mw vaddr_write        // 写虚拟地址

// 指令类型枚举
enum {
    TYPE_R, // R型指令，寄存器间操作
    TYPE_I, // I型指令，带有立即数和寄存器操作数
    TYPE_S, // S型指令，存储指令
    TYPE_B, // B型指令，分支指令
    TYPE_U, // U型指令，带有高位立即数
    TYPE_J, // J型指令，跳转指令
    TYPE_N, // none，无操作数类型
};

// 读取rs1寄存器
#define src1R() do { *src1 = R(rs1); } while (0)
// 读取rs2寄存器
#define src2R() do { *src2 = R(rs2); } while (0)
// 取I型立即数
#define immI()  do { *imm = SEXT(BITS(i, 31, 20), 12); } while (0)
// 取U型立即数
#define immU()  do { *imm = SEXT(BITS(i, 31, 12), 20) << 12; } while (0)
// 取S型立即数
#define immS()  do { *imm = (SEXT(BITS(i, 31, 25), 7) << 5) | BITS(i, 11, 7); } while (0)

// 解码操作数
static void decode_operand(Decode *s, int *rd, word_t *src1, word_t *src2,
                           word_t *imm, int type) {
    uint32_t i = s->isa.inst;
    int rs1 = BITS(i, 19, 15); // 源寄存器1编号
    int rs2 = BITS(i, 24, 20); // 源寄存器2编号
    *rd = BITS(i, 11, 7);      // 目的寄存器编号
    switch (type) {
    case TYPE_R: // R型指令，解码rs1和rs2
        src1R();
        src2R();
        break;
    case TYPE_I: // I型指令，解码rs1和imm
        src1R();
        immI();
        break;
    case TYPE_S: // S型指令，解码rs1、rs2和imm
        src1R();
        src2R();
        immS();
        break;
    case TYPE_B: // B型指令，解码rs1、rs2和分支立即数
        src1R();
        src2R();
        // B型立即数编码方式与S型类似，但需拼接不同位段
        *imm = (SEXT((BITS(i, 31, 31) << 12) | (BITS(i, 7, 7) << 11) |
                         (BITS(i, 30, 25) << 5) | (BITS(i, 11, 8) << 1),
                     13));
        break;
    case TYPE_U: // U型指令，只解码imm
        immU();
        break;
    case TYPE_J: // J型指令，只解码imm
        // J型立即数编码方式
        *imm = (SEXT((BITS(i, 31, 31) << 20) | (BITS(i, 19, 12) << 12) |
                         (BITS(i, 20, 20) << 11) | (BITS(i, 30, 21) << 1),
                     21));
        break;
    case TYPE_N: // 无操作数类型
        break;
    default:
        panic("unsupported type = %d", type);
    }
}

// 指令执行主函数
static int decode_exec(Decode *s) {
    s->dnpc = s->snpc; // 默认下一条指令地址为顺序执行

// 获取当前指令
#define INSTPAT_INST(s) ((s)->isa.inst)
// 指令匹配宏，解码操作数并执行指令主体
#define INSTPAT_MATCH(s, name, type, ... /* execute body */) \
        { \
            int rd = 0; \
            word_t src1 = 0, src2 = 0, imm = 0; \
            decode_operand(s, &rd, &src1, &src2, &imm, concat(TYPE_, type)); \
            __VA_ARGS__; \
        }

    INSTPAT_START();
    // auipc: rd = pc + imm，计算当前pc加上立即数
    INSTPAT("??????? ????? ????? ??? ????? 00101 11", auipc, U,
            R(rd) = s->pc + imm);
    // jal: 跳转并链接，rd = 返回地址，pc跳转到目标
    INSTPAT("??????? ????? ????? ??? ????? 11011 11", jal, J, R(rd) = s->snpc;
            s->dnpc = s->pc + imm;);
    // jalr: 跳转并链接寄存器，rd = 返回地址，pc跳转到(src1+imm)&~1
    INSTPAT("??????? ????? ????? 000 ????? 11001 11", jalr, I, 
            { word_t t = s->snpc; s->dnpc = (src1 + imm) & ~1; R(rd) = t; });
    // lw: 从内存读取4字节数据到rd
    INSTPAT("??????? ????? ????? 010 ????? 00000 11", lw, I,
            R(rd) = Mr(src1 + imm, 4));
    // lbu: 从内存读取1字节无符号数据到rd
    INSTPAT("??????? ????? ????? 100 ????? 00000 11", lbu, I,
            R(rd) = Mr(src1 + imm, 1));
    // sb: 将src2的最低字节写入内存
    INSTPAT("??????? ????? ????? 000 ????? 01000 11", sb, S,
            Mw(src1 + imm, 1, src2));
    // sw: 将src2的4字节写入内存
    INSTPAT("??????? ????? ????? 010 ????? 01000 11", sw, S,
            Mw(src1 + imm, 4, src2));
    // addi: rd = src1 + imm，带符号立即数加法
    INSTPAT("??????? ????? ????? 000 ????? 00100 11", addi, I,
            R(rd) = src1 + imm);
    // add: rd = src1 + src2，寄存器加法
    INSTPAT("0000000 ????? ????? 000 ????? 01100 11", add, R,
            R(rd) = src1 + src2);
    // sltu: rd = (src1 < src2) ? 1 : 0，无符号小于置位
    INSTPAT("0000000 ????? ????? 011 ????? 01100 11", sltu, R,
            R(rd) = (src1 < src2) ? 1 : 0);
    // ebreak: 触发调试断点或系统调用
    INSTPAT("0000000 00001 00000 000 00000 11100 11", ebreak, N,
            NEMUTRAP(s->pc, R(10))); // R(10) is $a0
    // inv: 未知或非法指令
    INSTPAT("??????? ????? ????? ??? ????? ????? ??", inv, N, INV(s->pc));
    INSTPAT_END();

    R(0) = 0; // reset $zero to 0

    return 0;
}

// 取指并执行一次指令
int isa_exec_once(Decode *s) {
    s->isa.inst = inst_fetch(&s->snpc, 4);
    return decode_exec(s);
}
