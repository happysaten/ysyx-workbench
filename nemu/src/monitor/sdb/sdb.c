/***************************************************************************************
 * Copyright (c) 2014-2024 Zihao Yu, Nanjing University
 *
 * NEMU is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan
 *PSL v2. You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY
 *KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
 *NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 *
 * See the Mulan PSL v2 for more details.
 ***************************************************************************************/

#include "sdb.h"
#include "debug.h"
#include "memory/vaddr.h"
#include <cpu/cpu.h>
#include <isa.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>

static bool is_batch_mode = false; // 是否为批处理模式

/* 使用 `readline` 库从标准输入读取命令 */
static char *rl_gets() {
    static char *line_read = NULL;

    // 释放之前分配的内存
    if (line_read) {
        free(line_read);
        line_read = NULL;
    }

    // 提示符并读取输入
    line_read = readline("(nemu) ");

    // 如果输入非空，将其添加到历史记录中
    if (line_read && *line_read) {
        add_history(line_read);
    }

    return line_read;
}

/* 命令：继续执行程序 */
static int cmd_c(char *args) {
    cpu_exec(-1); // 执行程序直到暂停或结束
    return 0;
}

/* 命令：退出 NEMU */
static int cmd_q(char *args) {
    nemu_state.state = NEMU_QUIT; // 设置状态为退出
    return -1;                    // 返回负值表示退出
}

/* 命令：单步执行 N 条指令 */
static int cmd_si(char *args) {
    int n = 1; // 默认执行 1 步
    if (args != NULL) {
        n = atoi(args); // 将参数转换为整数
        if (n <= 0) {
            printf("Invalid argument for 'si'. Please provide a positive "
                   "number.\n");
            return 0;
        }
    }
    cpu_exec(n); // 执行 n 步
    return 0;
}

/* 命令：显示所有支持的命令 */
static int cmd_help(char *args);

/* 命令：处理设置监视点的命令 */
static int cmd_w(char *args) {
    if (args == NULL) {
        printf("Usage: w EXPR - set watchpoint for expression EXPR\n");
        return 0;
    }
    set_watchpoint(args);
    return 0;
}

/* 命令：处理删除监视点的命令 */
static int cmd_d(char *args) {
    if (args == NULL) {
        printf("Usage: d NUM - delete watchpoint with number NUM\n");
        return 0;
    }

    int num = atoi(args);
    if (delete_watchpoint(num)) {
        printf("Deleted watchpoint %d\n", num);
    } else {
        printf("No watchpoint with number %d\n", num);
    }
    return 0;
}

/* 命令：信息查询 */
static int cmd_info(char *args) {
    if (args == NULL) {
        printf("Usage: info r - print register values\n");
        printf("       info w - print watchpoint information\n");
        return 0;
    }

    if (args[0] == 'r') {
        isa_reg_display(); // 调用API打印寄存器值
    } else if (args[0] == 'w') {
        print_watchpoints();
    } else {
        printf("Unknown info command: '%s'\n", args);
    }
    return 0;
}

static int cmd_x(char *args) {
    if (args == NULL) {
        printf("Usage: x N EXPR\n");
        return 0;
    }

    // 提取 N 和 EXPR
    char *n_str = strtok(args, " ");
    char *expr_str = strtok(NULL, " ");
    if (n_str == NULL || expr_str == NULL) {
        printf("Usage: x N EXPR\n");
        return 0;
    }

    // 将 N 转换为整数
    int n = atoi(n_str);
    if (n <= 0) {
        printf("Invalid N. Please provide a positive number.\n");
        return 0;
    }

    // 将 EXPR 转换为地址（假设 EXPR 是十六进制数）
    vaddr_t addr = 0;
    if (sscanf(expr_str, "%x", &addr) != 1) {
        printf("Invalid EXPR. Please provide a valid hexadecimal address.\n");
        return 0;
    }

    // 扫描内存并打印
    for (int i = 0; i < n; i++) {
        if (i % 4 == 0) {
            printf("0x%08x: ", addr + i * 4); // 每 4 个字打印地址
        }
        printf("0x%08x ", vaddr_read(addr + i * 4, 4)); // 读取并打印 4 字节
        if ((i + 1) % 4 == 0) {
            printf("\n"); // 每 4 个字换行
        }
    }
    if (n % 4 != 0) {
        printf("\n"); // 如果最后一行不足 4 个字，手动换行
    }

    return 0;
}

