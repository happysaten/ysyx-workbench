#include <stdint.h>
#include <stdio.h>
#include <string.h>

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
    int start = (iringbuf.head + IRINGBUF_SIZE - iringbuf.count) % IRINGBUF_SIZE;
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