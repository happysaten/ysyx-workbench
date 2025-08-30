#include <am.h>
#include <klib-macros.h>
#include <klib.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

// 输出函数类型定义
typedef void (*output_callback_t)(char ch, void *ctx);

// 输出到字符串的上下文
typedef struct {
    char *str;
} sprintf_ctx_t;

// 输出到字符串的回调函数
static void sprintf_output(char ch, void *ctx) {
    sprintf_ctx_t *sctx = (sprintf_ctx_t *)ctx;
    *(sctx->str)++ = ch;
}

// 输出到标准输出的回调函数
static void printf_output(char ch, void *ctx) {
    putch(ch);
}

// itoa: 整数转字符串，支持十进制和十六进制
// 原理：将整数按指定进制转换为字符串，支持负数和无符号
static char *itoa(int value, char *buf, int base, int is_unsigned) {
    char *p = buf, *p1 = buf, tmp_char;
    int tmp_value;
    unsigned int uvalue = (unsigned int)value;

    if (base == 10 && !is_unsigned && value < 0) {
        *p++ = '-';
        uvalue = (unsigned int)(-value);
    }

    do {
        tmp_value = uvalue % base;
        *p++ = (tmp_value < 10) ? (tmp_value + '0') : (tmp_value - 10 + 'a');
        uvalue /= base;
    } while (uvalue);

    *p = '\0';

    // 反转数字部分
    if (buf[0] == '-')
        p1++;
    char *p2 = p - 1;
    while (p1 < p2) {
        tmp_char = *p1;
        *p1 = *p2;
        *p2 = tmp_char;
        p1++;
        p2--;
    }
    return buf;
}

// 通用的格式化函数
static int vprintf_helper(output_callback_t output, void *ctx, const char *fmt, va_list ap) {
    const char *p = fmt;
    char numbuf[32];
    int count = 0;

    while (*p) {
        if (*p != '%') {
            output(*p++, ctx);
            count++;
            continue;
        }
        p++; // 跳过'%'
        switch (*p) {
        case 'd': { // 十进制整数
            int val = va_arg(ap, int);
            itoa(val, numbuf, 10, 0);
            for (char *s = numbuf; *s; ++s) {
                output(*s, ctx);
                count++;
            }
            break;
        }
        case 'u': { // 无符号十进制
            unsigned int val = va_arg(ap, unsigned int);
            itoa(val, numbuf, 10, 1);
            for (char *s = numbuf; *s; ++s) {
                output(*s, ctx);
                count++;
            }
            break;
        }
        case 'x': { // 十六进制
            unsigned int val = va_arg(ap, unsigned int);
            itoa(val, numbuf, 16, 1);
            for (char *s = numbuf; *s; ++s) {
                output(*s, ctx);
                count++;
            }
            break;
        }
        case 'c': { // 字符
            char ch = (char)va_arg(ap, int);
            output(ch, ctx);
            count++;
            break;
        }
        case 's': { // 字符串
            const char *s = va_arg(ap, const char *);
            while (*s) {
                output(*s++, ctx);
                count++;
            }
            break;
        }
        case '%': { // 百分号
            output('%', ctx);
            count++;
            break;
        }
        default:
            // 不支持的格式，原样输出
            output('%', ctx);
            output(*p, ctx);
            count += 2;
            break;
        }
        p++;
    }
    return count;
}

// printf: 格式化输出到标准输出
// 原理：使用va_list宏处理可变参数，调用vprintf_helper
int printf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int ret = vprintf_helper(printf_output, NULL, fmt, ap);
    va_end(ap);
    return ret;
}

// vsprintf: 格式化输出到字符串，参数通过va_list传递
// 原理：调用vprintf_helper，最后添加字符串结束符
int vsprintf(char *out, const char *fmt, va_list ap) {
    sprintf_ctx_t ctx = { .str = out };
    int ret = vprintf_helper(sprintf_output, &ctx, fmt, ap);
    *ctx.str = '\0';
    return ret;
}

// sprintf: 格式化输出到字符串，参数可变
// 原理：使用va_list宏转发到vsprintf
int sprintf(char *out, const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int ret = vsprintf(out, fmt, ap);
    va_end(ap);
    return ret;
}

// snprintf: 限制长度的格式化输出（未实现）
int snprintf(char *out, size_t n, const char *fmt, ...) {
    panic("Not implemented");
}

// vsnprintf: 限制长度的格式化输出（未实现）
int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
    panic("Not implemented");
}

#endif
