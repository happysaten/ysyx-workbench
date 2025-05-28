#include "Vtop.h"
#include "Vtop__Dpi.h"
#include <stdint.h>
#include <unistd.h>
#include <verilated.h>
#include <verilated_fst_c.h>

const std::unique_ptr<VerilatedContext> contextp{new VerilatedContext};
Vtop *top = new Vtop;
VerilatedFstC *tfp = new VerilatedFstC;

void single_cycle() {
    top->clk = ~top->clk; // Toggle clock
    top->eval();
    contextp->timeInc(1);
    tfp->dump(contextp->time());
    top->clk = ~top->clk; // Toggle clock
    top->eval();
    contextp->timeInc(1);
    tfp->dump(contextp->time());
}

void reset(int n) {
    top->rst = 0;
    top->rst = 1;
    while (n-- > 0)
        single_cycle();
    single_cycle();
    top->rst = 0;
}

typedef uint32_t word_t;
typedef word_t vaddr_t;

enum { NPC_RUNNING, NPC_STOP, NPC_END, NPC_ABORT, NPC_QUIT };

typedef struct {
    int state;
    vaddr_t halt_pc;
    uint32_t halt_ret;
} NPCState;
NPCState npc_state;

void set_npc_state(int state) {
    npc_state.state = state;
    // npc_state.halt_pc = pc;
    // npc_state.halt_ret = halt_ret;
}

// #define NPCTRAP() set_npc_state(NPC_END)
void NPCTRAP() { set_npc_state(NPC_END); }

// 简单仿真内存，大小可根据需要调整
static uint32_t pmem[1024] = {
    0x00100093, // addi x1, x0, 1
    0x00208113, // addi x2, x1, 2
    0x00310193, // addi x3, x2, 3
    0x00410093, // addi x1, x2, 4
    0x00508113, // addi x2, x1, 5
    0x00610193, // addi x3, x2, 6
    0x00718193, // addi x3, x3, 7
    0x00100073, // ebreak
    // ...
};

// 仿真内存读取函数
uint32_t pmem_read(uint32_t pc) {
    // 假设pc按字对齐，且小于pmem大小
    return pmem[(pc - 0x80000000) >> 2];
}

#define MAX_SIM_TIME 30
vluint64_t sim_time = 0;
int main(int argc, char **argv) {
    contextp->commandArgs(argc, argv);
    contextp->traceEverOn(true);
    top->trace(tfp, 99);                      // Trace 99 levels of hierarchy
    tfp->open("build/obj_dir/Vtop_wave.fst"); // 打开波形文件
    reset(10);

    while (1) {
        top->inst = pmem_read(top->pc);
        single_cycle();
        sim_time++;
        // usleep(1000); // Sleep for 1ms
        if (sim_time >= MAX_SIM_TIME)
            break; // 达到最大仿真时间，退出循环
        if (npc_state.state == NPC_END) {
            printf("Simulation ended at PC: 0x%08x", top->pc);
            break; // 如果遇到结束状态，退出循环
        }
    }
    tfp->close();
    delete top;

    // 返回成功的完成状态
    return 0;
}