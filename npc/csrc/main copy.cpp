#include "Vtop.h"
#include <nvboard.h>
#include <verilated.h>
// #include <verilated_fst_c.h>
#include <unistd.h>

void nvboard_bind_all_pins(Vtop *top);

void single_cycle(Vtop *top) {
    top->clk = ~top->clk; // Toggle clock
    top->eval();
}

void reset(Vtop *top, int n) {
    top->rst = 0;
    top->rst = 1;
    while (n-- > 0)
        single_cycle(top);
    single_cycle(top);
    top->rst = 0;
}

// 简单仿真内存，大小可根据需要调整
static uint32_t pmem[1024] = {
    // 这里可以初始化一些测试指令
    0x00100093, // addi x1, x0, 1
    0x00200113, // addi x2, x0, 2
    0x003081b3, // add  x3, x1, x3
    // ...
};

// 仿真内存读取函数
uint32_t pmem_read(uint32_t pc) {
    // 假设pc按字对齐，且小于pmem大小
    return pmem[pc >> 2];
}

#define MAX_SIM_TIME 10
vluint64_t sim_time = 0;
int main(int argc, char **argv) {
    const std::unique_ptr<VerilatedContext> contextp{new VerilatedContext};
    contextp->commandArgs(argc, argv);
    contextp->traceEverOn(true);
    Vtop *top = new Vtop;
    VerilatedFstC *tfp = new VerilatedFstC;
    top->trace(tfp, 99);                // Trace 99 levels of hierarchy
    tfp->open("obj_dir/Vtop_wave.fst"); // 打开波形文件

    // ...
    nvboard_bind_all_pins(top);
    nvboard_init(1);
    reset(top, 10);

    while (1) {
        // ...
        top->inst = pmem_read(top->pc);
        top->eval();
        nvboard_update();
        contextp->timeInc(1);
        single_cycle(top);
        tfp->dump(contextp->time());
        sim_time++;
        // usleep(1000); // Sleep for 1ms
    }
    nvboard_quit();
    // tfp->close();
    delete top;

    // 返回成功的完成状态
    return 0;
}