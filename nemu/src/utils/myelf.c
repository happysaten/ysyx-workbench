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

    // 使用32位ELF结构
    Elf32_Ehdr ehdr;
    if (fread(&ehdr, 1, sizeof(ehdr), fp) != sizeof(ehdr)) {
        fprintf(stderr, "Failed to read ELF header\n");
        fclose(fp);
        return -1;
    }

    // 验证ELF头
    if (memcmp(ehdr.e_ident, ELFMAG, SELFMAG) != 0) {
        fprintf(stderr, "Not a valid ELF file\n");
        fclose(fp);
        return -1;
    }

    printf("Reading ELF sections: %d sections found\n", ehdr.e_shnum);

    // 读取节区头表
    fseek(fp, ehdr.e_shoff, SEEK_SET);
    Elf32_Shdr *sh_table = malloc(ehdr.e_shnum * sizeof(Elf32_Shdr));
    if (fread(sh_table, sizeof(Elf32_Shdr), ehdr.e_shnum, fp) != ehdr.e_shnum) {
        fprintf(stderr, "Failed to read section headers\n");
        free(sh_table);
        fclose(fp);
        return -1;
    }

    // 读取节区名字符串表
    Elf32_Shdr sh_str = sh_table[ehdr.e_shstrndx];
    char *shstrtab = malloc(sh_str.sh_size);
    fseek(fp, sh_str.sh_offset, SEEK_SET);
    if (fread(shstrtab, 1, sh_str.sh_size, fp) != sh_str.sh_size) {
        fprintf(stderr, "Failed to read section string table\n");
        free(sh_table);
        free(shstrtab);
        fclose(fp);
        return -1;
    }

    // 找到.symtab和.strtab节
    Elf32_Shdr *symtab_hdr = NULL, *strtab_hdr = NULL;
    for (int i = 0; i < ehdr.e_shnum; i++) {
        const char *name = shstrtab + sh_table[i].sh_name;
        printf("Section %d: %s (type: %d)\n", i, name, sh_table[i].sh_type);
        if (strcmp(name, ".symtab") == 0) symtab_hdr = &sh_table[i];
        if (strcmp(name, ".strtab") == 0) strtab_hdr = &sh_table[i];
    }
    
    if (!symtab_hdr || !strtab_hdr) {
        fprintf(stderr, "No .symtab or .strtab found. symtab: %p, strtab: %p\n", 
                (void*)symtab_hdr, (void*)strtab_hdr);
        free(sh_table); 
        free(shstrtab); 
        fclose(fp);
        return -1;
    }

    printf("Found symtab at offset 0x%x, size: %d bytes\n", 
           symtab_hdr->sh_offset, symtab_hdr->sh_size);
    printf("Found strtab at offset 0x%x, size: %d bytes\n", 
           strtab_hdr->sh_offset, strtab_hdr->sh_size);

    // 读取符号表和字符串表
    Elf32_Sym *symtab = malloc(symtab_hdr->sh_size);
    fseek(fp, symtab_hdr->sh_offset, SEEK_SET);
    if (fread(symtab, 1, symtab_hdr->sh_size, fp) != symtab_hdr->sh_size) {
        fprintf(stderr, "Failed to read symbol table\n");
        free(sh_table);
        free(shstrtab);
        free(symtab);
        fclose(fp);
        return -1;
    }

    char *strtab = malloc(strtab_hdr->sh_size);
    fseek(fp, strtab_hdr->sh_offset, SEEK_SET);
    if (fread(strtab, 1, strtab_hdr->sh_size, fp) != strtab_hdr->sh_size) {
        fprintf(stderr, "Failed to read string table\n");
        free(sh_table);
        free(shstrtab);
        free(symtab);
        free(strtab);
        fclose(fp);
        return -1;
    }

    int sym_count = symtab_hdr->sh_size / sizeof(Elf32_Sym);
    printf("Found %d symbols in symbol table\n", sym_count);
    
    // 初始化符号表
    sym_table.count = 0;
    sym_table.capacity = 32;
    sym_table.symbols = malloc(sym_table.capacity * sizeof(func_symbol_t));

    // 遍历符号表，查找函数符号
    for (int i = 0; i < sym_count; i++) {
        unsigned char type = ELF32_ST_TYPE(symtab[i].st_info);
        if (type == STT_FUNC && symtab[i].st_size > 0 && symtab[i].st_name != 0) {
            if (sym_table.count >= sym_table.capacity) {
                sym_table.capacity *= 2;
                sym_table.symbols = realloc(sym_table.symbols, 
                                           sym_table.capacity * sizeof(func_symbol_t));
            }
            
            const char *name = strtab + symtab[i].st_name;
            printf("Found function: %s at 0x%x size %d\n", 
                   name, symtab[i].st_value, symtab[i].st_size);
                   
            func_symbol_t *fs = &sym_table.symbols[sym_table.count++];
            fs->name = strdup(name);
            fs->addr = symtab[i].st_value;
            fs->size = symtab[i].st_size;
        }
    }

    printf("Loaded %d function symbols\n", sym_table.count);

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