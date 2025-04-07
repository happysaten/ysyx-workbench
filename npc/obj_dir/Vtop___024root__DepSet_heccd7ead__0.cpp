// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtop.h for the primary calling header

#include "verilated.h"

#include "Vtop___024root.h"

void Vtop___024root___eval_act(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_act\n"); );
}

extern const VlUnpacked<CData/*6:0*/, 16> Vtop__ConstPool__TABLE_h25bdbd99_0;
extern const VlUnpacked<CData/*6:0*/, 32> Vtop__ConstPool__TABLE_h99acd3d6_0;

VL_INLINE_OPT void Vtop___024root___nba_sequent__TOP__0(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__0\n"); );
    // Init
    CData/*7:0*/ top__DOT__key_process0__DOT__cur_ascii;
    top__DOT__key_process0__DOT__cur_ascii = 0;
    CData/*6:0*/ top__DOT__key_process0__DOT____Vcellout__bcd7seg0__h;
    top__DOT__key_process0__DOT____Vcellout__bcd7seg0__h = 0;
    CData/*6:0*/ top__DOT__key_process0__DOT____Vcellout__bcd7seg1__h;
    top__DOT__key_process0__DOT____Vcellout__bcd7seg1__h = 0;
    CData/*6:0*/ top__DOT__key_process0__DOT____Vcellout__bcd7seg2__h;
    top__DOT__key_process0__DOT____Vcellout__bcd7seg2__h = 0;
    CData/*6:0*/ top__DOT__key_process0__DOT____Vcellout__bcd7seg3__h;
    top__DOT__key_process0__DOT____Vcellout__bcd7seg3__h = 0;
    CData/*6:0*/ top__DOT__key_process0__DOT____Vcellout__bcd7seg4__h;
    top__DOT__key_process0__DOT____Vcellout__bcd7seg4__h = 0;
    CData/*6:0*/ top__DOT__key_process0__DOT____Vcellout__bcd7seg5__h;
    top__DOT__key_process0__DOT____Vcellout__bcd7seg5__h = 0;
    CData/*4:0*/ __Vtableidx1;
    __Vtableidx1 = 0;
    CData/*4:0*/ __Vtableidx2;
    __Vtableidx2 = 0;
    CData/*4:0*/ __Vtableidx3;
    __Vtableidx3 = 0;
    CData/*4:0*/ __Vtableidx4;
    __Vtableidx4 = 0;
    CData/*3:0*/ __Vtableidx5;
    __Vtableidx5 = 0;
    CData/*3:0*/ __Vtableidx6;
    __Vtableidx6 = 0;
    CData/*2:0*/ __Vdly__top__DOT__ps2_keyboard0__DOT__ps2_clk_sync;
    __Vdly__top__DOT__ps2_keyboard0__DOT__ps2_clk_sync = 0;
    CData/*3:0*/ __Vdly__top__DOT__ps2_keyboard0__DOT__count;
    __Vdly__top__DOT__ps2_keyboard0__DOT__count = 0;
    CData/*2:0*/ __Vdly__top__DOT__ps2_keyboard0__DOT__w_ptr;
    __Vdly__top__DOT__ps2_keyboard0__DOT__w_ptr = 0;
    CData/*2:0*/ __Vdly__top__DOT__ps2_keyboard0__DOT__r_ptr;
    __Vdly__top__DOT__ps2_keyboard0__DOT__r_ptr = 0;
    CData/*0:0*/ __Vdly__overflow;
    __Vdly__overflow = 0;
    CData/*0:0*/ __Vdly__ready;
    __Vdly__ready = 0;
    CData/*2:0*/ __Vdlyvdim0__top__DOT__ps2_keyboard0__DOT__fifo__v0;
    __Vdlyvdim0__top__DOT__ps2_keyboard0__DOT__fifo__v0 = 0;
    CData/*7:0*/ __Vdlyvval__top__DOT__ps2_keyboard0__DOT__fifo__v0;
    __Vdlyvval__top__DOT__ps2_keyboard0__DOT__fifo__v0 = 0;
    CData/*0:0*/ __Vdlyvset__top__DOT__ps2_keyboard0__DOT__fifo__v0;
    __Vdlyvset__top__DOT__ps2_keyboard0__DOT__fifo__v0 = 0;
    CData/*7:0*/ __Vdly__top__DOT__key_process0__DOT__cnt;
    __Vdly__top__DOT__key_process0__DOT__cnt = 0;
    // Body
    __Vdly__top__DOT__ps2_keyboard0__DOT__ps2_clk_sync 
        = vlSelf->top__DOT__ps2_keyboard0__DOT__ps2_clk_sync;
    __Vdly__ready = vlSelf->ready;
    __Vdly__overflow = vlSelf->overflow;
    __Vdly__top__DOT__ps2_keyboard0__DOT__w_ptr = vlSelf->top__DOT__ps2_keyboard0__DOT__w_ptr;
    __Vdly__top__DOT__ps2_keyboard0__DOT__count = vlSelf->top__DOT__ps2_keyboard0__DOT__count;
    __Vdly__top__DOT__ps2_keyboard0__DOT__r_ptr = vlSelf->top__DOT__ps2_keyboard0__DOT__r_ptr;
    __Vdlyvset__top__DOT__ps2_keyboard0__DOT__fifo__v0 = 0U;
    __Vdly__top__DOT__key_process0__DOT__cnt = vlSelf->top__DOT__key_process0__DOT__cnt;
    __Vdly__top__DOT__ps2_keyboard0__DOT__ps2_clk_sync 
        = ((6U & ((IData)(vlSelf->top__DOT__ps2_keyboard0__DOT__ps2_clk_sync) 
                  << 1U)) | (IData)(vlSelf->ps2_clk));
    if (vlSelf->rst) {
        __Vdly__top__DOT__ps2_keyboard0__DOT__count = 0U;
        __Vdly__top__DOT__ps2_keyboard0__DOT__w_ptr = 0U;
        __Vdly__top__DOT__ps2_keyboard0__DOT__r_ptr = 0U;
        __Vdly__overflow = 0U;
        __Vdly__ready = 0U;
    } else {
        if (vlSelf->ready) {
            if ((1U & (~ (IData)(vlSelf->nextdata_n)))) {
                __Vdly__top__DOT__ps2_keyboard0__DOT__r_ptr 
                    = (7U & ((IData)(1U) + (IData)(vlSelf->top__DOT__ps2_keyboard0__DOT__r_ptr)));
                if (((IData)(vlSelf->top__DOT__ps2_keyboard0__DOT__w_ptr) 
                     == (7U & ((IData)(1U) + (IData)(vlSelf->top__DOT__ps2_keyboard0__DOT__r_ptr))))) {
                    __Vdly__ready = 0U;
                }
            }
        }
        if ((IData)((4U == (6U & (IData)(vlSelf->top__DOT__ps2_keyboard0__DOT__ps2_clk_sync))))) {
            if ((0xaU == (IData)(vlSelf->top__DOT__ps2_keyboard0__DOT__count))) {
                if ((((~ (IData)(vlSelf->top__DOT__ps2_keyboard0__DOT__buffer)) 
                      & (IData)(vlSelf->ps2_data)) 
                     & VL_REDXOR_32((0x1ffU & ((IData)(vlSelf->top__DOT__ps2_keyboard0__DOT__buffer) 
                                               >> 1U))))) {
                    __Vdlyvval__top__DOT__ps2_keyboard0__DOT__fifo__v0 
                        = (0xffU & ((IData)(vlSelf->top__DOT__ps2_keyboard0__DOT__buffer) 
                                    >> 1U));
                    __Vdlyvset__top__DOT__ps2_keyboard0__DOT__fifo__v0 = 1U;
                    __Vdlyvdim0__top__DOT__ps2_keyboard0__DOT__fifo__v0 
                        = vlSelf->top__DOT__ps2_keyboard0__DOT__w_ptr;
                    __Vdly__ready = 1U;
                    __Vdly__top__DOT__ps2_keyboard0__DOT__w_ptr 
                        = (7U & ((IData)(1U) + (IData)(vlSelf->top__DOT__ps2_keyboard0__DOT__w_ptr)));
                    __Vdly__overflow = ((IData)(vlSelf->overflow) 
                                        | ((IData)(vlSelf->top__DOT__ps2_keyboard0__DOT__r_ptr) 
                                           == (7U & 
                                               ((IData)(1U) 
                                                + (IData)(vlSelf->top__DOT__ps2_keyboard0__DOT__w_ptr)))));
                }
                __Vdly__top__DOT__ps2_keyboard0__DOT__count = 0U;
            } else {
                vlSelf->top__DOT__ps2_keyboard0__DOT____Vlvbound_h1a91ade8__0 
                    = vlSelf->ps2_data;
                if ((9U >= (IData)(vlSelf->top__DOT__ps2_keyboard0__DOT__count))) {
                    vlSelf->top__DOT__ps2_keyboard0__DOT__buffer 
                        = (((~ ((IData)(1U) << (IData)(vlSelf->top__DOT__ps2_keyboard0__DOT__count))) 
                            & (IData)(vlSelf->top__DOT__ps2_keyboard0__DOT__buffer)) 
                           | (0x3ffU & ((IData)(vlSelf->top__DOT__ps2_keyboard0__DOT____Vlvbound_h1a91ade8__0) 
                                        << (IData)(vlSelf->top__DOT__ps2_keyboard0__DOT__count))));
                }
                __Vdly__top__DOT__ps2_keyboard0__DOT__count 
                    = (0xfU & ((IData)(1U) + (IData)(vlSelf->top__DOT__ps2_keyboard0__DOT__count)));
            }
        }
    }
    vlSelf->top__DOT__ps2_keyboard0__DOT__count = __Vdly__top__DOT__ps2_keyboard0__DOT__count;
    vlSelf->top__DOT__ps2_keyboard0__DOT__w_ptr = __Vdly__top__DOT__ps2_keyboard0__DOT__w_ptr;
    vlSelf->overflow = __Vdly__overflow;
    vlSelf->top__DOT__ps2_keyboard0__DOT__ps2_clk_sync 
        = __Vdly__top__DOT__ps2_keyboard0__DOT__ps2_clk_sync;
    vlSelf->top__DOT__ps2_keyboard0__DOT__r_ptr = __Vdly__top__DOT__ps2_keyboard0__DOT__r_ptr;
    if (__Vdlyvset__top__DOT__ps2_keyboard0__DOT__fifo__v0) {
        vlSelf->top__DOT__ps2_keyboard0__DOT__fifo[__Vdlyvdim0__top__DOT__ps2_keyboard0__DOT__fifo__v0] 
            = __Vdlyvval__top__DOT__ps2_keyboard0__DOT__fifo__v0;
    }
    if (vlSelf->rst) {
        __Vdly__top__DOT__key_process0__DOT__cnt = 0U;
    } else if (((IData)(vlSelf->ready) & (0U == (IData)(vlSelf->top__DOT__key_process0__DOT__state)))) {
        __Vdly__top__DOT__key_process0__DOT__cnt = 
            (0xffU & ((IData)(1U) + (IData)(vlSelf->top__DOT__key_process0__DOT__cnt)));
    }
    vlSelf->top__DOT__key_process0__DOT__cnt = __Vdly__top__DOT__key_process0__DOT__cnt;
    __Vtableidx5 = (0xfU & (IData)(vlSelf->top__DOT__key_process0__DOT__cnt));
    top__DOT__key_process0__DOT____Vcellout__bcd7seg4__h 
        = Vtop__ConstPool__TABLE_h25bdbd99_0[__Vtableidx5];
    __Vtableidx6 = (0xfU & ((IData)(vlSelf->top__DOT__key_process0__DOT__cnt) 
                            >> 4U));
    top__DOT__key_process0__DOT____Vcellout__bcd7seg5__h 
        = Vtop__ConstPool__TABLE_h25bdbd99_0[__Vtableidx6];
    if (vlSelf->rst) {
        vlSelf->top__DOT__key_process0__DOT__cur_data = 0U;
    } else if (((IData)(vlSelf->ready) & (0U == (IData)(vlSelf->top__DOT__key_process0__DOT__state)))) {
        vlSelf->top__DOT__key_process0__DOT__cur_data 
            = vlSelf->top__DOT__data;
    }
    top__DOT__key_process0__DOT__cur_ascii = vlSelf->top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut
        [vlSelf->top__DOT__key_process0__DOT__cur_data];
    if (vlSelf->rst) {
        vlSelf->top__DOT__key_process0__DOT__state = 0U;
    } else if (VL_UNLIKELY(vlSelf->ready)) {
        VL_WRITEF("keyboard: %x\n",8,vlSelf->top__DOT__data);
        vlSelf->top__DOT__key_process0__DOT__state 
            = vlSelf->top__DOT__key_process0__DOT__next_state;
    }
    vlSelf->ready = __Vdly__ready;
    vlSelf->top__DOT__data = vlSelf->top__DOT__ps2_keyboard0__DOT__fifo
        [vlSelf->top__DOT__ps2_keyboard0__DOT__r_ptr];
    vlSelf->top__DOT__key_process0__DOT__next_state 
        = ((0U == (IData)(vlSelf->top__DOT__key_process0__DOT__state))
            ? 1U : ((1U == (IData)(vlSelf->top__DOT__key_process0__DOT__state))
                     ? ((0xf0U == (IData)(vlSelf->top__DOT__data))
                         ? 2U : 1U) : 0U));
    __Vtableidx1 = ((0x1eU & ((IData)(vlSelf->top__DOT__key_process0__DOT__cur_data) 
                              << 1U)) | (1U == (IData)(vlSelf->top__DOT__key_process0__DOT__state)));
    top__DOT__key_process0__DOT____Vcellout__bcd7seg0__h 
        = Vtop__ConstPool__TABLE_h99acd3d6_0[__Vtableidx1];
    __Vtableidx2 = ((0x1eU & ((IData)(vlSelf->top__DOT__key_process0__DOT__cur_data) 
                              >> 3U)) | (1U == (IData)(vlSelf->top__DOT__key_process0__DOT__state)));
    top__DOT__key_process0__DOT____Vcellout__bcd7seg1__h 
        = Vtop__ConstPool__TABLE_h99acd3d6_0[__Vtableidx2];
    __Vtableidx3 = ((0x1eU & ((IData)(top__DOT__key_process0__DOT__cur_ascii) 
                              << 1U)) | (1U == (IData)(vlSelf->top__DOT__key_process0__DOT__state)));
    top__DOT__key_process0__DOT____Vcellout__bcd7seg2__h 
        = Vtop__ConstPool__TABLE_h99acd3d6_0[__Vtableidx3];
    __Vtableidx4 = ((0x1eU & ((IData)(top__DOT__key_process0__DOT__cur_ascii) 
                              >> 3U)) | (1U == (IData)(vlSelf->top__DOT__key_process0__DOT__state)));
    top__DOT__key_process0__DOT____Vcellout__bcd7seg3__h 
        = Vtop__ConstPool__TABLE_h99acd3d6_0[__Vtableidx4];
    vlSelf->seg = (((QData)((IData)(top__DOT__key_process0__DOT____Vcellout__bcd7seg5__h)) 
                    << 0x23U) | (((QData)((IData)(top__DOT__key_process0__DOT____Vcellout__bcd7seg4__h)) 
                                  << 0x1cU) | (QData)((IData)(
                                                              (((IData)(top__DOT__key_process0__DOT____Vcellout__bcd7seg3__h) 
                                                                << 0x15U) 
                                                               | (((IData)(top__DOT__key_process0__DOT____Vcellout__bcd7seg2__h) 
                                                                   << 0xeU) 
                                                                  | (((IData)(top__DOT__key_process0__DOT____Vcellout__bcd7seg1__h) 
                                                                      << 7U) 
                                                                     | (IData)(top__DOT__key_process0__DOT____Vcellout__bcd7seg0__h))))))));
}

