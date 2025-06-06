#include "common.h"

#ifdef CONFIG_ITRACE
#define IRINGBUF_SIZE 16

typedef struct {
    char itrace[128];
} iringbuf_entry_t;

typedef struct {
    iringbuf_entry_t buf[IRINGBUF_SIZE];
    int head;
    int count;
} iringbuf_t;

static iringbuf_t iringbuf = {.head = 0, .count = 0};

// 写入一条trace
void iringbuf_push(const char itrace_str[128]) {
    memcpy(iringbuf.buf[iringbuf.head].itrace, itrace_str, 128);
    iringbuf.head = (iringbuf.head + 1) % IRINGBUF_SIZE;
    if (iringbuf.count < IRINGBUF_SIZE) {
        iringbuf.count++;
    }
}

// 打印环形缓冲区内容，出错指令用'-->'标记
void iringbuf_dump() {
    printf("Recent instruction trace (iringbuf):\n");
    int start =
        (iringbuf.head + IRINGBUF_SIZE - iringbuf.count) % IRINGBUF_SIZE;
    int bad_idx = iringbuf.count - 1; // 最新指令
    for (int i = 0; i < iringbuf.count; i++) {
        int idx = (start + i) % IRINGBUF_SIZE;
        if (i == bad_idx) {
            printf("--> ");
        } else {
            printf("    ");
        }
        printf("%s\n", iringbuf.buf[idx].itrace);
    }
}
#endif

#ifdef CONFIG_FTRACE
// 函数调用最大嵌套深度
#define FTRACE_MAX_DEPTH 256

// 记录当前函数调用深度
static int call_depth = 0;

const char *find_func_name(vaddr_t addr);

// 追踪函数调用
void ftrace_call(vaddr_t pc, vaddr_t target) {
    const char *func_name = find_func_name(target);
    if (func_name != NULL) {
        // 生成缩进
        char indent[FTRACE_MAX_DEPTH * 2 + 1] = {0};
        for (int i = 0; i < call_depth; i++) {
            strcat(indent, "  ");
        }
        // 记录函数调用
        log_write(FMT_WORD ": %scall [%s@" FMT_WORD "]\n", pc, indent,
                  func_name, target);

        // 增加调用深度
        if (call_depth < FTRACE_MAX_DEPTH - 1) {
            call_depth++;
        }
    } else
        log_write(FMT_WORD ": call [unknown function @ " FMT_WORD "]\n", pc,
                  target);
}

// 追踪函数返回
void ftrace_ret(vaddr_t pc, vaddr_t target) {
    const char *func_name = find_func_name(target);
    if (func_name != NULL) {
        // 减少调用深度
        if (call_depth > 0) {
            call_depth--;
        }
        // 生成缩进
        char indent[FTRACE_MAX_DEPTH * 2 + 1] = {0};
        for (int i = 0; i < call_depth; i++) {
            strcat(indent, "  ");
        }
        // 记录函数返回
        log_write(FMT_WORD ": %sret [%s@" FMT_WORD "]\n", pc, indent, func_name,
                  target);
    }
}
#endif