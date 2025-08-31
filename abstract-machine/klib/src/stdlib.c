#include <am.h>
#include <klib-macros.h>
#include <klib.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
static unsigned long int next = 1;
// 维护上次分配内存的位置，初始化为堆区起始位置
static char *addr = NULL;

int rand(void) {
    // RAND_MAX assumed to be 32767
    next = next * 1103515245 + 12345;
    return (unsigned int)(next / 65536) % 32768;
}

void srand(unsigned int seed) { next = seed; }

int abs(int x) { return (x < 0 ? -x : x); }

int atoi(const char *nptr) {
    int x = 0;
    while (*nptr == ' ') {
        nptr++;
    }
    while (*nptr >= '0' && *nptr <= '9') {
        x = x * 10 + *nptr - '0';
        nptr++;
    }
    return x;
}

void *malloc(size_t size) {
    // On native, malloc() will be called during initializaion of C runtime.
    // Therefore do not call panic() here, else it will yield a dead recursion:
    //   panic() -> putchar() -> (glibc) -> malloc() -> panic()
    // 如果是第一次调用malloc，初始化addr为堆区起始位置
    if (addr == NULL) {
        addr = (char *)heap.start;
    }

    // 将size向上对齐到8字节边界，满足malloc对地址对齐的要求
    size = (size_t)ROUNDUP(size, 8);

    // 保存当前分配位置
    char *old = addr;

    // 更新下次分配位置
    addr += size;

    // 检查是否超出堆区范围
    assert((uintptr_t)heap.start <= (uintptr_t)addr &&
           (uintptr_t)addr < (uintptr_t)heap.end);

    // 将分配的内存初始化为0
    for (uint64_t *p = (uint64_t *)old; p != (uint64_t *)addr; p++) {
        *p = 0;
    }

    // 返回分配的内存起始地址
    return old;
}

void free(void *ptr) {}

#endif
