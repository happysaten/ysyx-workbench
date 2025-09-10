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
#include "macro.h"
#include <cpu/difftest.h>
#include <isa.h>

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
    // return false;
    for (int i = 0; i < ARRLEN(cpu.gpr); i++) {
        if (ref_r->gpr[i] != cpu.gpr[i]) {
            Log("%s: ref = " FMT_WORD ", dut = " FMT_WORD ,
                reg_name(i), ref_r->gpr[i], cpu.gpr[i]);
            return false;
        }
    }
    if (ref_r->pc != cpu.pc) {
        Log("pc: ref = " FMT_WORD ", dut = " FMT_WORD, ref_r->pc, cpu.pc);
        return false;
    }

    // Check CSR registers
    if (ref_r->mstatus != cpu.mstatus) {
        Log("mstatus: ref = " FMT_WORD ", dut = " FMT_WORD, ref_r->mstatus,
            cpu.mstatus);
        return false;
    }
    if (ref_r->mepc != cpu.mepc) {
        Log("mepc: ref = " FMT_WORD ", dut = " FMT_WORD, ref_r->mepc, cpu.mepc);
        return false;
    }
    if (ref_r->mcause != cpu.mcause) {
        Log("mcause: ref = " FMT_WORD ", dut = " FMT_WORD, ref_r->mcause,
            cpu.mcause);
        return false;
    }
    if (ref_r->mtvec != cpu.mtvec) {
        Log("mtvec: ref = " FMT_WORD ", dut = " FMT_WORD, ref_r->mtvec,
            cpu.mtvec);
        return false;
    }

    return true;
}

void isa_difftest_attach() {}
