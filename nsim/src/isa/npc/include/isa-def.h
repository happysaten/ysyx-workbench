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

#ifndef __ISA_NPC_H__
#define __ISA_NPC_H__

#include <common.h>

typedef struct {
    word_t gpr[MUXDEF(CONFIG_RVE, 16, 32)];
    vaddr_t pc;
    // CSR (Control and Status Registers) 寄存器组
    word_t mtvec;   // 机器异常向量基地址寄存器 - 存放异常处理程序的入口地址
    word_t mepc;    // 机器异常程序计数器 - 存放触发异常时的PC值
    word_t mstatus; // 机器状态寄存器 - 存放处理器的运行状态(如中断使能位)
    word_t mcause;  // 机器异常原因寄存器 - 存放触发异常的原因代码
} npc_CPU_state;

// decode
typedef struct {
    uint32_t inst;
} npc_ISADecodeInfo;

#define isa_mmu_check(vaddr, len, type) (MMU_DIRECT)

#endif
