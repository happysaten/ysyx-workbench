#include "local-include/reg.h"
#include <cpu/cpu.h>
#include <cpu/ifetch.h>
#include <cpu/decode.h>
#include <cpu/difftest.h>
#include <common.h>

// 执行单条CPU指令的函数声明
void exec_one_cpu();

// 更新通用寄存器组
// gprs: 指向包含32个寄存器值的数组
void update_gprs(word_t *gprs) {
  for (int i = 0; i < ARRLEN(cpu.gpr); ++i) {
    gpr(i) = gprs[i]; // 设置每个寄存器的值
    // printf("%s: 0x%08x\n", reg_name(i),  gprs[i]); // 打印寄存器名称和值
  }
}

// 静态指令解码结构体指针
static Decode *ls;

// 更新当前指令信息
// inst: 指令值
// dnpc: 下一条指令的PC
void update_inst(word_t inst, vaddr_t dnpc) {
  ls->isa.inst = inst; // 设置指令值
  ls->snpc += 4;           // 指令长度为4字节，更新snpc
  ls->dnpc = dnpc;         // 设置下一条指令的PC
}

// 执行一次指令
// s: 指令解码结构体指针
int isa_exec_once(Decode *s) {
  ls = s;           // 保存当前解码结构体指针
  exec_one_cpu();   // 执行单条指令
  return 0;         // 返回0表示成功
}

void ebreak(){
  NSIMTRAP(ls->dnpc, gpr(10));
}
