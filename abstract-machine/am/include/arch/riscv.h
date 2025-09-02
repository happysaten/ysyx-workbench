#ifndef ARCH_H__
#define ARCH_H__

#ifdef __riscv_e
#define NR_REGS 16
#else
#define NR_REGS 32
#endif

struct Context {
  // 按照trap.S中的保存顺序重新组织
  // 通用寄存器按索引顺序保存（除了x0和x2）
  uintptr_t gpr[NR_REGS];
  // 控制状态寄存器
  uintptr_t mcause, mstatus, mepc;
  // 地址空间信息，与x0共用存储空间
  void *pdir;
};

#ifdef __riscv_e
#define GPR1 gpr[15] // a5
#else
#define GPR1 gpr[17] // a7
#endif

#define GPR2 gpr[10]  // a0
#define GPR3 gpr[11]  // a1
#define GPR4 gpr[12]  // a2
#define GPRx gpr[10]  // a0

#endif