void Vtop___024root___eval_nba(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_nba\n"); );
    // Body
    if (vlSelf->__VnbaTriggered.at(0U)) {
        Vtop___024root___nba_sequent__TOP__0(vlSelf);
    }
}

void Vtop___024root___eval_triggers__act(Vtop___024root* vlSelf);
#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__act(Vtop___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__nba(Vtop___024root* vlSelf);
#endif  // VL_DEBUG

void Vtop___024root___eval(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval\n"); );
    // Init
    VlTriggerVec<1> __VpreTriggered;
    IData/*31:0*/ __VnbaIterCount;
    CData/*0:0*/ __VnbaContinue;
    // Body
    __VnbaIterCount = 0U;
    __VnbaContinue = 1U;
    while (__VnbaContinue) {
        __VnbaContinue = 0U;
        vlSelf->__VnbaTriggered.clear();
        vlSelf->__VactIterCount = 0U;
        vlSelf->__VactContinue = 1U;
        while (vlSelf->__VactContinue) {
            vlSelf->__VactContinue = 0U;
            Vtop___024root___eval_triggers__act(vlSelf);
            if (vlSelf->__VactTriggered.any()) {
                vlSelf->__VactContinue = 1U;
                if (VL_UNLIKELY((0x64U < vlSelf->__VactIterCount))) {
#ifdef VL_DEBUG
                    Vtop___024root___dump_triggers__act(vlSelf);
#endif
                    VL_FATAL_MT("vsrc/top.sv", 4, "", "Active region did not converge.");
                }
                vlSelf->__VactIterCount = ((IData)(1U) 
                                           + vlSelf->__VactIterCount);
                __VpreTriggered.andNot(vlSelf->__VactTriggered, vlSelf->__VnbaTriggered);
                vlSelf->__VnbaTriggered.set(vlSelf->__VactTriggered);
                Vtop___024root___eval_act(vlSelf);
            }
        }
        if (vlSelf->__VnbaTriggered.any()) {
            __VnbaContinue = 1U;
            if (VL_UNLIKELY((0x64U < __VnbaIterCount))) {
#ifdef VL_DEBUG
                Vtop___024root___dump_triggers__nba(vlSelf);
#endif
                VL_FATAL_MT("vsrc/top.sv", 4, "", "NBA region did not converge.");
            }
            __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
            Vtop___024root___eval_nba(vlSelf);
        }
    }
}

#ifdef VL_DEBUG
void Vtop___024root___eval_debug_assertions(Vtop___024root* vlSelf) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_debug_assertions\n"); );
    // Body
    if (VL_UNLIKELY((vlSelf->clk & 0xfeU))) {
        Verilated::overWidthError("clk");}
    if (VL_UNLIKELY((vlSelf->rst & 0xfeU))) {
        Verilated::overWidthError("rst");}
    if (VL_UNLIKELY((vlSelf->ps2_clk & 0xfeU))) {
        Verilated::overWidthError("ps2_clk");}
    if (VL_UNLIKELY((vlSelf->ps2_data & 0xfeU))) {
        Verilated::overWidthError("ps2_data");}
}
#endif  // VL_DEBUG
