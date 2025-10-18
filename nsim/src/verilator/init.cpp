#include "../include/verilator.h"

// 全局变量定义
std::unique_ptr<VerilatedContext> context{new VerilatedContext};
Vtop *top = nullptr;
VerilatedFstC *tfp = nullptr;

// 初始化Verilator仿真环境
// trace_file: 指定波形文件路径（可选）
extern "C" void init_verilator(const char *trace_file) {
    top = new Vtop;
    tfp = new VerilatedFstC;

#ifdef CONFIG_WTRACE
    Verilated::traceEverOn(true); // 启用波形追踪
    top->trace(tfp, 99);          // 绑定波形对象，最大层级99
    tfp->open(trace_file == NULL ? "build/nsim-trace.fst"
                                 : trace_file); // 打开波形文件
#endif
}