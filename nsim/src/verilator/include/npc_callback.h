#ifndef __NPC_CALLBACK_H__
#define __NPC_CALLBACK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <common.h>

void update_gprs(word_t *gprs);
void write_gpr(int idx, word_t data);
void update_csrs(word_t *csrs);
void write_csr(int idx, word_t data);
void update_inst(word_t inst, vaddr_t dnpc);
void ebreak(void);

#ifdef __cplusplus
}
#endif

#endif // __NPC_CALLBACK_H__