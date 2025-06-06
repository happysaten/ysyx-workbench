#include <memory/vaddr.h>
#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    FILE *fp = fopen(elf_file, "rb");
    if (!fp) {
        fprintf(stderr, "Failed to open ELF file: %s\n", elf_file);
        return -1;
    }

    Elf64_Ehdr ehdr;
    fread(&ehdr, 1, sizeof(ehdr), fp);

    // 读取节区头表
    fseek(fp, ehdr.e_shoff, SEEK_SET);
    Elf64_Shdr *sh_table = malloc(ehdr.e_shnum * sizeof(Elf64_Shdr));
    fread(sh_table, ehdr.e_shnum, sizeof(Elf64_Shdr), fp);

    // 读取节区名字符串表
    Elf64_Shdr sh_str = sh_table[ehdr.e_shstrndx];
    char *shstrtab = malloc(sh_str.sh_size);
    fseek(fp, sh_str.sh_offset, SEEK_SET);
    fread(shstrtab, 1, sh_str.sh_size, fp);

    // 找到.symtab和.strtab节
    Elf64_Shdr *symtab_hdr = NULL, *strtab_hdr = NULL;
    for (int i = 0; i < ehdr.e_shnum; i++) {
        const char *name = shstrtab + sh_table[i].sh_name;
        if (strcmp(name, ".symtab") == 0) symtab_hdr = &sh_table[i];
        if (strcmp(name, ".strtab") == 0) strtab_hdr = &sh_table[i];
    }
    if (!symtab_hdr || !strtab_hdr) {
        fprintf(stderr, "No .symtab or .strtab found\n");
        free(sh_table); free(shstrtab); fclose(fp);
        return -1;
    }

    // 读取符号表和字符串表
    Elf64_Sym *symtab = malloc(symtab_hdr->sh_size);
    fseek(fp, symtab_hdr->sh_offset, SEEK_SET);
    fread(symtab, 1, symtab_hdr->sh_size, fp);

    char *strtab = malloc(strtab_hdr->sh_size);
    fseek(fp, strtab_hdr->sh_offset, SEEK_SET);
    fread(strtab, 1, strtab_hdr->sh_size, fp);

    int sym_count = symtab_hdr->sh_size / sizeof(Elf64_Sym);
    sym_table.count = 0;
    sym_table.capacity = 32;
    sym_table.symbols = malloc(sym_table.capacity * sizeof(func_symbol_t));

    for (int i = 0; i < sym_count; i++) {
        if (ELF64_ST_TYPE(symtab[i].st_info) == STT_FUNC && symtab[i].st_size > 0 && symtab[i].st_name != 0) {
            if (sym_table.count >= sym_table.capacity) {
                sym_table.capacity *= 2;
                sym_table.symbols = realloc(sym_table.symbols, sym_table.capacity * sizeof(func_symbol_t));
            }
            func_symbol_t *fs = &sym_table.symbols[sym_table.count++];
            fs->name = strdup(strtab + symtab[i].st_name);
            fs->addr = symtab[i].st_value;
            fs->size = symtab[i].st_size;
        }
    }

    free(sh_table);
    free(shstrtab);
    free(symtab);
    free(strtab);
    fclose(fp);
    return 0;
};

// 根据地址查找函数名
const char *find_func_name(vaddr_t addr) {
    // printf("Searching for function at address " FMT_WORD "\n", addr);
    for (int i = 0; i < sym_table.count; i++) {
        printf("Checking symbol %s at " FMT_WORD " with size " FMT_WORD "\n",
               sym_table.symbols[i].name, sym_table.symbols[i].addr,
               sym_table.symbols[i].size);
        if (addr >= sym_table.symbols[i].addr &&
            addr < sym_table.symbols[i].addr + sym_table.symbols[i].size) {
            return sym_table.symbols[i].name;
        }
    }
    return NULL;
}