#include <klib-macros.h>
#include <klib.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

// 计算字符串长度，不包括结尾的'\0'
// 原理：遍历字符串直到遇到'\0'，计数长度
size_t strlen(const char *s) {
    size_t len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

// 复制src字符串到dst，包括结尾的'\0'
// 原理：逐字符复制src到dst，直到遇到'\0'
char *strcpy(char *dst, const char *src) {
    char *ret = dst;
    while ((*dst++ = *src++) != '\0')
        ;
    return ret;
}

// 最多复制n个字符，从src到dst，若src不足n个字符则补'\0'
// 原理：先复制src的字符，剩余部分补'\0'直到n个字符
char *strncpy(char *dst, const char *src, size_t n) {
    char *ret = dst;
    size_t i = 0;
    while (i < n && src[i] != '\0') {
        dst[i] = src[i];
        i++;
    }
    while (i < n) {
        dst[i++] = '\0';
    }
    return ret;
}

// 将src字符串追加到dst字符串末尾
// 原理：先找到dst末尾，再逐字符复制src到dst末尾
char *strcat(char *dst, const char *src) {
    char *ret = dst;
    while (*dst)
        dst++;
    while ((*dst++ = *src++) != '\0')
        ;
    return ret;
}

// 比较两个字符串，返回差值
// 原理：逐字符比较，遇到不同或'\0'时返回差值
int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

// 比较两个字符串的前n个字符，返回差值
// 原理：逐字符比较前n个字符，遇到不同或'\0'时返回差值
int strncmp(const char *s1, const char *s2, size_t n) {
    size_t i = 0;
    for (; i < n; i++) {
        if (s1[i] != s2[i] || s1[i] == '\0' || s2[i] == '\0') {
            return (unsigned char)s1[i] - (unsigned char)s2[i];
        }
    }
    return 0;
}

// 将s的前n个字节设置为c
// 原理：遍历s的前n个字节，每个字节赋值为c
void *memset(void *s, int c, size_t n) {
    unsigned char *p = (unsigned char *)s;
    for (size_t i = 0; i < n; i++) {
        p[i] = (unsigned char)c;
    }
    return s;
}

// 内存安全地拷贝n个字节，从src到dst，支持重叠区域
// 原理：判断重叠关系，选择正序或逆序逐字节拷贝
void *memmove(void *dst, const void *src, size_t n) {
    unsigned char *d = (unsigned char *)dst;
    const unsigned char *s = (const unsigned char *)src;
    if (d == s || n == 0)
        return dst;
    if (d < s) {
        for (size_t i = 0; i < n; i++) {
            d[i] = s[i];
        }
    } else {
        for (size_t i = n; i > 0; i--) {
            d[i - 1] = s[i - 1];
        }
    }
    return dst;
}

// 拷贝n个字节，从in到out，不支持重叠区域
// 原理：逐字节从in复制到out
void *memcpy(void *out, const void *in, size_t n) {
    unsigned char *d = (unsigned char *)out;
    const unsigned char *s = (const unsigned char *)in;
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return out;
}

// 比较两块内存的前n个字节，返回差值
// 原理：逐字节比较，遇到不同字节时返回差值
int memcmp(const void *s1, const void *s2, size_t n) {
    const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *p2 = (const unsigned char *)s2;
    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] - p2[i];
        }
    }
    return 0;
}

#endif
