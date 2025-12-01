/***************************************************************************************
 * Copyright (c) 2014-2024 Zihao Yu, Nanjing University
 *
 * NSIM is licensed under Mulan PSL v2.
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

#include <capstone/capstone.h>
#include <common.h>
// #include <cstdio>
// #include <cstdio>
#include <dlfcn.h>

static size_t (*cs_disasm_dl)(csh handle, const uint8_t *code, size_t code_size,
                              uint64_t address, size_t count, cs_insn **insn);
static void (*cs_free_dl)(cs_insn *insn, size_t count);

static csh handle;

void init_disasm() {
    void *dl_handle;
    dl_handle = dlopen("tools/capstone/repo/libcapstone.so.5", RTLD_LAZY);
    assert(dl_handle);

    cs_err (*cs_open_dl)(cs_arch arch, cs_mode mode, csh *handle) = NULL;
    cs_open_dl = dlsym(dl_handle, "cs_open");
    assert(cs_open_dl);

    cs_disasm_dl = dlsym(dl_handle, "cs_disasm");
    assert(cs_disasm_dl);

    cs_free_dl = dlsym(dl_handle, "cs_free");
    assert(cs_free_dl);

    cs_arch arch =
        MUXDEF(CONFIG_ISA_x86, CS_ARCH_X86,
               MUXDEF(CONFIG_ISA_mips32, CS_ARCH_MIPS,
                      MUXDEF(CONFIG_ISA_riscv, CS_ARCH_RISCV,
                             MUXDEF(CONFIG_ISA_npc, CS_ARCH_RISCV,
                                    MUXDEF(CONFIG_ISA_loongarch32r,
                                           CS_ARCH_LOONGARCH, -1)))));
    cs_mode mode = MUXDEF(
        CONFIG_ISA_x86, CS_MODE_32,
        MUXDEF(CONFIG_ISA_mips32, CS_MODE_MIPS32,
               MUXDEF(CONFIG_ISA_riscv,
                      MUXDEF(CONFIG_ISA64, CS_MODE_RISCV64, CS_MODE_RISCV32) |
                          CS_MODE_RISCVC,
                      MUXDEF(CONFIG_ISA_npc,
                             MUXDEF(CONFIG_ISA64, CS_MODE_RISCV64,
                                    CS_MODE_RISCV32) |
                                 CS_MODE_RISCVC,
                             MUXDEF(CONFIG_ISA_loongarch32r,
                                    CS_MODE_LOONGARCH32, -1)))));
    printf("Disassembler initialized with arch: %d, mode: %d\n", arch, mode);
    int ret = cs_open_dl(arch, mode, &handle);
    assert(ret == CS_ERR_OK);

#ifdef CONFIG_ISA_x86
    cs_err (*cs_option_dl)(csh handle, cs_opt_type type, size_t value) = NULL;
    cs_option_dl = dlsym(dl_handle, "cs_option");
    assert(cs_option_dl);

    ret = cs_option_dl(handle, CS_OPT_SYNTAX, CS_OPT_SYNTAX_ATT);
    assert(ret == CS_ERR_OK);
#endif
}

// void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte) {
//     cs_insn *insn;
//     size_t count = cs_disasm_dl(handle, code, nbyte, pc, 0, &insn);
//     // printf("disasm count: %zu\n", count);
//     // assert(count == 1);
//     // int ret = snprintf(str, size, "%s", insn->mnemonic);
//     // if (insn->op_str[0] != '\0') {
//     //     snprintf(str + ret, size - ret, "\t%s", insn->op_str);
//     // }
//     cs_free_dl(insn, count);
// }

// 更安全的实现
// 把 cs_disasm_dl 的 count 参数设为 1（如果确实只需一条指令），并检查返回值 count > 0。
// 在访问 insn 前做空指针和 count 检查。
// 使用安全的剩余缓冲计算：size_t rem = (ret < size) ? (size - ret) : 0; 并仅在 rem > 1 时调用追加 snprintf，以保留至少一个字节放终止 NUL。也可用 strncat 或手工处理尾部指针以避免转换问题。
// 可记录并处理 snprintf 的截断情况（例如返回码 >= 可用大小表示被截断）。
// 在释放 insn 前确保 insn 非空且 count 合理，然后使用 cs_free_dl 释放。
void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte) {
    cs_insn *insn = NULL;
    size_t count = cs_disasm_dl(handle, code, nbyte, pc, 1, &insn); // 请求一条指令
    if (count == 0 || insn == NULL) {
        if (size > 0) str[0] = '\0';
        return;
    }
    int ret = snprintf(str, size, "%s", insn->mnemonic);
    size_t rem = (ret < size) ? (size - (size_t)ret) : 0;
    if (rem > 1 && insn->op_str[0] != '\0') {
        /* 保证至少留一个字节给终止 NUL */
        snprintf(str + ((ret < size) ? ret : size - 1), rem, "\t%s", insn->op_str);
    }

    cs_free_dl(insn, count);
}
