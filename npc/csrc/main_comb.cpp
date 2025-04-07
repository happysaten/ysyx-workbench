#include "Vtop.h"
#include <nvboard.h>
#include <verilated.h>

void nvboard_bind_all_pins(Vtop *top);

void single_cycle(Vtop *top) {
    top->eval();
}

int main(int argc, char **argv) {
    const std::unique_ptr<VerilatedContext> contextp{new VerilatedContext};
    contextp->commandArgs(argc, argv);
    Vtop *top = new Vtop;

    // ...
    nvboard_bind_all_pins(top);
    nvboard_init(1);

    while (1) {
        // ...
        nvboard_update();
        single_cycle(top);
    }
    nvboard_quit();
    delete top;

    // 返回成功的完成状态
    return 0;
}