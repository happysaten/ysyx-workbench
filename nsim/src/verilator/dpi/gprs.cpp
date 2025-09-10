#include "../include/npc_callback.h"
#include "../include/verilator.h"
#include "Vtop__Dpi.h"
#include <common.h>
#include <cstdint>
#include <svdpi.h>
#include <verilated_dpi.h>

// 外部C接口
// extern "C" void update_gprs(word_t *gprs);

void output_gprs(const svOpenArrayHandle gprs) {
    if (!DPI_EN)
        return;
    update_gprs((word_t *)(((VerilatedDpiOpenVar *)gprs)->datap()));
}

// 外部C接口
// extern "C" void write_gpr(int idx, word_t data);
// DPI-C接口：写入通用寄存器
void write_gpr_npc(const svLogicVecVal *idx, const svLogicVecVal *data) {
    if (!DPI_EN)
        return;
    // 调用外部C接口写入寄存器
    write_gpr(*(int *)idx, *(word_t *)data);
}

// 外部C接口
// extern "C" void write_csr(int idx, word_t data);
// DPI-C接口：写入CSR寄存器
void write_csr_npc(const svLogicVecVal *idx, const svLogicVecVal *data) {
    if (!DPI_EN)
        return;
    // 调用外部C接口写入CSR寄存器
    write_csr(*(int *)idx, *(word_t *)data);
}
