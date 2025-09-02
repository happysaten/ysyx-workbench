#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

Context* __am_irq_handle(Context *c) {
  // 调试输出：打印上下文信息
  printf("=== Context Debug Info ===\n");
  printf("mcause: 0x%x\n", c->mcause);
  printf("mstatus: 0x%x\n", c->mstatus);
  printf("mepc: 0x%x\n", c->mepc);
  
  // 打印部分通用寄存器
  printf("GPR dump:\n");
  for (int i = 0; i < NR_REGS && i < 8; i++) {
    printf("  gpr[%d] (x%d): 0x%x\n", i, 
           (i == 0) ? 0 : (i == 1) ? 1 : i + 2, c->gpr[i]);
  }
  
#ifdef __riscv_e
  printf("GPR1 (a5): 0x%x\n", c->GPR1);
#else
  printf("GPR1 (a7): 0x%x\n", c->GPR1);
#endif
  
  printf("pdir: %p\n", c->pdir);
  printf("=== End Context Debug ===\n");

  if (user_handler) {
    Event ev = {0};
    switch (c->mcause) {
      case 11: // Environment call from M-mode (自陷异常)
        // 检查是否为yield操作：GPR1寄存器值为-1
        if (c->GPR1 == -1) {
          ev.event = EVENT_YIELD;
        } else {
          ev.event = EVENT_SYSCALL;
        }
        // 自陷异常需要将mepc+4，跳过ecall指令
        c->mepc += 4;
        break;
      default: 
        ev.event = EVENT_ERROR; 
        break;
    }

    c = user_handler(ev, c);
    assert(c != NULL);
  }

  return c;
}

extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));

  // register event handler
  user_handler = handler;

  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  return NULL;
}

void yield() {
#ifdef __riscv_e
  asm volatile("li a5, -1; ecall");
#else
  asm volatile("li a7, -1; ecall");
#endif
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
