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

#include <isa.h>

/* 我们使用POSIX正则表达式函数来处理正则表达式。
 * 输入'man regex'以获取有关POSIX正则表达式函数的更多信息。
 */
#include <regex.h>

enum {
    TK_NOTYPE = 256, // 无类型
    TK_EQ,           // 等于
    TK_NUM,          // 数字
    TK_HEX,          // 十六进制
    TK_REG,          // 寄存器
    TK_NEG,          // 负号
    /* TODO: 添加更多的token类型 */
};

static struct rule {
    const char *regex; // 正则表达式
    int token_type;    // token类型
} rules[] = {
    {" +", TK_NOTYPE},                   // 空格
    {"\\+", '+'},                        // 加号
    {"-", '-'},                          // 减号
    {"\\*", '*'},                        // 乘号
    {"/", '/'},                          // 除号
    {"\\(", '('},                        // 左括号
    {"\\)", ')'},                        // 右括号
    {"0[xX][0-9a-fA-F]+", TK_HEX},       // 十六进制数字
    {"[0-9]+", TK_NUM},                  // 十进制数字
    {"\\$[a-zA-Z][a-zA-Z0-9]*", TK_REG}, // 寄存器
    {"==", TK_EQ},                       // 等于
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* 规则会被多次使用。
 * 因此我们在第一次使用前只编译一次。
 */
void init_regex() {
    int i;
    char error_msg[128];
    int ret;

    for (i = 0; i < NR_REGEX; i++) {
        ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
        if (ret != 0) {
            regerror(ret, &re[i], error_msg, 128);
            panic("regex compilation failed: %s\n%s", error_msg,
                  rules[i].regex);
        }
    }
}

typedef struct token {
    int type;     // token类型
    char str[32]; // token字符串
} Token;

static Token tokens[256] __attribute__((used)) = {}; // 存储token的数组
static int nr_token __attribute__((used)) = 0;       // token数量

/* 解析表达式并生成token */
static bool make_token(char *e) {
    int position = 0;
    int i;
    regmatch_t pmatch;

    nr_token = 0;

    while (e[position] != '\0') {
        /* 逐个尝试所有规则 */
        for (i = 0; i < NR_REGEX; i++) {
            if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 &&
                pmatch.rm_so == 0) {
                char *substr_start = e + position;
                int substr_len = pmatch.rm_eo;

                // 日志记录匹配
                // Log("match rules[%d] = \"%s\" at position %d with len %d:
                // %.*s",
                //     i, rules[i].regex, position, substr_len, substr_len,
                //     substr_start);

                position += substr_len;

                /* 记录token */
                if (rules[i].token_type != TK_NOTYPE) { // 忽略空格
                    tokens[nr_token].type = rules[i].token_type;
                    if (substr_len < sizeof(tokens[nr_token].str)) {
                        strncpy(tokens[nr_token].str, substr_start, substr_len);
                        tokens[nr_token].str[substr_len] =
                            '\0'; // 字符串以空字符结尾
                    } else {
                        printf("Token too long at position %d\n", position);
                        return false;
                    }

                    // 处理负号
                    if (tokens[nr_token].type == '-' &&
                        (nr_token == 0 || tokens[nr_token - 1].type == '(' ||
                         tokens[nr_token - 1].type == '+' ||
                         tokens[nr_token - 1].type == '-' ||
                         tokens[nr_token - 1].type == '*' ||
                         tokens[nr_token - 1].type == '/' ||
                         tokens[nr_token - 1].type == TK_EQ)) {
                        tokens[nr_token].type = TK_NEG; // 将减号解释为负号
                    }

                    nr_token++;
                }

                break;
            }
        }

        if (i == NR_REGEX) {
            printf("no match at position %d\n%s\n%*.s^\n", position, e,
                   position, "");
            return false;
        }
    }

    return true;
}

/* 检查表达式是否被一对匹配的括号包围 */
static bool check_parentheses(int p, int q) {
    if (tokens[p].type != '(' || tokens[q].type != ')') {
        return false; // 首尾不是括号，直接返回 false
    }

    int balance = 0;
    for (int i = p; i <= q; i++) {
        if (tokens[i].type == '(') {
            balance++;
        } else if (tokens[i].type == ')') {
            balance--;
        }

        if (balance < 0) {
            return false; // 如果在遍历过程中右括号多于左括号，返回 false
        }
    }

    if (balance != 0) {
        return false; // 如果最终左右括号数量不匹配，返回 false
    }

    // 检查最外层括号是否匹配
    balance = 0;
    for (int i = p + 1; i < q; i++) {
        if (tokens[i].type == '(') {
            balance++;
        } else if (tokens[i].type == ')') {
            balance--;
        }

        if (balance < 0) {
            return false; // 如果在最外层括号内右括号多于左括号，返回 false
        }
    }

    return balance == 0; // 如果最外层括号内匹配，返回 true
}

/* 在表达式中寻找主运算符 */
static int find_main_operator(int p, int q) {
    int op = -1;
    int min_priority = 100; // 假设优先级最大为100
    int balance = 0;

    for (int i = p; i <= q; i++) {
        if (tokens[i].type == '(')
            balance++;
        else if (tokens[i].type == ')')
            balance--;
        else if (balance == 0) { // 不在括号内
            int priority;
            switch (tokens[i].type) {
            case '+':
            case '-':
                priority = 1;
                break;
            case '*':
            case '/':
                priority = 2;
                break;
            default:
                continue;
            }
            if (priority <= min_priority) {
                min_priority = priority;
                op = i;
            }
        }
    }
    return op;
}

/* 递归求值函数 */
static word_t eval(int p, int q, bool *success) {
    if (p > q) {
        *success = false; // 表达式非法
        printf("Error: Invalid expression (p > q).\n");
        return 0;
    } else if (p == q) {
        // 单个token，必须是数字或十六进制
        if (tokens[p].type == TK_NUM) {
            return strtoul(tokens[p].str, NULL, 10);
        } else if (tokens[p].type == TK_HEX) {
            return strtoul(tokens[p].str, NULL, 16);
        } else {
            *success = false;
            printf("Error: Invalid token '%s'.\n", tokens[p].str);
            return 0;
        }
    } else if (check_parentheses(p, q)) {
        // 去掉括号后递归求值
        return eval(p + 1, q - 1, success);
    } else {
        int op = find_main_operator(p, q);
        if (op == -1) {
            *success = false;
            printf("Error: No valid operator found in expression.\n");
            return 0;
        }

        if (tokens[op].type == TK_NEG) {
            // 处理负号
            word_t val = eval(op + 1, q, success);
            if (!*success)
                return 0;
            return -val;
        }

        word_t val1 = eval(p, op - 1, success);
        if (!*success)
            return 0;
        word_t val2 = eval(op + 1, q, success);
        if (!*success)
            return 0;

        switch (tokens[op].type) {
        case '+':
            return val1 + val2;
        case '-':
            return val1 - val2;
        case '*':
            return val1 * val2;
        case '/':
            if (val2 == 0) {
                *success = false; // 除零错误
                printf("Error: Division by zero.\n");
                return 0;
            }
            return val1 / val2;
        default:
            *success = false;
            printf("Error: Unsupported operator '%c'.\n", tokens[op].type);
            return 0;
        }
    }
}

/* 表达式求值接口 */
word_t expr(char *e, bool *success) {
    *success = true;
    if (!make_token(e)) {
        *success = false;
        printf("Error: Failed to tokenize expression '%s'.\n", e);
        return 0;
    }
    return eval(0, nr_token - 1, success);
}
