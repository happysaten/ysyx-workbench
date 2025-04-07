// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_fst_c.h"
#include "Vtop__Syms.h"


void Vtop___024root__trace_chg_sub_0(Vtop___024root* vlSelf, VerilatedFst::Buffer* bufp);

void Vtop___024root__trace_chg_top_0(void* voidSelf, VerilatedFst::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root__trace_chg_top_0\n"); );
    // Init
    Vtop___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vtop___024root*>(voidSelf);
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (VL_UNLIKELY(!vlSymsp->__Vm_activity)) return;
    // Body
    Vtop___024root__trace_chg_sub_0((&vlSymsp->TOP), bufp);
}

void Vtop___024root__trace_chg_sub_0(Vtop___024root* vlSelf, VerilatedFst::Buffer* bufp) {
    if (false && vlSelf) {}  // Prevent unused
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root__trace_chg_sub_0\n"); );
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode + 1);
    // Body
    if (VL_UNLIKELY(vlSelf->__Vm_traceActivity[1U])) {
        bufp->chgBit(oldp+0,((1U & (~ (IData)((0U != (IData)(vlSelf->top__DOT__add_result)))))));
        bufp->chgCData(oldp+1,(vlSelf->top__DOT__add_result),4);
        bufp->chgCData(oldp+2,(vlSelf->top__DOT__adder_inst__DOT__B1),4);
    }
    bufp->chgCData(oldp+3,(vlSelf->A),4);
    bufp->chgCData(oldp+4,(vlSelf->B),4);
    bufp->chgCData(oldp+5,(vlSelf->ALUop),3);
    bufp->chgCData(oldp+6,(vlSelf->Result),4);
    bufp->chgCData(oldp+7,(vlSelf->b),4);
    bufp->chgCData(oldp+8,(vlSelf->h),7);
    bufp->chgBit(oldp+9,((((1U & ((IData)(vlSelf->A) 
                                  >> 3U)) == (1U & 
                                              ((IData)(vlSelf->top__DOT__adder_inst__DOT__B1) 
                                               >> 3U))) 
                          & ((1U & ((IData)(vlSelf->top__DOT__add_result) 
                                    >> 3U)) != (1U 
                                                & ((IData)(vlSelf->A) 
                                                   >> 3U))))));
    bufp->chgCData(oldp+10,((0xfU & (~ (IData)(vlSelf->A)))),4);
    bufp->chgCData(oldp+11,(((IData)(vlSelf->A) & (IData)(vlSelf->B))),4);
    bufp->chgCData(oldp+12,(((IData)(vlSelf->A) | (IData)(vlSelf->B))),4);
    bufp->chgCData(oldp+13,(((IData)(vlSelf->A) ^ (IData)(vlSelf->B))),4);
    bufp->chgBit(oldp+14,((0U != (3U & (IData)(vlSelf->ALUop)))));
    bufp->chgBit(oldp+15,((1U & (((IData)(vlSelf->A) 
                                  + ((IData)(vlSelf->top__DOT__adder_inst__DOT__B1) 
                                     + (0U != (3U & (IData)(vlSelf->ALUop))))) 
                                 >> 4U))));
}

void Vtop___024root__trace_cleanup(void* voidSelf, VerilatedFst* /*unused*/) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root__trace_cleanup\n"); );
    // Init
    Vtop___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vtop___024root*>(voidSelf);
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    vlSymsp->__Vm_activity = false;
    vlSymsp->TOP.__Vm_traceActivity[0U] = 0U;
    vlSymsp->TOP.__Vm_traceActivity[1U] = 0U;
}
