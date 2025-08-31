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

#include <device/map.h>
#include <device/alarm.h>
#include <utils.h>

// RTC端口基地址，用于映射8字节的RTC寄存器
static uint32_t *rtc_port_base = NULL;

// RTC I/O处理函数：处理RTC设备的读写操作
// offset: 偏移量（0或4），len: 长度，is_write: 是否为写操作
static void rtc_io_handler(uint32_t offset, int len, bool is_write) {
  // 断言偏移量必须为0或4
  assert(offset == 0 || offset == 4);
  // 如果是读操作且偏移为4，获取当前时间（微秒），写入RTC端口
  if (!is_write && offset == 4) {
    uint64_t us = get_time();
    rtc_port_base[0] = (uint32_t)us;
    rtc_port_base[1] = us >> 32;
  }
}

#ifndef CONFIG_TARGET_AM
// 定时器中断处理函数：当模拟器运行时，触发设备中断
static void timer_intr() {
  if (nsim_state.state == NSIM_RUNNING) {
    extern void dev_raise_intr();
    dev_raise_intr();
  }
}
#endif

// 初始化定时器设备：分配RTC端口空间，添加映射，并设置报警处理
void init_timer() {
  rtc_port_base = (uint32_t *)new_space(8);
#ifdef CONFIG_HAS_PORT_IO
  // 添加端口I/O映射
  add_pio_map ("rtc", CONFIG_RTC_PORT, rtc_port_base, 8, rtc_io_handler);
#else
  // 添加内存映射I/O
  add_mmio_map("rtc", CONFIG_RTC_MMIO, rtc_port_base, 8, rtc_io_handler);
#endif
  // 如果不是AM目标，添加定时器中断报警处理
  IFNDEF(CONFIG_TARGET_AM, add_alarm_handle(timer_intr));
}
