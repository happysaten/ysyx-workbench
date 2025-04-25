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

#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format = "#include <stdio.h>\n"
                           "int main() { "
                           "  unsigned result = %s; "
                           "  printf(\"%%u\", result); "
                           "  return 0; "
                           "}";

// 生成一个小于n的随机数
uint32_t choose(uint32_t n) {
    uint32_t num = rand() % n;
    return num;
}

// 将字符追加到 buf 中
static void gen(char c) {
    int len = strlen(buf);
    if (len < sizeof(buf) - 1) { // 确保不会溢出
        buf[len] = c;
        buf[len + 1] = '\0';
    }
}

// 随机插入空格
static void gen_space() {
    int spaces = choose(2); // 随机生成0到1个空格
    for (int i = 0; i < spaces; i++) {
        gen(' ');
    }
}

// 生成一个随机数字
static void gen_num() {
    int len = choose(8) + 1; // 随机生成1到8位的数字长度
    for (int i = 0; i < len; i++) {
        char digit;
        if (i == 0) {
            digit = '1' + choose(9); // 确保首位数字为 '1' 到 '9'
        } else {
            digit = '0' + choose(10); // 随机生成字符 '0' 到 '9'
        }
        gen(digit); // 追加到 buf 中
    }
    gen('u'); // 确保 buf 中的数字是无符号的
}

// 生成一个随机运算符
static void gen_rand_op() {
    switch (choose(4)) {
    case 0:
        gen('+');
        break; // 加法
    case 1:
        gen('-');
        break; // 减法
    case 2:
        gen('*');
        break; // 乘法
    case 3:
        gen('/');
        break; // 除法
    }
}

// 生成随机表达式
static void gen_rand_expr() {
    if (strlen(buf) > 1000) {
        gen_num(); // 如果buf长度过长，直接生成数字
        return;
    }
    gen_space(); // 随机插入空格
    switch (choose(3)) {
    case 0:
        gen_num();
        break; // 生成一个数字
    case 1:
        gen('(');
        gen_rand_expr();
        gen(')');
        break; // 生成带括号的表达式
    default:
        gen_rand_expr();
        gen_rand_op();
        gen_rand_expr();
        break; // 生成两个子表达式并用运算符连接
    }
}

int main(int argc, char *argv[]) {
    int seed = time(0);
    srand(seed);
    int loop = 1;
    if (argc > 1) {
        sscanf(argv[1], "%d", &loop);
    }
    int i;
    for (i = 0; i < loop; i++) {
        buf[0] = '\0';   // 清空buf
        gen_rand_expr(); // 生成随机表达式

        sprintf(code_buf, code_format, buf); // 将表达式嵌入到代码中

        FILE *fp = fopen("/tmp/.code.c", "w");
        assert(fp != NULL);
        fputs(code_buf, fp);
        fclose(fp);

        int ret = system("gcc /tmp/.code.c -o /tmp/.expr"); // 编译生成的代码
        if (ret != 0)
            continue;

        fp = popen("/tmp/.expr", "r"); // 执行生成的程序
        assert(fp != NULL);

        int result;
        int status;
        ret = fscanf(fp, "%d", &result); // 获取程序输出的结果
        status = pclose(fp); // 关闭程序并获取其退出状态

        // 检查程序是否正常退出
        if (WIFEXITED(status)) {
            // 如果退出状态为 136，表示发生了除零操作
            if (WEXITSTATUS(status) == 136)
                continue; // 跳过该表达式
        }

        printf("%s = %u\n", buf, result); // 修改输出格式为“表达式 = 结果”
    }
    return 0;
}
