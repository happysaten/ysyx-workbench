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
#include <memory/paddr.h>
#include <stdio.h>

// img数组为内置指令镜像，直接以uint32_t存储指令数据
// 注意：与uint8_t不一致，但不会直接访问数组内容，因此无影响
static const uint32_t img [] = {
  0x00000297,  // auipc t0,0   // 将当前PC存入t0寄存器
  0x00028823,  // sb  zero,16(t0) // 将0写入t0+16地址
  0x0102c503,  // lbu a0,16(t0)  // 从t0+16地址读入a0
  0x00100073,  // ebreak (used as nsim_trap) // 触发断点（模拟trap）
  0xdeadbeef,  // some data // 测试数据
};

// 声明复位CPU的函数
void reset_cpu();

// restart函数用于重置CPU状态并设置初始PC
static void restart() {
  reset_cpu(); // 复位CPU所有寄存器和状态
  /* 设置初始程序计数器（PC） */
  cpu.pc = RESET_VECTOR;
  /* 零号寄存器始终为0 */
  cpu.gpr[0] = 0;
}

// init_isa函数用于初始化ISA相关内容
void init_isa() {
  /* 加载内置镜像到内存 */
  memcpy(guest_to_host(RESET_VECTOR), img, sizeof(img));

  /* 初始化虚拟计算机系统 */
  restart();
}
