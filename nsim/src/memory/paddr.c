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

#include "macro.h"
#include "utils.h"
#include <device/mmio.h>
#include <isa.h>
#include <memory/host.h>
#include <memory/paddr.h>
#include <stdio.h>

// 物理内存的分配方式：使用malloc或静态数组
#if defined(CONFIG_PMEM_MALLOC)
static uint8_t *pmem = NULL; // 指向物理内存的指针
#else                        // CONFIG_PMEM_GARRAY
static uint8_t pmem[CONFIG_MSIZE] PG_ALIGN = {}; // 静态分配的物理内存
#endif

// 物理地址转主机地址
uint8_t *guest_to_host(paddr_t paddr) { return pmem + paddr - CONFIG_MBASE; }
// 主机地址转物理地址
paddr_t host_to_guest(uint8_t *haddr) { return haddr - pmem + CONFIG_MBASE; }

// 从物理内存读取数据
static word_t pmem_read(paddr_t addr, int len) {
    word_t ret = host_read(guest_to_host(addr), len);
    return ret;
}

// 向物理内存写入数据
static void pmem_write(paddr_t addr, int len, word_t data) {
    host_write(guest_to_host(addr), len, data);
}

// 物理地址越界处理
static void out_of_bound(paddr_t addr) {
    panic("address = " FMT_PADDR " is out of bound of pmem [" FMT_PADDR
          ", " FMT_PADDR "] at pc = " FMT_WORD,
          addr, PMEM_LEFT, PMEM_RIGHT, cpu.pc);
}

// 初始化物理内存
void init_mem() {
#if defined(CONFIG_PMEM_MALLOC)
    pmem = malloc(CONFIG_MSIZE);
    assert(pmem);
#endif
    IFDEF(CONFIG_MEM_RANDOM, memset(pmem, rand(), CONFIG_MSIZE));
    Log("physical memory area [" FMT_PADDR ", " FMT_PADDR "]", PMEM_LEFT,
        PMEM_RIGHT);
}

// word_t paddr_read(paddr_t addr, int len) {
//   if (likely(in_pmem(addr))) return pmem_read(addr, len);
//   IFDEF(CONFIG_DEVICE, return mmio_read(addr, len));
//   out_of_bound(addr);
//   return 0;
// }

// 物理地址读取接口
word_t paddr_read(paddr_t addr, int len) {
    word_t ret = 0;
    if (likely(in_pmem(addr)))
        ret = pmem_read(addr, len);
    else {
#ifdef CONFIG_DEVICE
        ret = mmio_read(addr, len);
#else
        // out_of_bound(addr);
#endif
    }
    IFDEF(CONFIG_MTRACE,
          log_write("pread  at " FMT_PADDR ", len = %d, data = " FMT_WORD "\n",
                    addr, len, ret));
    return ret;
}

// 物理地址写入接口
void paddr_write(paddr_t addr, int len, word_t data) {
    IFDEF(CONFIG_MTRACE,
          log_write("pwrite at " FMT_PADDR ", len = %d, data = " FMT_WORD "\n",
                    addr, len, data));
    if (likely(in_pmem(addr))) {
        pmem_write(addr, len, data);
        return;
    }
    IFDEF(CONFIG_DEVICE, mmio_write(addr, len, data); return);
    out_of_bound(addr);
}
