// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtop.h for the primary calling header

#include "Vtop__pch.h"
#include "Vtop___024root.h"

void Vtop___024root___eval_act(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_act\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

void Vtop___024root___nba_sequent__TOP__0(Vtop___024root* vlSelf);

void Vtop___024root___eval_nba(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_nba\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        Vtop___024root___nba_sequent__TOP__0(vlSelf);
    }
}

extern const VlUnpacked<CData/*6:0*/, 16> Vtop__ConstPool__TABLE_ha30365ce_0;
extern const VlUnpacked<CData/*6:0*/, 32> Vtop__ConstPool__TABLE_h22e50447_0;

VL_INLINE_OPT void Vtop___024root___nba_sequent__TOP__0(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__0\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
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
    CData/*7:0*/ __Vdly__top__DOT__key_process0__DOT__cnt;
    __Vdly__top__DOT__key_process0__DOT__cnt = 0;
    CData/*7:0*/ __VdlyVal__top__DOT__ps2_keyboard0__DOT__fifo__v0;
    __VdlyVal__top__DOT__ps2_keyboard0__DOT__fifo__v0 = 0;
    CData/*2:0*/ __VdlyDim0__top__DOT__ps2_keyboard0__DOT__fifo__v0;
    __VdlyDim0__top__DOT__ps2_keyboard0__DOT__fifo__v0 = 0;
    CData/*0:0*/ __VdlySet__top__DOT__ps2_keyboard0__DOT__fifo__v0;
    __VdlySet__top__DOT__ps2_keyboard0__DOT__fifo__v0 = 0;
    // Body
    __Vdly__top__DOT__ps2_keyboard0__DOT__ps2_clk_sync 
        = vlSelfRef.top__DOT__ps2_keyboard0__DOT__ps2_clk_sync;
    __Vdly__top__DOT__ps2_keyboard0__DOT__count = vlSelfRef.top__DOT__ps2_keyboard0__DOT__count;
    __Vdly__top__DOT__ps2_keyboard0__DOT__w_ptr = vlSelfRef.top__DOT__ps2_keyboard0__DOT__w_ptr;
    __Vdly__overflow = vlSelfRef.overflow;
    __Vdly__ready = vlSelfRef.ready;
    __Vdly__top__DOT__ps2_keyboard0__DOT__r_ptr = vlSelfRef.top__DOT__ps2_keyboard0__DOT__r_ptr;
    __VdlySet__top__DOT__ps2_keyboard0__DOT__fifo__v0 = 0U;
    __Vdly__top__DOT__key_process0__DOT__cnt = vlSelfRef.top__DOT__key_process0__DOT__cnt;
    __Vdly__top__DOT__ps2_keyboard0__DOT__ps2_clk_sync 
        = ((6U & ((IData)(vlSelfRef.top__DOT__ps2_keyboard0__DOT__ps2_clk_sync) 
                  << 1U)) | (IData)(vlSelfRef.ps2_clk));
    if (vlSelfRef.rst) {
        __Vdly__top__DOT__ps2_keyboard0__DOT__count = 0U;
        __Vdly__top__DOT__ps2_keyboard0__DOT__w_ptr = 0U;
        __Vdly__top__DOT__ps2_keyboard0__DOT__r_ptr = 0U;
        __Vdly__overflow = 0U;
        __Vdly__ready = 0U;
    } else {
        if (vlSelfRef.ready) {
            if ((1U & (~ (IData)(vlSelfRef.nextdata_n)))) {
                __Vdly__top__DOT__ps2_keyboard0__DOT__r_ptr 
                    = (7U & ((IData)(1U) + (IData)(vlSelfRef.top__DOT__ps2_keyboard0__DOT__r_ptr)));
                if (((IData)(vlSelfRef.top__DOT__ps2_keyboard0__DOT__w_ptr) 
                     == (7U & ((IData)(1U) + (IData)(vlSelfRef.top__DOT__ps2_keyboard0__DOT__r_ptr))))) {
                    __Vdly__ready = 0U;
                }
            }
        }
        if ((IData)((4U == (6U & (IData)(vlSelfRef.top__DOT__ps2_keyboard0__DOT__ps2_clk_sync))))) {
            if ((0xaU == (IData)(vlSelfRef.top__DOT__ps2_keyboard0__DOT__count))) {
                if ((((~ (IData)(vlSelfRef.top__DOT__ps2_keyboard0__DOT__buffer)) 
                      & (IData)(vlSelfRef.ps2_data)) 
                     & VL_REDXOR_32((0x1ffU & ((IData)(vlSelfRef.top__DOT__ps2_keyboard0__DOT__buffer) 
                                               >> 1U))))) {
                    __VdlyVal__top__DOT__ps2_keyboard0__DOT__fifo__v0 
                        = (0xffU & ((IData)(vlSelfRef.top__DOT__ps2_keyboard0__DOT__buffer) 
                                    >> 1U));
                    __VdlyDim0__top__DOT__ps2_keyboard0__DOT__fifo__v0 
                        = vlSelfRef.top__DOT__ps2_keyboard0__DOT__w_ptr;
                    __VdlySet__top__DOT__ps2_keyboard0__DOT__fifo__v0 = 1U;
                    __Vdly__ready = 1U;
                    __Vdly__top__DOT__ps2_keyboard0__DOT__w_ptr 
                        = (7U & ((IData)(1U) + (IData)(vlSelfRef.top__DOT__ps2_keyboard0__DOT__w_ptr)));
                    __Vdly__overflow = ((IData)(vlSelfRef.overflow) 
                                        | ((IData)(vlSelfRef.top__DOT__ps2_keyboard0__DOT__r_ptr) 
                                           == (7U & 
                                               ((IData)(1U) 
                                                + (IData)(vlSelfRef.top__DOT__ps2_keyboard0__DOT__w_ptr)))));
                }
                __Vdly__top__DOT__ps2_keyboard0__DOT__count = 0U;
            } else {
                vlSelfRef.top__DOT__ps2_keyboard0__DOT____Vlvbound_h4ebbf8f4__0 
                    = vlSelfRef.ps2_data;
                if ((9U >= (IData)(vlSelfRef.top__DOT__ps2_keyboard0__DOT__count))) {
                    vlSelfRef.top__DOT__ps2_keyboard0__DOT__buffer 
                        = (((~ ((IData)(1U) << (IData)(vlSelfRef.top__DOT__ps2_keyboard0__DOT__count))) 
                            & (IData)(vlSelfRef.top__DOT__ps2_keyboard0__DOT__buffer)) 
                           | (0x3ffU & ((IData)(vlSelfRef.top__DOT__ps2_keyboard0__DOT____Vlvbound_h4ebbf8f4__0) 
                                        << (IData)(vlSelfRef.top__DOT__ps2_keyboard0__DOT__count))));
                }
                __Vdly__top__DOT__ps2_keyboard0__DOT__count 
                    = (0xfU & ((IData)(1U) + (IData)(vlSelfRef.top__DOT__ps2_keyboard0__DOT__count)));
            }
        }
    }
    vlSelfRef.top__DOT__ps2_keyboard0__DOT__count = __Vdly__top__DOT__ps2_keyboard0__DOT__count;
    vlSelfRef.top__DOT__ps2_keyboard0__DOT__w_ptr = __Vdly__top__DOT__ps2_keyboard0__DOT__w_ptr;
    vlSelfRef.overflow = __Vdly__overflow;
    vlSelfRef.top__DOT__ps2_keyboard0__DOT__ps2_clk_sync 
        = __Vdly__top__DOT__ps2_keyboard0__DOT__ps2_clk_sync;
    vlSelfRef.top__DOT__ps2_keyboard0__DOT__r_ptr = __Vdly__top__DOT__ps2_keyboard0__DOT__r_ptr;
    if (__VdlySet__top__DOT__ps2_keyboard0__DOT__fifo__v0) {
        vlSelfRef.top__DOT__ps2_keyboard0__DOT__fifo[__VdlyDim0__top__DOT__ps2_keyboard0__DOT__fifo__v0] 
            = __VdlyVal__top__DOT__ps2_keyboard0__DOT__fifo__v0;
    }
    if (vlSelfRef.rst) {
        __Vdly__top__DOT__key_process0__DOT__cnt = 0U;
    } else if (((IData)(vlSelfRef.ready) & (0U == (IData)(vlSelfRef.top__DOT__key_process0__DOT__state)))) {
        __Vdly__top__DOT__key_process0__DOT__cnt = 
            (0xffU & ((IData)(1U) + (IData)(vlSelfRef.top__DOT__key_process0__DOT__cnt)));
    }
    vlSelfRef.top__DOT__key_process0__DOT__cnt = __Vdly__top__DOT__key_process0__DOT__cnt;
    __Vtableidx5 = (0xfU & (IData)(vlSelfRef.top__DOT__key_process0__DOT__cnt));
    top__DOT__key_process0__DOT____Vcellout__bcd7seg4__h 
        = Vtop__ConstPool__TABLE_ha30365ce_0[__Vtableidx5];
    __Vtableidx6 = (0xfU & ((IData)(vlSelfRef.top__DOT__key_process0__DOT__cnt) 
                            >> 4U));
    top__DOT__key_process0__DOT____Vcellout__bcd7seg5__h 
        = Vtop__ConstPool__TABLE_ha30365ce_0[__Vtableidx6];
    if (vlSelfRef.rst) {
        vlSelfRef.top__DOT__key_process0__DOT__cur_data = 0U;
    } else if (((IData)(vlSelfRef.ready) & (0U == (IData)(vlSelfRef.top__DOT__key_process0__DOT__state)))) {
        vlSelfRef.top__DOT__key_process0__DOT__cur_data 
            = vlSelfRef.top__DOT__data;
    }
    if (vlSelfRef.rst) {
        vlSelfRef.top__DOT__key_process0__DOT__state = 0U;
    } else if (VL_UNLIKELY((vlSelfRef.ready))) {
        VL_WRITEF_NX("keyboard: %x\n",0,8,vlSelfRef.top__DOT__data);
        vlSelfRef.top__DOT__key_process0__DOT__state 
            = vlSelfRef.top__DOT__key_process0__DOT__next_state;
    }
    vlSelfRef.ready = __Vdly__ready;
    vlSelfRef.top__DOT__data = vlSelfRef.top__DOT__ps2_keyboard0__DOT__fifo
        [vlSelfRef.top__DOT__ps2_keyboard0__DOT__r_ptr];
    vlSelfRef.top__DOT__key_process0__DOT__next_state 
        = ((0U == (IData)(vlSelfRef.top__DOT__key_process0__DOT__state))
            ? 1U : ((1U == (IData)(vlSelfRef.top__DOT__key_process0__DOT__state))
                     ? ((0xf0U == (IData)(vlSelfRef.top__DOT__data))
                         ? 2U : 1U) : 0U));
    __Vtableidx1 = ((0x1eU & ((IData)(vlSelfRef.top__DOT__key_process0__DOT__cur_data) 
                              << 1U)) | (1U == (IData)(vlSelfRef.top__DOT__key_process0__DOT__state)));
    top__DOT__key_process0__DOT____Vcellout__bcd7seg0__h 
        = Vtop__ConstPool__TABLE_h22e50447_0[__Vtableidx1];
    __Vtableidx2 = ((0x1eU & ((IData)(vlSelfRef.top__DOT__key_process0__DOT__cur_data) 
                              >> 3U)) | (1U == (IData)(vlSelfRef.top__DOT__key_process0__DOT__state)));
    top__DOT__key_process0__DOT____Vcellout__bcd7seg1__h 
        = Vtop__ConstPool__TABLE_h22e50447_0[__Vtableidx2];
    __Vtableidx3 = ((0x1eU & (vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut
                              [vlSelfRef.top__DOT__key_process0__DOT__cur_data] 
                              << 1U)) | (1U == (IData)(vlSelfRef.top__DOT__key_process0__DOT__state)));
    top__DOT__key_process0__DOT____Vcellout__bcd7seg2__h 
        = Vtop__ConstPool__TABLE_h22e50447_0[__Vtableidx3];
    __Vtableidx4 = ((0x1eU & (vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut
                              [vlSelfRef.top__DOT__key_process0__DOT__cur_data] 
                              >> 3U)) | (1U == (IData)(vlSelfRef.top__DOT__key_process0__DOT__state)));
    top__DOT__key_process0__DOT____Vcellout__bcd7seg3__h 
        = Vtop__ConstPool__TABLE_h22e50447_0[__Vtableidx4];
    vlSelfRef.seg = (((QData)((IData)((((IData)(top__DOT__key_process0__DOT____Vcellout__bcd7seg5__h) 
                                        << 0xeU) | 
                                       (((IData)(top__DOT__key_process0__DOT____Vcellout__bcd7seg4__h) 
                                         << 7U) | (IData)(top__DOT__key_process0__DOT____Vcellout__bcd7seg3__h))))) 
                      << 0x15U) | (QData)((IData)((
                                                   ((IData)(top__DOT__key_process0__DOT____Vcellout__bcd7seg2__h) 
                                                    << 0xeU) 
                                                   | (((IData)(top__DOT__key_process0__DOT____Vcellout__bcd7seg1__h) 
                                                       << 7U) 
                                                      | (IData)(top__DOT__key_process0__DOT____Vcellout__bcd7seg0__h))))));
}