/* 命令：测试表达式求值 */
static int cmd_test(char *args) {
    char *test_file =
        "/home/saten/Code/Other/ysyx-workbench/nemu/tools/gen-expr/build/input";
    FILE *fp = fopen(test_file, "r");
    if (fp == NULL) {
        printf("Failed to open test file: %s\n", test_file);
        return 0;
    }

    char buf[65536];
    unsigned expected_result;
    bool success;

    int total_tests = 0;
    int passed_tests = 0;

    while (fgets(buf, sizeof(buf), fp) != NULL) {
        char *equals_sign = strrchr(buf, '='); // 查找最后一个 '='
        if (equals_sign == NULL) {
            printf("Invalid test case format: %s\n", buf);
            continue;
        }

        *equals_sign = '\0'; // 将最后一个 '=' 替换为字符串结束符
        expected_result = atoi(equals_sign + 1); // 提取期望结果

        total_tests++;
        word_t result = expr(buf, &success);
        if (!success) {
            Log("Test failed: %s = %u (failed to evaluate expression)\n", buf,
                result);
        } else if (result != expected_result) {
            Log("Test failed: %s = %u (expected %u)\n", buf, result,
                expected_result);
        } else {
            Log("Test passed: %s = %u\n", buf, result);
            passed_tests++;
        }
    }

    fclose(fp);

    // 打印统计信息
    printf("Tests: %d, Passed: %d\n", total_tests, passed_tests);

    return 0;
}

/* 命令：求表达式 EXPR 的值 */
static int cmd_p(char *args) {
    if (args == NULL) {
        printf("Usage: p EXPR\n");
        return 0;
    }

    bool success;
    word_t result = expr(args, &success);
    if (!success) {
        printf("Failed to evaluate expression: %s\n", args);
    } else {
        printf("Result: %u (0x%x)\n", result, result);
    }

    return 0;
}

static struct {
    const char *name;        // 命令名称
    const char *description; // 命令描述
    int (*handler)(char *);  // 命令处理函数
} cmd_table[] = {
    {"help", "Display information about all supported commands", cmd_help},
    {"c", "Continue the execution of the program", cmd_c},
    {"q", "Exit NEMU", cmd_q},
    {"si", "Step through N instructions (default 1)", cmd_si},
    {"info", "Print information (e.g., info r for registers)", cmd_info},
    {"x", "Examine memory: x N EXPR", cmd_x},
    {"test", "Test expression evaluation", cmd_test}, // 添加 test 命令
    {"p", "Evaluate the value of an expression", cmd_p},
    {"w", "Set a watchpoint", cmd_w},
    {"d", "Delete a watchpoint", cmd_d},

    /* TODO: Add more commands */
};

#define NR_CMD ARRLEN(cmd_table) // 命令数量

/* 命令：显示帮助信息 */
static int cmd_help(char *args) {
    char *arg = strtok(NULL, " "); // 提取第一个参数
    int i;

    if (arg == NULL) {
        // 如果没有参数，显示所有命令
        for (i = 0; i < NR_CMD; i++) {
            printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        }
    } else {
        // 查找并显示指定命令的信息
        for (i = 0; i < NR_CMD; i++) {
            if (strcmp(arg, cmd_table[i].name) == 0) {
                printf("%s - %s\n", cmd_table[i].name,
                       cmd_table[i].description);
                return 0;
            }
        }
        printf("Unknown command '%s'\n", arg); // 未找到命令
    }
    return 0;
}

/* 设置批处理模式 */
// void sdb_set_batch_mode() { is_batch_mode = true; }
void sdb_set_batch_mode() { printf("Just for debugging, batch mode is not supported yet.\n"); 
    is_batch_mode = false; }

/* 主循环：处理用户输入的命令 */
void sdb_mainloop() {
    if (is_batch_mode) {
        cmd_c(NULL); // 如果是批处理模式，直接执行程序
        return;
    }

    for (char *str; (str = rl_gets()) != NULL;) {
        char *str_end = str + strlen(str);

        // 提取命令
        char *cmd = strtok(str, " ");
        if (cmd == NULL) {
            continue; // 如果没有输入命令，继续下一次循环
        }

        // 提取命令参数
        char *args = cmd + strlen(cmd) + 1;
        if (args >= str_end) {
            args = NULL; // 如果没有参数，设置为 NULL
        }

#ifdef CONFIG_DEVICE
        extern void sdl_clear_event_queue();
        sdl_clear_event_queue(); // 清空事件队列（如果启用设备配置）
#endif

        // 查找并执行命令
        int i;
        for (i = 0; i < NR_CMD; i++) {
            if (strcmp(cmd, cmd_table[i].name) == 0) {
                if (cmd_table[i].handler(args) < 0) {
                    return; // 如果命令返回负值，退出主循环
                }
                break;
            }
        }

        if (i == NR_CMD) {
            printf("Unknown command '%s'\n", cmd); // 未找到命令
        }
    }
}

/* 初始化调试器 */
void init_sdb() {
    init_regex();   // 编译正则表达式
    init_wp_pool(); // 初始化监视点池
}
