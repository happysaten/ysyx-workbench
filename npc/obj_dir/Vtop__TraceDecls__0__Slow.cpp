// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing declarations
#include "verilated_fst_c.h"


void Vtop___024root__traceDeclTypesSub0(VerilatedFst* tracep) {
    {
        const char* __VenumItemNames[]
        = {"TYPE_R", "TYPE_I", "TYPE_S", "TYPE_B", 
                                "TYPE_U", "TYPE_J", 
                                "TYPE_N"};
        const char* __VenumItemValues[]
        = {"0", "1", "10", "11", "100", "101", "110"};
        tracep->declDTypeEnum(1, "$unit::inst_t", 7, 3, __VenumItemNames, __VenumItemValues);
    }
    {
        const char* __VenumItemNames[]
        = {"ALU_ADD", "ALU_SUB", "ALU_NOT", "ALU_AND", 
                                "ALU_OR", "ALU_XOR", 
                                "ALU_EQ", "ALU_SLT", 
                                "ALU_SLTU", "ALU_SLL", 
                                "ALU_SRL", "ALU_SRA"};
        const char* __VenumItemValues[]
        = {"0", "1", "10", "11", "100", "101", "110", 
                                "111", "1000", "1001", 
                                "1010", "1011"};
        tracep->declDTypeEnum(2, "$unit::alu_op_t", 12, 4, __VenumItemNames, __VenumItemValues);
    }
}

void Vtop___024root__trace_decl_types(VerilatedFst* tracep) {
    Vtop___024root__traceDeclTypesSub0(tracep);
}
