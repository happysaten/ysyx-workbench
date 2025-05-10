// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtop.h for the primary calling header

#include "Vtop__pch.h"
#include "Vtop___024root.h"

VL_ATTR_COLD void Vtop___024root___eval_static(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_static\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

VL_ATTR_COLD void Vtop___024root___eval_initial__TOP(Vtop___024root* vlSelf);

VL_ATTR_COLD void Vtop___024root___eval_initial(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_initial\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vtop___024root___eval_initial__TOP(vlSelf);
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
}

VL_ATTR_COLD void Vtop___024root___eval_initial__TOP(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_initial__TOP\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.nextdata_n = 0U;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x1cU] = 0x41U;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x32U] = 0x42U;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x21U] = 0x43U;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x23U] = 0x44U;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x24U] = 0x45U;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x2bU] = 0x46U;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x34U] = 0x47U;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x33U] = 0x48U;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x43U] = 0x49U;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x3bU] = 0x4aU;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x42U] = 0x4bU;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x4bU] = 0x4cU;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x3aU] = 0x4dU;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x31U] = 0x4eU;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x44U] = 0x4fU;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x4dU] = 0x50U;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x15U] = 0x51U;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x2dU] = 0x52U;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x1bU] = 0x53U;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x2cU] = 0x54U;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x3cU] = 0x55U;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x2aU] = 0x56U;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x1dU] = 0x57U;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x22U] = 0x58U;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x35U] = 0x59U;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x1aU] = 0x5aU;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x45U] = 0x30U;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x16U] = 0x31U;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x1eU] = 0x32U;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x26U] = 0x33U;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x25U] = 0x34U;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x2eU] = 0x35U;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x36U] = 0x36U;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x3dU] = 0x37U;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x3eU] = 0x38U;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0x46U] = 0x39U;
    vlSelfRef.top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[0U] = 0U;
}

