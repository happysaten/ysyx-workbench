// 为了使用 std::unique_ptr
#include <memory>

// 包含通用例程
#include <verilated.h>
#include <verilated_fst_c.h>

// 包含由Verilator "top.v"生成的模型头文件
#include "Vtop.h"

#define MAX_SIM_TIME 20
vluint64_t sim_time = 0;

int main(int argc, char **argv) {
    // 创建一个更加复杂的示例，请参考examples/make_hello_c中的简单例子。

    // 构造一个VerilatedContext来保存仿真时间等信息。
    // 多个模块（如下文创建的Vtop）可以共享同一个contextp以共享时间，
    // 或者每个模块都有自己的contextp如果它们应该相互独立的话。

    // 使用unique_ptr类似于 "VerilatedContext* contextp = new VerilatedContext;"
    // 然后在结束时删除它。
    const std::unique_ptr<VerilatedContext> contextp{new VerilatedContext};

    // 传递参数给Verilated代码，例如$value$plusargs
    // 这需要在创建任何模型之前调用
    contextp->commandArgs(argc, argv);
    contextp->traceEverOn(true);

    // 根据Verilator从"top.v"生成的Vtop.h构造Verilated模型。
    // 使用unique_ptr类似于 "Vtop* top = new Vtop;" 然后在结束时删除它。
    const std::unique_ptr<Vtop> top{new Vtop{contextp.get(), "TOP"}};
    VerilatedFstC *tfp = new VerilatedFstC;
    top->trace(tfp, 99); // Trace 99 levels of hierarchy
    tfp->open("obj_dir/Vtop_wave.fst"); // 打开波形文件

    // 设置Vtop的输入信号
    top->rst_n = !0;
    top->clk = 0;

    // 模拟直到遇到$finish
    while (sim_time < MAX_SIM_TIME) {
        contextp->timeInc(1); // 增加1个时间单位

        // 切换时钟信号
        top->clk = !top->clk;

        // 在时钟下降沿更新控制信号
        if (!top->clk) {
            if (contextp->time() > 1 && contextp->time() < 10) {
                top->rst_n = !1; // 断言复位
            } else {
                top->rst_n = !0; // 取消复位
            }
        }

        // 评估模型状态变化
        top->eval();
        tfp->dump(contextp->time());
        printf("time: %lu, output: %d\n", contextp->time(), top->out); // 打印输出信号
        sim_time++;
    }
    tfp->close();
    // 最终模型清理
    top->final();

    // 返回成功的完成状态
    return 0;
}