void Vtop___024root___eval_triggers__act(Vtop___024root* vlSelf);

bool Vtop___024root___eval_phase__act(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_phase__act\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    VlTriggerVec<1> __VpreTriggered;
    CData/*0:0*/ __VactExecute;
    // Body
    Vtop___024root___eval_triggers__act(vlSelf);
    __VactExecute = vlSelfRef.__VactTriggered.any();
    if (__VactExecute) {
        __VpreTriggered.andNot(vlSelfRef.__VactTriggered, vlSelfRef.__VnbaTriggered);
        vlSelfRef.__VnbaTriggered.thisOr(vlSelfRef.__VactTriggered);
        Vtop___024root___eval_act(vlSelf);
    }
    return (__VactExecute);
}

bool Vtop___024root___eval_phase__nba(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_phase__nba\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = vlSelfRef.__VnbaTriggered.any();
    if (__VnbaExecute) {
        Vtop___024root___eval_nba(vlSelf);
        vlSelfRef.__VnbaTriggered.clear();
    }
    return (__VnbaExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__nba(Vtop___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__act(Vtop___024root* vlSelf);
#endif  // VL_DEBUG

void Vtop___024root___eval(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    IData/*31:0*/ __VnbaIterCount;
    CData/*0:0*/ __VnbaContinue;
    // Body
    __VnbaIterCount = 0U;
    __VnbaContinue = 1U;
    while (__VnbaContinue) {
        if (VL_UNLIKELY(((0x64U < __VnbaIterCount)))) {
#ifdef VL_DEBUG
            Vtop___024root___dump_triggers__nba(vlSelf);
#endif
            VL_FATAL_MT("vsrc/top.sv", 4, "", "NBA region did not converge.");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        __VnbaContinue = 0U;
        vlSelfRef.__VactIterCount = 0U;
        vlSelfRef.__VactContinue = 1U;
        while (vlSelfRef.__VactContinue) {
            if (VL_UNLIKELY(((0x64U < vlSelfRef.__VactIterCount)))) {
#ifdef VL_DEBUG
                Vtop___024root___dump_triggers__act(vlSelf);
#endif
                VL_FATAL_MT("vsrc/top.sv", 4, "", "Active region did not converge.");
            }
            vlSelfRef.__VactIterCount = ((IData)(1U) 
                                         + vlSelfRef.__VactIterCount);
            vlSelfRef.__VactContinue = 0U;
            if (Vtop___024root___eval_phase__act(vlSelf)) {
                vlSelfRef.__VactContinue = 1U;
            }
        }
        if (Vtop___024root___eval_phase__nba(vlSelf)) {
            __VnbaContinue = 1U;
        }
    }
}

#ifdef VL_DEBUG
void Vtop___024root___eval_debug_assertions(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_debug_assertions\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (VL_UNLIKELY(((vlSelfRef.clk & 0xfeU)))) {
        Verilated::overWidthError("clk");}
    if (VL_UNLIKELY(((vlSelfRef.rst & 0xfeU)))) {
        Verilated::overWidthError("rst");}
    if (VL_UNLIKELY(((vlSelfRef.ps2_clk & 0xfeU)))) {
        Verilated::overWidthError("ps2_clk");}
    if (VL_UNLIKELY(((vlSelfRef.ps2_data & 0xfeU)))) {
        Verilated::overWidthError("ps2_data");}
}
#endif  // VL_DEBUG
