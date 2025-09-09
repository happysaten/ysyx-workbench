#include "../include/verilator.h"
#include <Vtop__Dpi.h>
#include <common.h>
#include <cstdint>
#include <memory/paddr.h>
#include <svdpi.h>

// // 外部C接口：从物理内存读取数据
// extern "C" word_t paddr_read(paddr_t addr, int len);
// // 外部C接口：从物理内存写入数据
// extern "C" void paddr_write(paddr_t addr, int len, word_t data);

#define UNALIGN_MEM 1

// DPI-C接口：读取内存
int pmem_read_npc(int raddr) {
    if (!DPI_EN)
        return 0;
#if UNALIGN_MEM
    paddr_t addr = raddr;
#else
    paddr_t addr = raddr & ~0x3u;
#endif
    word_t data = paddr_read(addr, 4);
    return static_cast<int>(data);
}

// DPI-C接口：写入内存
void pmem_write_npc(int waddr, int wdata, char wmask) {
    if (!DPI_EN)
        return;
    // 检查是否写入串口地址
    if (waddr >= CONFIG_SERIAL_MMIO && waddr < CONFIG_SERIAL_MMIO + 8) {
        // putchar(wdata & 0xff);
        paddr_write(waddr, 1, wdata & 0xff);
        return;
    }

#ifdef UNALIGN_MEM
    paddr_t waddr_ = waddr;
#else
    paddr_t addr = waddr & ~0x3u;
#endif
    word_t wdata_ = paddr_read(waddr_, 4);

    for (int i = 0; i < 4; ++i) {
        if (wmask & (1 << i)) {
            wdata_ &= ~(0xFFu << (i * 8));
            wdata_ |= ((wdata >> (i * 8)) & 0xFFu) << (i * 8);
        }
    }
    paddr_write(waddr_, 4, wdata_);
}
