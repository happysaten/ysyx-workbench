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

#ifndef __RISCV_REG_H__
#define __RISCV_REG_H__

#include <common.h>

// CSR寄存器地址定义
#define CSR_MSTATUS   0x300  // 机器状态寄存器
#define CSR_MTVEC     0x305  // 机器异常向量基地址寄存器
#define CSR_MEPC      0x341  // 机器异常程序计数器
#define CSR_MCAUSE    0x342  // 机器异常原因寄存器

// 检查通用寄存器索引的有效性
// idx: 寄存器索引 (0-31 for RV32I, 0-15 for RV32E)
// 返回: 有效的寄存器索引
static inline int check_reg_idx(int idx) {
    IFDEF(CONFIG_RT_CHECK,
          assert(idx >= 0 && idx < MUXDEF(CONFIG_RVE, 16, 32)));
    return idx;
}

// 检查CSR寄存器地址的有效性
// addr: CSR寄存器地址
// 返回: 有效的CSR寄存器地址
static inline uint32_t check_csr_addr(uint32_t addr) {
    IFDEF(CONFIG_RT_CHECK, {
        switch(addr) {
            case CSR_MSTATUS:
            case CSR_MTVEC:
            case CSR_MEPC:
            case CSR_MCAUSE:
                break;
            default:
                Assert(0, "Invalid CSR address 0x%x\n", addr);
        }
    });
    return addr;
}

// 通用寄存器访问宏
// idx: 寄存器索引
#define gpr(idx) (cpu.gpr[check_reg_idx(idx)])

// CSR寄存器地址获取内部宏
// addr: CSR寄存器地址
// 返回: 指向对应CSR寄存器的指针
#define __csr_addr(addr) ({ \
    uint32_t checked_addr = check_csr_addr(addr); \
    word_t* ptr; \
    switch(checked_addr) { \
        case CSR_MSTATUS: ptr = &cpu.mstatus; break; \
        case CSR_MTVEC:   ptr = &cpu.mtvec; break; \
        case CSR_MEPC:    ptr = &cpu.mepc; break; \
        case CSR_MCAUSE:  ptr = &cpu.mcause; break; \
        default: \
            Assert(0, "Unsupported CSR address 0x%x\n", checked_addr); \
            ptr = &cpu.mstatus; \
    } \
    ptr; \
})

// CSR寄存器解引用宏
// addr: CSR寄存器地址
// 返回对应CSR寄存器的引用，可作为左值或右值使用
#define csr(addr) (*__csr_addr(addr))

// 获取通用寄存器名称
// idx: 寄存器索引
// 返回: 寄存器名称字符串
static inline const char *reg_name(int idx) {
    extern const char *regs[];
    return regs[check_reg_idx(idx)];
}

// 获取CSR寄存器名称
// addr: CSR寄存器地址
// 返回: CSR寄存器名称字符串
static inline const char *csr_name(uint32_t addr) {
    switch (addr) {
    case CSR_MSTATUS:
        return "mstatus";
    case CSR_MTVEC:
        return "mtvec";
    case CSR_MEPC:
        return "mepc";
    case CSR_MCAUSE:
        return "mcause";
    default:
        return "unknown";
    }
}

#endif
