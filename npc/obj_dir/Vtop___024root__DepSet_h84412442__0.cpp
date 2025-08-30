// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See Vtop.h for the primary calling header

#include "Vtop__pch.h"
#include "Vtop__Syms.h"
#include "Vtop___024root.h"

extern "C" int pmem_read_npc(int raddr);

VL_INLINE_OPT void Vtop___024root____Vdpiimwrap_top__DOT__pmem_read_npc_TOP(IData/*31:0*/ raddr, IData/*31:0*/ &pmem_read_npc__Vfuncrtn) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root____Vdpiimwrap_top__DOT__pmem_read_npc_TOP\n"); );
    // Body
    int raddr__Vcvt;
    for (size_t raddr__Vidx = 0; raddr__Vidx < 1; ++raddr__Vidx) raddr__Vcvt = raddr;
    int pmem_read_npc__Vfuncrtn__Vcvt;
    pmem_read_npc__Vfuncrtn__Vcvt = pmem_read_npc(raddr__Vcvt);
    pmem_read_npc__Vfuncrtn = pmem_read_npc__Vfuncrtn__Vcvt;
}

extern "C" void update_inst_npc(int inst, int dnpc);

VL_INLINE_OPT void Vtop___024root____Vdpiimwrap_top__DOT__update_inst_npc_TOP(IData/*31:0*/ inst, IData/*31:0*/ dnpc) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root____Vdpiimwrap_top__DOT__update_inst_npc_TOP\n"); );
    // Body
    int inst__Vcvt;
    for (size_t inst__Vidx = 0; inst__Vidx < 1; ++inst__Vidx) inst__Vcvt = inst;
    int dnpc__Vcvt;
    for (size_t dnpc__Vidx = 0; dnpc__Vidx < 1; ++dnpc__Vidx) dnpc__Vcvt = dnpc;
    update_inst_npc(inst__Vcvt, dnpc__Vcvt);
}

extern "C" void write_gpr_npc(const svLogicVecVal* idx, const svLogicVecVal* data);

VL_INLINE_OPT void Vtop___024root____Vdpiimwrap_top__DOT__u_gpr__DOT__write_gpr_npc_TOP(CData/*4:0*/ idx, IData/*31:0*/ data) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root____Vdpiimwrap_top__DOT__u_gpr__DOT__write_gpr_npc_TOP\n"); );
    // Body
    svLogicVecVal idx__Vcvt[1];
    for (size_t idx__Vidx = 0; idx__Vidx < 1; ++idx__Vidx) VL_SET_SVLV_I(5, idx__Vcvt + 1 * idx__Vidx, idx);
    svLogicVecVal data__Vcvt[1];
    for (size_t data__Vidx = 0; data__Vidx < 1; ++data__Vidx) VL_SET_SVLV_I(32, data__Vcvt + 1 * data__Vidx, data);
    write_gpr_npc(idx__Vcvt, data__Vcvt);
}

extern "C" void NPCTRAP();

VL_INLINE_OPT void Vtop___024root____Vdpiimwrap_top__DOT__u_decode__DOT__NPCTRAP_TOP() {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root____Vdpiimwrap_top__DOT__u_decode__DOT__NPCTRAP_TOP\n"); );
    // Body
    NPCTRAP();
}

extern "C" void pmem_write_npc(int waddr, int wdata, char wmask);

VL_INLINE_OPT void Vtop___024root____Vdpiimwrap_top__DOT__u_exe__DOT__pmem_write_npc_TOP(IData/*31:0*/ waddr, IData/*31:0*/ wdata, CData/*7:0*/ wmask) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root____Vdpiimwrap_top__DOT__u_exe__DOT__pmem_write_npc_TOP\n"); );
    // Body
    int waddr__Vcvt;
    for (size_t waddr__Vidx = 0; waddr__Vidx < 1; ++waddr__Vidx) waddr__Vcvt = waddr;
    int wdata__Vcvt;
    for (size_t wdata__Vidx = 0; wdata__Vidx < 1; ++wdata__Vidx) wdata__Vcvt = wdata;
    char wmask__Vcvt;
    for (size_t wmask__Vidx = 0; wmask__Vidx < 1; ++wmask__Vidx) wmask__Vcvt = wmask;
    pmem_write_npc(waddr__Vcvt, wdata__Vcvt, wmask__Vcvt);
}

extern "C" void NPCINV(int pc);

VL_INLINE_OPT void Vtop___024root____Vdpiimwrap_top__DOT__u_exe__DOT__NPCINV_TOP(IData/*31:0*/ pc) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root____Vdpiimwrap_top__DOT__u_exe__DOT__NPCINV_TOP\n"); );
    // Body
    int pc__Vcvt;
    for (size_t pc__Vidx = 0; pc__Vidx < 1; ++pc__Vidx) pc__Vcvt = pc;
    NPCINV(pc__Vcvt);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void Vtop___024root___dump_triggers__act(Vtop___024root* vlSelf);
#endif  // VL_DEBUG

void Vtop___024root___eval_triggers__act(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_triggers__act\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VactTriggered.set(0U, ((IData)(vlSelfRef.clk) 
                                       & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__clk__0))));
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        Vtop___024root___dump_triggers__act(vlSelf);
    }
#endif
}
