/***************************************************************************************
* Copyright (c) 2014-2024 Zihao Yu, Nanjing University
*
* NSIM is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>
#include <memory/host.h>
#include <memory/vaddr.h>
#include <device/map.h>

#define IO_SPACE_MAX (32 * 1024 * 1024)

static uint8_t *io_space = NULL;
static uint8_t *p_space = NULL;

uint8_t* new_space(int size) {
  uint8_t *p = p_space;
  // page aligned;
  size = (size + (PAGE_SIZE - 1)) & ~PAGE_MASK;
  p_space += size;
  assert(p_space - io_space < IO_SPACE_MAX);
  return p;
}

static void check_bound(IOMap *map, paddr_t addr) {
  if (map == NULL) {
    Assert(map != NULL, "address (" FMT_PADDR ") is out of bound at pc = " FMT_WORD, addr, cpu.pc);
  } else {
    Assert(addr <= map->high && addr >= map->low,
        "address (" FMT_PADDR ") is out of bound {%s} [" FMT_PADDR ", " FMT_PADDR "] at pc = " FMT_WORD,
        addr, map->name, map->low, map->high, cpu.pc);
  }
}

static void invoke_callback(io_callback_t c, paddr_t offset, int len, bool is_write) {
  if (c != NULL) { c(offset, len, is_write); }
}

void init_map() {
  io_space = malloc(IO_SPACE_MAX);
  assert(io_space);
  p_space = io_space;
}

/**
 * 从IO映射区域读取数据
 * @param addr 物理地址
 * @param len 读取长度(1-8字节)
 * @param map IO映射结构体指针
 * @return 读取到的数据
 */
word_t map_read(paddr_t addr, int len, IOMap *map) {
  assert(len >= 1 && len <= 8);  // 确保读取长度有效
  check_bound(map, addr);        // 检查地址是否在映射范围内
  paddr_t offset = addr - map->low;  // 计算在映射区域内的偏移
  invoke_callback(map->callback, offset, len, false); // 调用回调函数准备读取数据
  word_t ret = host_read(map->space + offset, len);   // 从主机内存读取数据
  return ret;
}

/**
 * 向IO映射区域写入数据
 * @param addr 物理地址
 * @param len 写入长度(1-8字节)  
 * @param data 要写入的数据
 * @param map IO映射结构体指针
 */
void map_write(paddr_t addr, int len, word_t data, IOMap *map) {
  assert(len >= 1 && len <= 8);  // 确保写入长度有效
  check_bound(map, addr);        // 检查地址是否在映射范围内
  paddr_t offset = addr - map->low;  // 计算在映射区域内的偏移
  host_write(map->space + offset, len, data);  // 向主机内存写入数据
  invoke_callback(map->callback, offset, len, true);  // 调用回调函数处理写入操作
}
