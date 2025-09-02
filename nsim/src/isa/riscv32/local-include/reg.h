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

// 检查通用寄存器索引的有效性
// idx: 寄存器索引 (0-31 for RV32I, 0-15 for RV32E)
// 返回: 有效的寄存器索引
static inline int check_reg_idx(int idx) {
    IFDEF(CONFIG_RT_CHECK,
          assert(idx >= 0 && idx < MUXDEF(CONFIG_RVE, 16, 32)));
    return idx;
}

// 通用寄存器访问宏
// idx: 寄存器索引
#define gpr(idx) (cpu.gpr[check_reg_idx(idx)])

// CSR寄存器访问宏定义
#define csr_mepc    cpu.mepc     // 机器异常程序计数器
#define csr_mstatus cpu.mstatus  // 机器状态寄存器
#define csr_mcause  cpu.mcause   // 机器异常原因寄存器
#define csr_mtvec  cpu.mtvec    // 机器异常向量基地址寄存器

// CSR寄存器读取函数
// addr: CSR寄存器地址
// 返回: CSR寄存器的值
#define csr_read(addr) ({ \
  word_t result = 0; \
  switch(addr) { \
    case CSR_MSTATUS: result = csr_mstatus; break; \
    case 0x305: result = csr_mtvec; break; \
    case 0x341: result = csr_mepc; break; \
    case 0x342: result = csr_mcause; break; \
    default: \
      Assert(0, "Unsupported CSR read at address 0x%x\n", addr); \
        break; \
  } \
  result; \
})

// CSR寄存器写入函数
// addr: CSR寄存器地址
// val: 要写入的值
// CSR寄存器写入函数
// addr: CSR寄存器地址
// val: 要写入的值
#define csr_write(addr, val) do { \
  switch(addr) { \
    case 0x300: csr_mstatus = val; break;  /* MSTATUS */ \
    case 0x305: csr_mtvec = val; break;    /* MTVEC */ \
    case 0x341: csr_mepc = val; break;     /* MEPC */ \
    case 0x342: csr_mcause = val; break;   /* MCAUSE */ \
    default: \
      Assert(0, "Unsupported CSR write at address 0x%x\n", addr); \
        break; \
  } \
} while(0)

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
    case 0x300:
        return "mstatus";
    case 0x305:
        return "mtvec";
    case 0x341:
        return "mepc";
    case 0x342:
        return "mcause";
    default:
        return "unknown";
    }
}

#endif
