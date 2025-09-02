// 宏定义：用于字符串连接
#define concat_temp(x, y) x ## y
#define concat(x, y) concat_temp(x, y)
#define MAP(c, f) c(f)

// 根据RISC-V架构位宽选择指令和常量
#if __riscv_xlen == 32
#define LOAD  lw    // 32位加载指令
#define STORE sw    // 32位存储指令
#define XLEN  4     // 寄存器宽度为4字节
#else
#define LOAD  ld    // 64位加载指令
#define STORE sd    // 64位存储指令
#define XLEN  8     // 寄存器宽度为8字节
#endif

// 定义低16个寄存器的映射（跳过x0=零寄存器, x2=sp栈指针）
#define REGS_LO16(f) \
      f( 1)       f( 3) f( 4) f( 5) f( 6) f( 7) f( 8) f( 9) \
f(10) f(11) f(12) f(13) f(14) f(15)

#ifndef __riscv_e
// 完整RISC-V架构：定义高16个寄存器的映射
#define REGS_HI16(f) \
                                    f(16) f(17) f(18) f(19) \
f(20) f(21) f(22) f(23) f(24) f(25) f(26) f(27) f(28) f(29) \
f(30) f(31)
#define NR_REGS 32  // 总共32个寄存器
#else
// 嵌入式RISC-V架构：只有16个寄存器
#define REGS_HI16(f)
#define NR_REGS 16
#endif

// 组合所有需要保存的寄存器
#define REGS(f) REGS_LO16(f) REGS_HI16(f)

// 寄存器压栈和出栈宏
#define PUSH(n) STORE concat(x, n), (n * XLEN)(sp);  // 将寄存器xn保存到栈上
#define POP(n)  LOAD  concat(x, n), (n * XLEN)(sp);  // 从栈上恢复寄存器xn

// 上下文结构的大小和偏移量定义
#define CONTEXT_SIZE  ((NR_REGS + 3) * XLEN)  // 上下文总大小：寄存器+3个CSR
#define OFFSET_SP     ( 2 * XLEN)             // sp寄存器的偏移量
#define OFFSET_CAUSE  ((NR_REGS + 0) * XLEN)  // mcause寄存器的偏移量
#define OFFSET_STATUS ((NR_REGS + 1) * XLEN)  // mstatus寄存器的偏移量
#define OFFSET_EPC    ((NR_REGS + 2) * XLEN)  // mepc寄存器的偏移量

.align 3                    // 8字节对齐
.globl __am_asm_trap        // 全局符号：汇编trap入口
__am_asm_trap:
  addi sp, sp, -CONTEXT_SIZE  // 在栈上分配上下文空间

  MAP(REGS, PUSH)            // 保存所有通用寄存器到栈上

  // 读取机器模式下的控制状态寄存器
  csrr t0, mcause            // 读取trap原因
  csrr t1, mstatus           // 读取机器状态寄存器
  csrr t2, mepc              // 读取异常程序计数器

  // 将CSR寄存器保存到栈上的指定位置
  STORE t0, OFFSET_CAUSE(sp)   // 保存mcause
  STORE t1, OFFSET_STATUS(sp)  // 保存mstatus
  STORE t2, OFFSET_EPC(sp)     // 保存mepc

  // 设置mstatus.MPRV位以通过difftest
  li a0, (1 << 17)           // MPRV位掩码
  or t1, t1, a0              // 设置MPRV位
  csrw mstatus, t1           // 写回mstatus

  mv a0, sp                  // 将上下文指针作为参数传递
  call __am_irq_handle       // 调用C语言中断处理函数

  // 上下文切换：检查返回值是否为新的上下文指针
  mv sp, a0                  // 将返回的上下文指针设置为新的栈指针

  // 恢复控制状态寄存器
  LOAD t1, OFFSET_STATUS(sp) // 从栈上加载mstatus
  LOAD t2, OFFSET_EPC(sp)    // 从栈上加载mepc
  csrw mstatus, t1           // 恢复mstatus
  csrw mepc, t2              // 恢复mepc

  MAP(REGS, POP)             // 恢复所有通用寄存器

  addi sp, sp, CONTEXT_SIZE  // 释放上下文空间
  mret                       // 机器模式异常返回
