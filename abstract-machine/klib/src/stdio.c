#include <am.h>
#include <klib-macros.h>
#include <klib.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

// printf: 格式化输出到标准输出（未实现）
int printf(const char *fmt, ...) { panic("Not implemented"); }

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

// vsprintf: 格式化输出到字符串，参数通过va_list传递
// 原理：遍历格式字符串，遇到%时处理对应类型，支持%d/%u/%x/%c/%s/%%
int vsprintf(char *out, const char *fmt, va_list ap) {
    char *str = out;
    const char *p = fmt;
    char numbuf[32];

    while (*p) {
        if (*p != '%') {
            *str++ = *p++;
            continue;
        }
        p++; // 跳过'%'
        switch (*p) {
        case 'd': { // 十进制整数
            int val = va_arg(ap, int);
            itoa(val, numbuf, 10, 0);
            for (char *s = numbuf; *s; ++s)
                *str++ = *s;
            break;
        }
        case 'u': { // 无符号十进制
            unsigned int val = va_arg(ap, unsigned int);
            itoa(val, numbuf, 10, 1);
            for (char *s = numbuf; *s; ++s)
                *str++ = *s;
            break;
        }
        case 'x': { // 十六进制
            unsigned int val = va_arg(ap, unsigned int);
            itoa(val, numbuf, 16, 1);
            for (char *s = numbuf; *s; ++s)
                *str++ = *s;
            break;
        }
        case 'c': { // 字符
            char ch = (char)va_arg(ap, int);
            *str++ = ch;
            break;
        }
        case 's': { // 字符串
            const char *s = va_arg(ap, const char *);
            while (*s)
                *str++ = *s++;
            break;
        }
        case '%': { // 百分号
            *str++ = '%';
            break;
        }
        default:
            // 不支持的格式，原样输出
            *str++ = '%';
            *str++ = *p;
            break;
        }
        p++;
    }
    *str = '\0';
    return str - out;
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