VL_ATTR_COLD void Vtop___024root___eval_final(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_final\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__stl(Vtop___024root* vlSelf);
#endif  // VL_DEBUG
VL_ATTR_COLD bool Vtop___024root___eval_phase__stl(Vtop___024root* vlSelf);

VL_ATTR_COLD void Vtop___024root___eval_settle(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_settle\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    IData/*31:0*/ __VstlIterCount;
    CData/*0:0*/ __VstlContinue;
    // Body
    __VstlIterCount = 0U;
    vlSelfRef.__VstlFirstIteration = 1U;
    __VstlContinue = 1U;
    while (__VstlContinue) {
        if (VL_UNLIKELY(((0x64U < __VstlIterCount)))) {
#ifdef VL_DEBUG
            Vtop___024root___dump_triggers__stl(vlSelf);
#endif
            VL_FATAL_MT("vsrc/top.sv", 4, "", "Settle region did not converge.");
        }
        __VstlIterCount = ((IData)(1U) + __VstlIterCount);
        __VstlContinue = 0U;
        if (Vtop___024root___eval_phase__stl(vlSelf)) {
            __VstlContinue = 1U;
        }
        vlSelfRef.__VstlFirstIteration = 0U;
    }
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__stl(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___dump_triggers__stl\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VstlTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VstlTriggered.word(0U))) {
        VL_DBG_MSGF("         'stl' region trigger index 0 is active: Internal 'stl' trigger - first iteration\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vtop___024root___stl_sequent__TOP__0(Vtop___024root* vlSelf);

VL_ATTR_COLD void Vtop___024root___eval_stl(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_stl\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VstlTriggered.word(0U))) {
        Vtop___024root___stl_sequent__TOP__0(vlSelf);
    }
}

extern const VlUnpacked<CData/*6:0*/, 16> Vtop__ConstPool__TABLE_ha30365ce_0;
extern const VlUnpacked<CData/*6:0*/, 32> Vtop__ConstPool__TABLE_h22e50447_0;

VL_ATTR_COLD void Vtop___024root___stl_sequent__TOP__0(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___stl_sequent__TOP__0\n"); );
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
    // Body
    vlSelfRef.top__DOT__data = vlSelfRef.top__DOT__ps2_keyboard0__DOT__fifo
        [vlSelfRef.top__DOT__ps2_keyboard0__DOT__r_ptr];
    __Vtableidx5 = (0xfU & (IData)(vlSelfRef.top__DOT__key_process0__DOT__cnt));
    top__DOT__key_process0__DOT____Vcellout__bcd7seg4__h 
        = Vtop__ConstPool__TABLE_ha30365ce_0[__Vtableidx5];
    __Vtableidx6 = (0xfU & ((IData)(vlSelfRef.top__DOT__key_process0__DOT__cnt) 
                            >> 4U));
    top__DOT__key_process0__DOT____Vcellout__bcd7seg5__h 
        = Vtop__ConstPool__TABLE_ha30365ce_0[__Vtableidx6];
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
    vlSelfRef.top__DOT__key_process0__DOT__next_state 
        = ((0U == (IData)(vlSelfRef.top__DOT__key_process0__DOT__state))
            ? 1U : ((1U == (IData)(vlSelfRef.top__DOT__key_process0__DOT__state))
                     ? ((0xf0U == (IData)(vlSelfRef.top__DOT__data))
                         ? 2U : 1U) : 0U));
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

VL_ATTR_COLD void Vtop___024root___eval_triggers__stl(Vtop___024root* vlSelf);

VL_ATTR_COLD bool Vtop___024root___eval_phase__stl(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_phase__stl\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*0:0*/ __VstlExecute;
    // Body
    Vtop___024root___eval_triggers__stl(vlSelf);
    __VstlExecute = vlSelfRef.__VstlTriggered.any();
    if (__VstlExecute) {
        Vtop___024root___eval_stl(vlSelf);
    }
    return (__VstlExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__act(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___dump_triggers__act\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VactTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 0 is active: @(posedge clk)\n");
    }
}
#endif  // VL_DEBUG

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__nba(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___dump_triggers__nba\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VnbaTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 0 is active: @(posedge clk)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void Vtop___024root___ctor_var_reset(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___ctor_var_reset\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelf->clk = VL_RAND_RESET_I(1);
    vlSelf->rst = VL_RAND_RESET_I(1);
    vlSelf->ps2_clk = VL_RAND_RESET_I(1);
    vlSelf->ps2_data = VL_RAND_RESET_I(1);
    vlSelf->seg = VL_RAND_RESET_Q(42);
    vlSelf->overflow = VL_RAND_RESET_I(1);
    vlSelf->ready = VL_RAND_RESET_I(1);
    vlSelf->nextdata_n = VL_RAND_RESET_I(1);
    vlSelf->top__DOT__data = VL_RAND_RESET_I(8);
    vlSelf->top__DOT__ps2_keyboard0__DOT__buffer = VL_RAND_RESET_I(10);
    for (int __Vi0 = 0; __Vi0 < 8; ++__Vi0) {
        vlSelf->top__DOT__ps2_keyboard0__DOT__fifo[__Vi0] = VL_RAND_RESET_I(8);
    }
    vlSelf->top__DOT__ps2_keyboard0__DOT__w_ptr = VL_RAND_RESET_I(3);
    vlSelf->top__DOT__ps2_keyboard0__DOT__r_ptr = VL_RAND_RESET_I(3);
    vlSelf->top__DOT__ps2_keyboard0__DOT__count = VL_RAND_RESET_I(4);
    vlSelf->top__DOT__ps2_keyboard0__DOT__ps2_clk_sync = VL_RAND_RESET_I(3);
    vlSelf->top__DOT__ps2_keyboard0__DOT____Vlvbound_h4ebbf8f4__0 = VL_RAND_RESET_I(1);
    vlSelf->top__DOT__key_process0__DOT__cur_data = VL_RAND_RESET_I(8);
    vlSelf->top__DOT__key_process0__DOT__cnt = VL_RAND_RESET_I(8);
    vlSelf->top__DOT__key_process0__DOT__state = VL_RAND_RESET_I(2);
    vlSelf->top__DOT__key_process0__DOT__next_state = VL_RAND_RESET_I(2);
    for (int __Vi0 = 0; __Vi0 < 256; ++__Vi0) {
        vlSelf->top__DOT__key_process0__DOT__ps2_to_ascii0__DOT__lut[__Vi0] = VL_RAND_RESET_I(8);
    }
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = VL_RAND_RESET_I(1);
}
