// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See Vtop.h for the primary calling header

#ifndef VERILATED_VTOP___024ROOT_H_
#define VERILATED_VTOP___024ROOT_H_  // guard

#include "verilated.h"


class Vtop__Syms;

class alignas(VL_CACHE_LINE_BYTES) Vtop___024root final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    VL_IN8(clk,0,0);
    VL_IN8(reset,0,0);
    CData/*0:0*/ top__DOT__reset_sync;
    CData/*2:0*/ top__DOT__inst_type;
    CData/*0:0*/ top__DOT__jump_en;
    CData/*3:0*/ top__DOT__u_exe__DOT__alu_op;
    CData/*6:0*/ __Vtableidx1;
    CData/*0:0*/ __VstlFirstIteration;
    CData/*0:0*/ __Vtrigprevexpr___TOP__clk__0;
    CData/*0:0*/ __VactContinue;
    IData/*31:0*/ top__DOT__rdata1;
    IData/*31:0*/ top__DOT__rdata2;
    IData/*31:0*/ top__DOT__wdata;
    IData/*31:0*/ top__DOT__imm;
    IData/*31:0*/ top__DOT__pc;
    IData/*31:0*/ top__DOT__snpc;
    IData/*31:0*/ top__DOT__dnpc;
    IData/*31:0*/ top__DOT__inst;
    IData/*31:0*/ top__DOT__jump_target;
    IData/*31:0*/ top__DOT__u_gpr__DOT__unnamedblk1__DOT__i;
    IData/*31:0*/ top__DOT__u_exe__DOT__alu_a;
    IData/*31:0*/ top__DOT__u_exe__DOT__alu_b;
    IData/*31:0*/ top__DOT__u_exe__DOT__alu_result;
    IData/*31:0*/ top__DOT__u_exe__DOT__mem_rdata;
    IData/*31:0*/ top__DOT__u_exe__DOT__u_alu__DOT__add_res;
    IData/*31:0*/ top__DOT__u_exe__DOT__u_alu__DOT__adder_inst__DOT__B1;
    IData/*31:0*/ __Vfunc_top__DOT__pmem_read_npc__0__Vfuncout;
    IData/*31:0*/ __Vfunc_top__DOT__u_exe__DOT__pmem_read_npc__8__Vfuncout;
    IData/*31:0*/ __VactIterCount;
    VlUnpacked<IData/*31:0*/, 32> top__DOT__u_gpr__DOT__regfile;
    VlUnpacked<CData/*0:0*/, 2> __Vm_traceActivity;
    VlTriggerVec<1> __VstlTriggered;
    VlTriggerVec<1> __VactTriggered;
    VlTriggerVec<1> __VnbaTriggered;

    // INTERNAL VARIABLES
    Vtop__Syms* const vlSymsp;

    // CONSTRUCTORS
    Vtop___024root(Vtop__Syms* symsp, const char* v__name);
    ~Vtop___024root();
    VL_UNCOPYABLE(Vtop___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
