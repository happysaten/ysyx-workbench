/***************************************************************************************
 * Copyright (c) 2014-2024 Zihao Yu, Nanjing University
 *
 * NSIM is licensed under Mulan PSL v2.
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
#include "utils.h"
#include <common.h>
#include <cpu/cpu.h>
#include <cpu/decode.h>
#include <cpu/ifetch.h>
#include <stdio.h>

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
// 取S型立即数
#define immS()  do { *imm = (SEXT(BITS(i, 31, 25), 7) << 5) | BITS(i, 11, 7); } while (0)
// 取B型立即数
#define immB()  do { *imm = (SEXT((BITS(i, 31, 31) << 12) | (BITS(i, 7, 7) << 11) | \
                                 (BITS(i, 30, 25) << 5) | (BITS(i, 11, 8) << 1), 13)); } while (0)
// 取U型立即数
#define immU()  do { *imm = SEXT(BITS(i, 31, 12), 20) << 12; } while (0)
// 取J型立即数
#define immJ()  do { *imm = (SEXT((BITS(i, 31, 31) << 20) | (BITS(i, 19, 12) << 12) | \
                                 (BITS(i, 20, 20) << 11) | (BITS(i, 30, 21) << 1), 21)); } while (0)

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
        immB();
        break;
    case TYPE_U: // U型指令，只解码imm
        immU();
        break;
    case TYPE_J: // J型指令，只解码imm
        immJ();
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
    // lui: rd = imm，加载高位立即数
    INSTPAT("??????? ????? ????? ??? ????? 01101 11", lui, U, R(rd) = imm);
    // auipc: rd = pc + imm，计算当前pc加上立即数
    INSTPAT("??????? ????? ????? ??? ????? 00101 11", auipc, U,
            R(rd) = s->pc + imm);
    // jal: 跳转并链接，rd = 返回地址，pc跳转到目标
    INSTPAT("??????? ????? ????? ??? ????? 11011 11", jal, J, {
        R(rd) = s->snpc;
        s->dnpc = s->pc + imm;
        IFDEF(CONFIG_FTRACE, {
            void ftrace_call(vaddr_t pc, vaddr_t target);
            // rd==1 (ra) 视为call
            if (rd == 1) {
                void ftrace_call(vaddr_t pc, vaddr_t target);
                ftrace_call(s->pc, s->dnpc);
            }
        });
    });

    // jalr: 跳转并链接寄存器，rd = 返回地址，pc跳转到(src1+imm)&~1
    INSTPAT("??????? ????? ????? 000 ????? 11001 11", jalr, I, {
        s->dnpc = (src1 + imm) & ~1;
        R(rd) = s->snpc;
        IFDEF(CONFIG_FTRACE, {
            void ftrace_call(vaddr_t pc, vaddr_t target);
            void ftrace_ret(vaddr_t pc);
            // rd==1 (ra) 视为call
            if (rd == 1)
                ftrace_call(s->pc, s->dnpc);
            // 标准RET: rd==0, rs1==1, imm==0
            else if (s->isa.inst == 0x00008067)
                ftrace_ret(s->pc);
        })
    });
    // beq: 如果src1==src2则跳转
    INSTPAT("??????? ????? ????? 000 ????? 11000 11", beq, B,
            if (src1 == src2) s->dnpc = s->pc + imm;);
    // bne: 如果src1!=src2则跳转
    INSTPAT("??????? ????? ????? 001 ????? 11000 11", bne, B,
            if (src1 != src2) s->dnpc = s->pc + imm;);
    // blt: 如果src1<src2则跳转（有符号比较）
    INSTPAT("??????? ????? ????? 100 ????? 11000 11", blt, B,
            if ((sword_t)src1 < (sword_t)src2) s->dnpc = s->pc + imm;);
    // bge: 如果src1>=src2则跳转（有符号比较）
    INSTPAT("??????? ????? ????? 101 ????? 11000 11", bge, B,
            if ((sword_t)src1 >= (sword_t)src2) s->dnpc = s->pc + imm;);
    // bltu: 如果src1<src2则跳转（无符号比较）
    INSTPAT("??????? ????? ????? 110 ????? 11000 11", bltu, B,
            if (src1 < src2) s->dnpc = s->pc + imm;);
    // bgeu: 如果src1>=src2则跳转（无符号比较）
    INSTPAT("??????? ????? ????? 111 ????? 11000 11", bgeu, B,
            if (src1 >= src2) s->dnpc = s->pc + imm;);
    // lb: 从内存读取1字节有符号数据到rd
    INSTPAT("??????? ????? ????? 000 ????? 00000 11", lb, I,
            R(rd) = SEXT(Mr(src1 + imm, 1), 8));
    // lh: 从内存读取2字节有符号数据到rd
    INSTPAT("??????? ????? ????? 001 ????? 00000 11", lh, I,
            R(rd) = SEXT(Mr(src1 + imm, 2), 16));
    // lw: 从内存读取4字节数据到rd
    INSTPAT("??????? ????? ????? 010 ????? 00000 11", lw, I,
            R(rd) = Mr(src1 + imm, 4));
    // lbu: 从内存读取1字节无符号数据到rd
    INSTPAT("??????? ????? ????? 100 ????? 00000 11", lbu, I,
            R(rd) = Mr(src1 + imm, 1));
    // lhu: 从内存读取2字节无符号数据到rd
    INSTPAT("??????? ????? ????? 101 ????? 00000 11", lhu, I,
            R(rd) = Mr(src1 + imm, 2) & 0xffff);
    // sb: 将src2的最低字节写入内存
    INSTPAT("??????? ????? ????? 000 ????? 01000 11", sb, S,
            Mw(src1 + imm, 1, src2));
    // sh: 将src2的2字节写入内存
    INSTPAT("??????? ????? ????? 001 ????? 01000 11", sh, S,
            Mw(src1 + imm, 2, src2));
    // sw: 将src2的4字节写入内存
    INSTPAT("??????? ????? ????? 010 ????? 01000 11", sw, S,
            Mw(src1 + imm, 4, src2));
    // addi: rd = src1 + imm，带符号立即数加法
    INSTPAT("??????? ????? ????? 000 ????? 00100 11", addi, I,
            R(rd) = src1 + imm);
    // slti: rd = (src1 < imm) ? 1 : 0，有符号立即数小于置位
    INSTPAT("??????? ????? ????? 010 ????? 00100 11", slti, I,
            R(rd) = ((sword_t)src1 < (sword_t)imm) ? 1 : 0);
    // sltiu: rd = (src1 < imm) ? 1 : 0，无符号立即数小于置位
    INSTPAT("??????? ????? ????? 011 ????? 00100 11", sltiu, I,
            R(rd) = (src1 < (word_t)imm) ? 1 : 0);
    // xori: rd = src1 ^ imm，按位异或
    INSTPAT("??????? ????? ????? 100 ????? 00100 11", xori, I,
            R(rd) = src1 ^ imm);
    // ori: rd = src1 | imm，按位或
    INSTPAT("??????? ????? ????? 110 ????? 00100 11", ori, I,
            R(rd) = src1 | imm);
    // andi: rd = src1 & imm，按位与
    INSTPAT("??????? ????? ????? 111 ????? 00100 11", andi, I,
            R(rd) = src1 & imm);
    // slli: rd = src1逻辑左移imm[4:0]位
    INSTPAT("0000000 ????? ????? 001 ????? 00100 11", slli, I,
            R(rd) = src1 << (imm & 0x1f));
    // srli: rd = src1逻辑右移imm[4:0]位
    INSTPAT("0000000 ????? ????? 101 ????? 00100 11", srli, I,
            R(rd) = src1 >> (imm & 0x1f));
    // srai: rd = src1算术右移imm[4:0]位
    INSTPAT("0100000 ????? ????? 101 ????? 00100 11", srai, I,
            R(rd) = (sword_t)src1 >> (imm & 0x1f));
    // add: rd = src1 + src2，寄存器加法
    INSTPAT("0000000 ????? ????? 000 ????? 01100 11", add, R,
            R(rd) = src1 + src2);
    // sub: rd = src1 - src2，寄存器减法
    INSTPAT("0100000 ????? ????? 000 ????? 01100 11", sub, R,
            R(rd) = src1 - src2);
    // sll: rd = src1 << (src2 & 0x1f)，逻辑左移
    INSTPAT("0000000 ????? ????? 001 ????? 01100 11", sll, R,
            R(rd) = src1 << (src2 & 0x1f));
    // slt: rd = (src1 < src2) ? 1 : 0，有符号小于置位
    INSTPAT("0000000 ????? ????? 010 ????? 01100 11", slt, R,
            R(rd) = ((sword_t)src1 < (sword_t)src2) ? 1 : 0);
    // sltu: rd = (src1 < src2) ? 1 : 0，无符号小于置位
    INSTPAT("0000000 ????? ????? 011 ????? 01100 11", sltu, R,
            R(rd) = (src1 < src2) ? 1 : 0);
    // xor: rd = src1 ^ src2，按位异或
    INSTPAT("0000000 ????? ????? 100 ????? 01100 11", xor, R,
            R(rd) = src1 ^ src2);
    // srl: rd = src1逻辑右移(src2 & 0x1f)位
    INSTPAT("0000000 ????? ????? 101 ????? 01100 11", srl, R,
            R(rd) = src1 >> (src2 & 0x1f));
    // sra: rd = src1算术右移(src2 & 0x1f)位
    INSTPAT("0100000 ????? ????? 101 ????? 01100 11", sra, R,
            R(rd) = (sword_t)src1 >> (src2 & 0x1f));
    // or: rd = src1 | src2，按位或
    INSTPAT("0000000 ????? ????? 110 ????? 01100 11", or, R,
            R(rd) = src1 | src2);
    // and: rd = src1 & src2，按位与
    INSTPAT("0000000 ????? ????? 111 ????? 01100 11", and, R,
            R(rd) = src1 & src2);
    // ebreak: 触发调试断点或系统调用
    INSTPAT("0000000 00001 00000 000 00000 11100 11", ebreak, N,
            NSIMTRAP(s->pc, R(10))); // R(10) is $a0

    // ecall: 环境调用，触发系统调用
    INSTPAT("0000000 00000 00000 000 00000 11100 11", ecall, N, {
        // 调用 isa_raise_intr 函数，NO=11 表示环境调用异常，epc为当前pc
        word_t isa_raise_intr(word_t NO, vaddr_t epc);
        s->dnpc = isa_raise_intr(11, s->pc);
        // printf("Hello from ecall\n");
#ifdef CONFIG_ETRACE
        // printf("[etrace] ecall at pc = " FMT_WORD " cause = " FMT_WORD "\n",
        //           csr(CSR_MEPC), csr(CSR_MCAUSE));
        extern bool log_enable();
        extern uint64_t g_nr_guest_inst;
        printf("log is %d, g_nr_guest_inst is %zu", log_enable(),
               g_nr_guest_inst);

        log_write("[etrace] ecall at pc = " FMT_WORD " cause = " FMT_WORD "\n",
                  csr(CSR_MEPC), csr(CSR_MCAUSE));
#endif
    });

    // csrw: CSR 写指令，将 rs1 的值写入 CSR
    INSTPAT("??????? ????? ????? 001 00000 11100 11", csrw, I, {
        // CSR 地址在 imm 字段中（即指令的 31:20 位）
        csr(imm & 0xfff) = src1;
    });

    // csrr: CSR 读指令，将 CSR 的值读入 rd
    INSTPAT("??????? ????? 00000 010 ????? 11100 11", csrr, I, {
        // CSR 地址在 imm 字段中（即指令的 31:20 位）
        R(rd) = csr(imm & 0xfff);
    });

    // mret: 机器模式返回指令，从机器模式陷阱返回
    INSTPAT("0011000 00010 00000 000 00000 11100 11", mret, N, {
        // 从 mepc 寄存器中恢复 pc
        s->dnpc = csr(CSR_MEPC);
    });

    // mul: rd = src1 * src2，乘法
    INSTPAT("0000001 ????? ????? 000 ????? 01100 11", mul, R,
            R(rd) = src1 * src2);
    // mulh: rd = 高32位(src1 * src2)，有符号乘法高位
    INSTPAT("0000001 ????? ????? 001 ????? 01100 11", mulh, R, {
        int64_t res = (int64_t)(sword_t)src1 * (int64_t)(sword_t)src2;
        R(rd) = (word_t)(res >> 32);
    });
    // mulhu: rd = 高32位(src1 * src2)，无符号乘法高位
    INSTPAT("0000001 ????? ????? 011 ????? 01100 11", mulhu, R, {
        uint64_t res = (uint64_t)src1 * (uint64_t)src2;
        R(rd) = (word_t)(res >> 32);
    });
    // div: rd = src1 / src2，除法（有符号）
    INSTPAT("0000001 ????? ????? 100 ????? 01100 11", div, R,
            R(rd) = (sword_t)src2 == 0 ? (word_t)-1
                                       : (sword_t)src1 / (sword_t)src2);
    // divu: rd = src1 / src2，除法（无符号）
    INSTPAT("0000001 ????? ????? 101 ????? 01100 11", divu, R,
            R(rd) = src2 == 0 ? (word_t)-1 : src1 / src2);
    // rem: rd = src1 % src2，取余（有符号）
    INSTPAT("0000001 ????? ????? 110 ????? 01100 11", rem, R,
            R(rd) = (sword_t)src2 == 0 ? src1 : (sword_t)src1 % (sword_t)src2);
    // remu: rd = src1 % src2，取余（无符号）
    INSTPAT("0000001 ????? ????? 111 ????? 01100 11", remu, R,
            R(rd) = src2 == 0 ? src1 : src1 % src2);
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
