// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Prototypes for DPI import and export functions.
//
// Verilator includes this file in all generated .cpp files that use DPI functions.
// Manually include this file where DPI .c import functions are declared to ensure
// the C functions match the expectations of the DPI imports.

#ifndef VERILATED_VTOP__DPI_H_
#define VERILATED_VTOP__DPI_H_  // guard

#include "svdpi.h"

#ifdef __cplusplus
extern "C" {
#endif


    // DPI IMPORTS
    // DPI import at /home/saten/Code/Other/ysyx-workbench/npc/vsrc/top.sv:331:34
    extern void NPCINV(int pc);
    // DPI import at /home/saten/Code/Other/ysyx-workbench/npc/vsrc/top.sv:287:34
    extern void NPCTRAP();
    // DPI import at /home/saten/Code/Other/ysyx-workbench/npc/vsrc/top.sv:118:33
    extern int pmem_read_npc(int raddr);
    // DPI import at /home/saten/Code/Other/ysyx-workbench/npc/vsrc/top.sv:325:34
    extern void pmem_write_npc(int waddr, int wdata, char wmask);
    // DPI import at /home/saten/Code/Other/ysyx-workbench/npc/vsrc/top.sv:120:34
    extern void update_inst_npc(int inst, int dnpc);
    // DPI import at /home/saten/Code/Other/ysyx-workbench/npc/vsrc/top.sv:213:34
    extern void write_gpr_npc(const svLogicVecVal* idx, const svLogicVecVal* data);

#ifdef __cplusplus
}
#endif

#endif  // guard
