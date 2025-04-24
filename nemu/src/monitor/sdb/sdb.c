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
#include <cpu/cpu.h>
#include <isa.h>
#include <readline/history.h>
#include <readline/readline.h>

static int is_batch_mode = false; // 是否为批处理模式

void init_regex();      // 初始化正则表达式
void init_wp_pool();    // 初始化监视点池

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
    return -1; // 返回负值表示退出
}

/* 命令：单步执行 N 条指令 */
static int cmd_si(char *args) {
    int n = 1; // 默认执行 1 步
    if (args != NULL) {
        n = atoi(args); // 将参数转换为整数
        if (n <= 0) {
            printf("Invalid argument for 'si'. Please provide a positive number.\n");
            return 0;
        }
    }
    cpu_exec(n); // 执行 n 步
    return 0;
}

/* 命令：显示所有支持的命令 */
static int cmd_help(char *args);

/* 命令：信息查询 */
static int cmd_info(char *args) {
    if (args == NULL) {
        printf("Usage: info r\n");
        return 0;
    }

    if (strcmp(args, "r") == 0) {
        isa_reg_display(); // 调用API打印寄存器值
    } else {
        printf("Unknown argument '%s' for info\n", args);
    }
    return 0;
}

/* 命令表，包含命令名称、描述和处理函数 */
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
                printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
                return 0;
            }
        }
        printf("Unknown command '%s'\n", arg); // 未找到命令
    }
    return 0;
}

/* 设置批处理模式 */
void sdb_set_batch_mode() { is_batch_mode = true; }

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
    init_regex();    // 编译正则表达式
    init_wp_pool();  // 初始化监视点池
}
