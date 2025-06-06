#include "common.h"
#include "utils.h"
#include <elf.h>
#include <memory/vaddr.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
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

// ELF符号表相关结构
typedef struct {
    char *name;   // 函数名
    vaddr_t addr; // 函数地址
    vaddr_t size; // 函数大小
} func_symbol_t;

typedef struct {
    func_symbol_t *symbols; // 符号数组
    int count;              // 符号数量
    int capacity;           // 数组容量
} symbol_table_t;

static symbol_table_t sym_table = {.symbols = NULL, .count = 0, .capacity = 0};

// 初始化ftrace功能，从ELF文件读取符号表
int init_ftrace(const char *elf_file) {
    if (elf_file == NULL) {
        printf("No ELF file specified for ftrace\n");
        return -1;
    }

    FILE *fp = fopen(elf_file, "rb");
    if (fp == NULL) {
        printf("Failed to open ELF file: %s\n", elf_file);
        return -1;
    }

    // 读取ELF头
    Elf64_Ehdr elf_header;
    if (fread(&elf_header, sizeof(elf_header), 1, fp) != 1) {
        printf("Failed to read ELF header\n");
        fclose(fp);
        return -1;
    }

    // 检查ELF魔数
    if (memcmp(elf_header.e_ident, ELFMAG, SELFMAG) != 0) {
        printf("Invalid ELF file\n");
        fclose(fp);
        return -1;
    }

    // 读取节头表
    Elf64_Shdr *section_headers =
        malloc(sizeof(Elf64_Shdr) * elf_header.e_shnum);
    if (!section_headers) {
        printf("Failed to allocate memory for section headers\n");
        fclose(fp);
        return -1;
    }

    fseek(fp, elf_header.e_shoff, SEEK_SET);
    if (fread(section_headers, sizeof(Elf64_Shdr), elf_header.e_shnum, fp) !=
        elf_header.e_shnum) {
        printf("Failed to read section headers\n");
        free(section_headers);
        fclose(fp);
        return -1;
    }

    // 查找字符串表和符号表
    Elf64_Shdr *symtab = NULL;
    Elf64_Shdr *strtab = NULL;
    for (int i = 0; i < elf_header.e_shnum; i++) {
        if (section_headers[i].sh_type == SHT_SYMTAB) {
            symtab = &section_headers[i];
        } else if (i == elf_header.e_shstrndx) {
            strtab = &section_headers[i];
        }
    }

    if (!symtab || !strtab) {
        printf("Symbol table or string table not found\n");
        free(section_headers);
        fclose(fp);
        return -1;
    }

    // 读取字符串表
    char *str_table = malloc(strtab->sh_size);
    if (!str_table) {
        printf("Failed to allocate memory for string table\n");
        free(section_headers);
        fclose(fp);
        return -1;
    }

    fseek(fp, strtab->sh_offset, SEEK_SET);
    if (fread(str_table, strtab->sh_size, 1, fp) != 1) {
        printf("Failed to read string table\n");
        free(str_table);
        free(section_headers);
        fclose(fp);
        return -1;
    }

    // 找到符号表的字符串表
    Elf64_Shdr *sym_strtab = &section_headers[symtab->sh_link];

    // 读取符号表的字符串表
    char *sym_str_table = malloc(sym_strtab->sh_size);
    if (!sym_str_table) {
        printf("Failed to allocate memory for symbol string table\n");
        free(str_table);
        free(section_headers);
        fclose(fp);
        return -1;
    }

    fseek(fp, sym_strtab->sh_offset, SEEK_SET);
    if (fread(sym_str_table, sym_strtab->sh_size, 1, fp) != 1) {
        printf("Failed to read symbol string table\n");
        free(sym_str_table);
        free(str_table);
        free(section_headers);
        fclose(fp);
        return -1;
    }

    // 读取符号表
    int sym_count = symtab->sh_size / symtab->sh_entsize;
    Elf64_Sym *syms = malloc(symtab->sh_size);
    if (!syms) {
        printf("Failed to allocate memory for symbols\n");
        free(sym_str_table);
        free(str_table);
        free(section_headers);
        fclose(fp);
        return -1;
    }

    fseek(fp, symtab->sh_offset, SEEK_SET);
    if (fread(syms, symtab->sh_size, 1, fp) != 1) {
        printf("Failed to read symbols\n");
        free(syms);
        free(sym_str_table);
        free(str_table);
        free(section_headers);
        fclose(fp);
        return -1;
    }

    // 初始化符号表
    sym_table.capacity = sym_count;
    sym_table.symbols = malloc(sizeof(func_symbol_t) * sym_count);
    if (!sym_table.symbols) {
        printf("Failed to allocate memory for symbol table\n");
        free(syms);
        free(sym_str_table);
        free(str_table);
        free(section_headers);
        fclose(fp);
        return -1;
    }

    // 提取函数符号（类型为FUNC且大小大于0）
    for (int i = 0; i < sym_count; i++) {
        if (ELF64_ST_TYPE(syms[i].st_info) == STT_FUNC && syms[i].st_size > 0) {
            sym_table.symbols[sym_table.count].name =
                strdup(sym_str_table + syms[i].st_name);
            sym_table.symbols[sym_table.count].addr = syms[i].st_value;
            sym_table.symbols[sym_table.count].size = syms[i].st_size;
            sym_table.count++;
        }
    }

    printf("Loaded %d function symbols from %s\n", sym_table.count, elf_file);

    // 清理资源
    free(syms);
    free(sym_str_table);
    free(str_table);
    free(section_headers);
    fclose(fp);
    return 0;
}

// 根据地址查找函数名
const char *find_func_name(vaddr_t addr) {
    for (int i = 0; i < sym_table.count; i++) {
        if (addr >= sym_table.symbols[i].addr &&
            addr < sym_table.symbols[i].addr + sym_table.symbols[i].size) {
            return sym_table.symbols[i].name;
        }
    }
    return NULL;
}

// 清理符号表资源
void cleanup_ftrace() {
    if (sym_table.symbols) {
        for (int i = 0; i < sym_table.count; i++) {
            if (sym_table.symbols[i].name) {
                free(sym_table.symbols[i].name);
            }
        }
        free(sym_table.symbols);
        sym_table.symbols = NULL;
    }
    sym_table.count = 0;
    sym_table.capacity = 0;
}

// 函数调用最大嵌套深度
#define FTRACE_MAX_DEPTH 256

// 记录当前函数调用深度
static int call_depth = 0;

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