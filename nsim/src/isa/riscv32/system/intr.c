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

#include "../local-include/reg.h"
#include <isa.h>

/**
 * 触发异常/中断处理
 * @param NO 异常/中断号 - 用于标识异常类型(如非法指令、系统调用等)
 * @param epc 异常程序计数器 - 触发异常时的PC值
 * @return 异常/中断向量地址 - 异常处理程序的入口地址
 */
word_t isa_raise_intr(word_t NO, vaddr_t epc) {
    /* 实现RISC-V异常处理机制:
     * 1. 保存触发异常时的PC到mepc寄存器
     * 2. 设置异常原因到mcause寄存器
     * 3. 更新mstatus寄存器的状态位
     * 4. 返回异常处理程序入口地址(mtvec寄存器值)
     */

    // 保存异常发生时的PC值到mepc寄存器
    csr(CSR_MEPC) = epc; // MEPC

    // 设置异常原因码到mcause寄存器
    // 最高位为0表示异常，为1表示中断
    csr(CSR_MCAUSE) = NO; // MCAUSE

    // 返回异常向量地址，异常处理程序将从此地址开始执行
    return csr(CSR_MTVEC); // MTVEC
}

/**
 * 查询待处理的中断
 * @return 中断状态 - 当前返回INTR_EMPTY表示无待处理中断
 */
word_t isa_query_intr() {
    // 目前返回无中断状态
    // 在完整实现中，这里应该检查中断控制器和中断使能位
    return INTR_EMPTY;
}
