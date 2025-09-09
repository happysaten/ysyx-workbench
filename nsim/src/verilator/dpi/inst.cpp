#include "../include/verilator.h"
#include "../include/npc_callback.h"
#include <Vtop__Dpi.h>
#include <common.h>
#include <svdpi.h>

// 外部C接口：更新指令信息
// extern "C" void update_inst(word_t inst, vaddr_t dnpc);


// DPI-C接口：更新指令信息
void update_inst_npc(int inst, int dnpc) {
    if (!DPI_EN)
        return;
    // 调用外部C接口更新指令信息
    update_inst(inst, dnpc);
}