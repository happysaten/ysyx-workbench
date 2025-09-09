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
    csr(CSR_MEPC) = epc;

    // 设置异常原因码到mcause寄存器
    // 最高位为0表示异常，为1表示中断
    csr(CSR_MCAUSE) = NO;

    // 更新 mstatus 寄存器
    word_t mstatus = csr(CSR_MSTATUS);
    // 保存当前中断使能状态：将 MIE 复制到 MPIE
    word_t mie = (mstatus >> 3) & 0x1;            // 提取 MIE 字段 (bit 3)
    mstatus = (mstatus & ~(1 << 7)) | (mie << 7); // 将 MIE 设置到 MPIE (bit 7)
    mstatus = (mstatus & ~(0x3 << 11)) | (0x3 << 11); // 暂时假设为机器模式
    // 禁用中断：清空 MIE 位
    mstatus &= ~(1 << 3);
    // 写回 mstatus 寄存器
    csr(CSR_MSTATUS) = mstatus;

#ifdef CONFIG_ETRACE
    log_write("[etrace] ecall at pc = " FMT_WORD " cause = " FMT_WORD "\n",
              csr(CSR_MEPC), csr(CSR_MCAUSE));
#endif
    // 返回异常向量地址，异常处理程序将从此地址开始执行
    return csr(CSR_MTVEC);
}

word_t isa_return_intr(vaddr_t pc) {
    // 恢复特权级别：将 mstatus.MPP 的值复制到当前特权级别
    word_t mstatus = csr(CSR_MSTATUS);
    // 恢复中断使能状态：将 mstatus.MPIE 复制到 mstatus.MIE
    word_t mpie = (mstatus >> 7) & 0x1;       // 提取 MPIE 字段 (bit 7)
    mstatus = (mstatus & ~0x8) | (mpie << 3); // 将 MPIE 的值设置到 MIE (bit 3)
    // 设置 mstatus.MPIE = 1（为下次中断做准备）
    mstatus |= (1 << 7);
    // 清空 mstatus.MPP = 0（设置为用户模式）
    mstatus &= ~(0x3 << 11);
    // 写回 mstatus 寄存器
    csr(CSR_MSTATUS) = mstatus;

#ifdef CONFIG_ETRACE
    log_write("[etrace] mret at pc = " FMT_WORD " return to " FMT_WORD "\n", pc,
              csr(CSR_MEPC));
#endif
    return csr(CSR_MEPC);
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
