#include "../include/npc_callback.h"
#include "../include/verilator.h"
#include <Vtop__Dpi.h>
#include <common.h>
#include <cpu/cpu.h>
#include <cstdio>
#include <svdpi.h>

#define HIGH 1  // 高电平
#define LOW  0  // 低电平

// 外部声明：Verilator仿真上下文、顶层模块、波形文件操作对象
extern std::unique_ptr<VerilatedContext> context;
extern Vtop *top;
extern VerilatedFstC *tfp;

// 单步时钟仿真函数
// 每次调用模拟一个完整的时钟周期（上升沿+下降沿）
static void step() {
    context->timeInc(1); // 仿真时间递增
    top->clk = HIGH;     // 设置时钟高电平
    top->eval();         // 计算仿真
#ifdef CONFIG_WTRACE
    tfp->dump(context->time()); // 波形记录
#endif

    context->timeInc(1); // 仿真时间递增
    top->clk = LOW;      // 设置时钟低电平
    top->eval();         // 计算仿真
#ifdef CONFIG_WTRACE
    tfp->dump(context->time()); // 波形记录
#endif
}

// // 外部C接口：更新指令信息
// extern "C" void update_inst(word_t inst, vaddr_t dnpc);
// 外部C接口：从物理内存读取数据
// extern "C" word_t paddr_read(paddr_t addr, int len);
// 外部C接口：从物理内存写入数据
// extern "C" void paddr_write(paddr_t addr, int len, word_t data);
// 外部C接口：NPCTRAP
// extern "C" void ebreak();
// 外部C接口：无效指令处理
// extern "C" void invalid_inst(vaddr_t thispc);

// 执行单条CPU指令
extern "C" void exec_one_cpu() {
    // for (int i = 0; i < 2; i++) {
    //     step(); // 仿真两个周期以完成指令执行
    //     printf("%d\n", top->npc_resp_valid);
    // }
    // 等待 IFU 响应有效，带超时保护
    constexpr int kTimeoutCycles = 10;
    int cycles = 0;
    do {
        // printf("%d\n", top->npc_resp_valid);
        step();
        // printf("%d\n", top->npc_resp_valid);
        if (context->gotFinish())
            break;
        if (++cycles > kTimeoutCycles) {
            fprintf(stderr,
                    "exec_one_cpu: timeout waiting for npc_resp_valid\n");
            break;
        }
    } while (top->npc_resp_valid != 1);
}

bool DPI_EN = false; // 定义并初始化
// 复位CPU
extern "C" void reset_cpu() {
    DPI_EN = false;    // 禁用DPI-C接口
    top->reset = HIGH; // 复位信号拉高

    for (int i = 0; i < 3; i++) {
        step(); // 仿真3个周期，保证复位信号有效
    }

    DPI_EN = true;    // 启用DPI-C接口
    top->reset = LOW; // 复位信号拉低
}

// 仿真结束操作
extern "C" void finish_simulation() {
    tfp->close();      // 关闭波形文件
    top->final();      // 结束仿真
    context->gotFinish(true);
    // printf("NPC Finish = %d\n", context->gotFinish());
}

void NPCTRAP() {
    if (!DPI_EN)
        return;
    // printf("NPCTRAP called at pc = " FMT_WORD "\n", top->pc);
    ebreak();     // 调用ebreak处理函数
    // reset_cpu();
}

void NPCINV(int pc) {
    if (!DPI_EN)
        return;
    INV((vaddr_t)pc); // 调用无效指令处理函数
}