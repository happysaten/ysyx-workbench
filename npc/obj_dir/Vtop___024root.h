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
    VL_IN8(rst,0,0);
    VL_IN8(ps2_clk,0,0);
    VL_IN8(ps2_data,0,0);
    VL_OUT64(seg,41,0);
    VL_OUT8(overflow,0,0);
    VL_OUT8(ready,0,0);
    VL_OUT8(nextdata_n,0,0);
    CData/*7:0*/ top__DOT__data;
    CData/*2:0*/ top__DOT__ps2_keyboard0__DOT__w_ptr;
    CData/*2:0*/ top__DOT__ps2_keyboard0__DOT__r_ptr;
    CData/*3:0*/ top__DOT__ps2_keyboard0__DOT__count;
    CData/*2:0*/ top__DOT__ps2_keyboard0__DOT__ps2_clk_sync;
    CData/*0:0*/ top__DOT__ps2_keyboard0__DOT____Vlvbound_h4ebbf8f4__0;
    CData/*7:0*/ top__DOT__key_process0__DOT__cur_data;
    CData/*7:0*/ top__DOT__key_process0__DOT__cnt;
    CData/*1:0*/ top__DOT__key_process0__DOT__state;
    CData/*1:0*/ top__DOT__key_process0__DOT__next_state;
    CData/*0:0*/ __VstlFirstIteration;
    CData/*0:0*/ __Vtrigprevexpr___TOP__clk__0;
    CData/*0:0*/ __VactContinue;
    SData/*9:0*/ top__DOT__ps2_keyboard0__DOT__buffer;
    IData/*31:0*/ __VactIterCount;
    VlUnpacked<CData/*7:0*/, 8> top__DOT__ps2_keyboard0__DOT__fifo;
    VlUnpacked<CData/*7:0*/, 256> top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut;
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
