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

VL_ATTR_COLD void Vtop___024root___eval_initial(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_initial\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
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
            VL_FATAL_MT("/home/saten/Code/Other/ysyx-workbench/npc/vsrc/top.sv", 85, "", "Settle region did not converge.");
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
VL_ATTR_COLD void Vtop___024root____Vm_traceActivitySetAll(Vtop___024root* vlSelf);

VL_ATTR_COLD void Vtop___024root___eval_stl(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___eval_stl\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1ULL & vlSelfRef.__VstlTriggered.word(0U))) {
        Vtop___024root___stl_sequent__TOP__0(vlSelf);
        Vtop___024root____Vm_traceActivitySetAll(vlSelf);
    }
}

void Vtop___024root____Vdpiimwrap_top__DOT__pmem_read_npc_TOP(IData/*31:0*/ raddr, IData/*31:0*/ &pmem_read_npc__Vfuncrtn);
void Vtop___024root____Vdpiimwrap_top__DOT__u_decode__DOT__NPCTRAP_TOP();
extern const VlUnpacked<CData/*2:0*/, 128> Vtop__ConstPool__TABLE_hd866e14c_0;
void Vtop___024root____Vdpiimwrap_top__DOT__update_inst_npc_TOP(IData/*31:0*/ inst, IData/*31:0*/ dnpc);
void Vtop___024root____Vdpiimwrap_top__DOT__u_exe__DOT__NPCINV_TOP(IData/*31:0*/ pc);
void Vtop___024root____Vdpiimwrap_top__DOT__u_exe__DOT__pmem_write_npc_TOP(IData/*31:0*/ waddr, IData/*31:0*/ wdata, CData/*7:0*/ wmask);
void Vtop___024root____Vdpiimwrap_top__DOT__u_gpr__DOT__write_gpr_npc_TOP(CData/*4:0*/ idx, IData/*31:0*/ data);

VL_ATTR_COLD void Vtop___024root___stl_sequent__TOP__0(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___stl_sequent__TOP__0\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vtop___024root____Vdpiimwrap_top__DOT__pmem_read_npc_TOP(vlSelfRef.top__DOT__pc, vlSelfRef.__Vfunc_top__DOT__pmem_read_npc__0__Vfuncout);
    vlSelfRef.top__DOT__inst = vlSelfRef.__Vfunc_top__DOT__pmem_read_npc__0__Vfuncout;
    if ((0x100073U == vlSelfRef.top__DOT__inst)) {
        Vtop___024root____Vdpiimwrap_top__DOT__u_decode__DOT__NPCTRAP_TOP();
    }
    vlSelfRef.top__DOT__rdata1 = ((0U == (0x1fU & (vlSelfRef.top__DOT__inst 
                                                   >> 0xfU)))
                                   ? 0U : vlSelfRef.top__DOT__u_gpr__DOT__regfile
                                  [(0x1fU & (vlSelfRef.top__DOT__inst 
                                             >> 0xfU))]);
    vlSelfRef.top__DOT__rdata2 = ((0U == (0x1fU & (vlSelfRef.top__DOT__inst 
                                                   >> 0x14U)))
                                   ? 0U : vlSelfRef.top__DOT__u_gpr__DOT__regfile
                                  [(0x1fU & (vlSelfRef.top__DOT__inst 
                                             >> 0x14U))]);
    vlSelfRef.__Vtableidx1 = (0x7fU & vlSelfRef.top__DOT__inst);
    vlSelfRef.top__DOT__inst_type = Vtop__ConstPool__TABLE_hd866e14c_0
        [vlSelfRef.__Vtableidx1];
    vlSelfRef.top__DOT__jump_en = 0U;
    vlSelfRef.top__DOT__u_exe__DOT__alu_op = 0U;
    if (((0x13U == (0x7fU & vlSelfRef.top__DOT__inst)) 
         | (0x33U == (0x7fU & vlSelfRef.top__DOT__inst)))) {
        if ((0x4000U & vlSelfRef.top__DOT__inst)) {
            vlSelfRef.top__DOT__u_exe__DOT__alu_op 
                = ((0x2000U & vlSelfRef.top__DOT__inst)
                    ? ((0x1000U & vlSelfRef.top__DOT__inst)
                        ? 3U : 4U) : ((0x1000U & vlSelfRef.top__DOT__inst)
                                       ? ((0x40000000U 
                                           & vlSelfRef.top__DOT__inst)
                                           ? 0xbU : 0xaU)
                                       : 5U));
        } else if ((0x2000U & vlSelfRef.top__DOT__inst)) {
            vlSelfRef.top__DOT__u_exe__DOT__alu_op 
                = ((0x1000U & vlSelfRef.top__DOT__inst)
                    ? 8U : 0U);
        } else if ((0x1000U & vlSelfRef.top__DOT__inst)) {
            vlSelfRef.top__DOT__u_exe__DOT__alu_op = 9U;
        } else if (((0U == (IData)(vlSelfRef.top__DOT__inst_type)) 
                    & (0x20U == (vlSelfRef.top__DOT__inst 
                                 >> 0x19U)))) {
            vlSelfRef.top__DOT__u_exe__DOT__alu_op = 1U;
        }
    }
    if ((4U & (IData)(vlSelfRef.top__DOT__inst_type))) {
        if ((1U & (~ ((IData)(vlSelfRef.top__DOT__inst_type) 
                      >> 1U)))) {
            if ((1U & (IData)(vlSelfRef.top__DOT__inst_type))) {
                vlSelfRef.top__DOT__jump_en = 1U;
            }
        }
        if ((2U & (IData)(vlSelfRef.top__DOT__inst_type))) {
            vlSelfRef.top__DOT__u_exe__DOT__alu_a = vlSelfRef.top__DOT__rdata1;
            vlSelfRef.top__DOT__imm = 0U;
            vlSelfRef.top__DOT__u_exe__DOT__alu_b = vlSelfRef.top__DOT__rdata2;
        } else {
            if ((1U & (IData)(vlSelfRef.top__DOT__inst_type))) {
                vlSelfRef.top__DOT__u_exe__DOT__alu_a 
                    = vlSelfRef.top__DOT__pc;
                vlSelfRef.top__DOT__imm = ((((- (IData)(
                                                        (vlSelfRef.top__DOT__inst 
                                                         >> 0x1fU))) 
                                             << 0x15U) 
                                            | (0x100000U 
                                               & (vlSelfRef.top__DOT__inst 
                                                  >> 0xbU))) 
                                           | (((0xff000U 
                                                & vlSelfRef.top__DOT__inst) 
                                               | (0x800U 
                                                  & (vlSelfRef.top__DOT__inst 
                                                     >> 9U))) 
                                              | (0x7feU 
                                                 & (vlSelfRef.top__DOT__inst 
                                                    >> 0x14U))));
            } else {
                vlSelfRef.top__DOT__u_exe__DOT__alu_a 
                    = ((0x37U == (0x7fU & vlSelfRef.top__DOT__inst))
                        ? 0U : vlSelfRef.top__DOT__pc);
                vlSelfRef.top__DOT__imm = (0xfffff000U 
                                           & vlSelfRef.top__DOT__inst);
            }
            vlSelfRef.top__DOT__u_exe__DOT__alu_b = vlSelfRef.top__DOT__imm;
        }
    } else if ((2U & (IData)(vlSelfRef.top__DOT__inst_type))) {
        if ((1U & (IData)(vlSelfRef.top__DOT__inst_type))) {
            vlSelfRef.top__DOT__jump_en = ((0x4000U 
                                            & vlSelfRef.top__DOT__inst)
                                            ? ((0x2000U 
                                                & vlSelfRef.top__DOT__inst)
                                                ? (
                                                   (0x1000U 
                                                    & vlSelfRef.top__DOT__inst)
                                                    ? 
                                                   (vlSelfRef.top__DOT__rdata1 
                                                    >= vlSelfRef.top__DOT__rdata2)
                                                    : 
                                                   (vlSelfRef.top__DOT__rdata1 
                                                    < vlSelfRef.top__DOT__rdata2))
                                                : (
                                                   (0x1000U 
                                                    & vlSelfRef.top__DOT__inst)
                                                    ? 
                                                   VL_GTES_III(32, vlSelfRef.top__DOT__rdata1, vlSelfRef.top__DOT__rdata2)
                                                    : 
                                                   VL_LTS_III(32, vlSelfRef.top__DOT__rdata1, vlSelfRef.top__DOT__rdata2)))
                                            : ((1U 
                                                & (~ 
                                                   (vlSelfRef.top__DOT__inst 
                                                    >> 0xdU))) 
                                               && ((0x1000U 
                                                    & vlSelfRef.top__DOT__inst)
                                                    ? 
                                                   (vlSelfRef.top__DOT__rdata1 
                                                    != vlSelfRef.top__DOT__rdata2)
                                                    : 
                                                   (vlSelfRef.top__DOT__rdata1 
                                                    == vlSelfRef.top__DOT__rdata2))));
            vlSelfRef.top__DOT__u_exe__DOT__alu_a = vlSelfRef.top__DOT__pc;
            vlSelfRef.top__DOT__imm = (((- (IData)(
                                                   (vlSelfRef.top__DOT__inst 
                                                    >> 0x1fU))) 
                                        << 0xdU) | 
                                       (((0x1000U & 
                                          (vlSelfRef.top__DOT__inst 
                                           >> 0x13U)) 
                                         | (0x800U 
                                            & (vlSelfRef.top__DOT__inst 
                                               << 4U))) 
                                        | ((0x7e0U 
                                            & (vlSelfRef.top__DOT__inst 
                                               >> 0x14U)) 
                                           | (0x1eU 
                                              & (vlSelfRef.top__DOT__inst 
                                                 >> 7U)))));
            vlSelfRef.top__DOT__u_exe__DOT__alu_b = vlSelfRef.top__DOT__imm;
        } else {
            vlSelfRef.top__DOT__u_exe__DOT__alu_a = vlSelfRef.top__DOT__rdata1;
            vlSelfRef.top__DOT__imm = (((- (IData)(
                                                   (vlSelfRef.top__DOT__inst 
                                                    >> 0x1fU))) 
                                        << 0xcU) | 
                                       ((0xfe0U & (vlSelfRef.top__DOT__inst 
                                                   >> 0x14U)) 
                                        | (0x1fU & 
                                           (vlSelfRef.top__DOT__inst 
                                            >> 7U))));
            vlSelfRef.top__DOT__u_exe__DOT__alu_b = vlSelfRef.top__DOT__rdata2;
        }
    } else {
        if ((1U & (IData)(vlSelfRef.top__DOT__inst_type))) {
            if ((0x67U == (0x7fU & vlSelfRef.top__DOT__inst))) {
                vlSelfRef.top__DOT__jump_en = 1U;
            }
            vlSelfRef.top__DOT__imm = (((- (IData)(
                                                   (vlSelfRef.top__DOT__inst 
                                                    >> 0x1fU))) 
                                        << 0xcU) | 
                                       (vlSelfRef.top__DOT__inst 
                                        >> 0x14U));
            vlSelfRef.top__DOT__u_exe__DOT__alu_b = vlSelfRef.top__DOT__imm;
        } else {
            vlSelfRef.top__DOT__imm = 0U;
            vlSelfRef.top__DOT__u_exe__DOT__alu_b = vlSelfRef.top__DOT__rdata2;
        }
        vlSelfRef.top__DOT__u_exe__DOT__alu_a = vlSelfRef.top__DOT__rdata1;
    }
    vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__adder_inst__DOT__B1 
        = ((- (IData)((0U != (IData)(vlSelfRef.top__DOT__u_exe__DOT__alu_op)))) 
           ^ vlSelfRef.top__DOT__u_exe__DOT__alu_b);
    vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__add_res 
        = (vlSelfRef.top__DOT__u_exe__DOT__alu_a + 
           (vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__adder_inst__DOT__B1 
            + (0U != (IData)(vlSelfRef.top__DOT__u_exe__DOT__alu_op))));
    vlSelfRef.top__DOT__u_exe__DOT__alu_result = ((8U 
                                                   & (IData)(vlSelfRef.top__DOT__u_exe__DOT__alu_op))
                                                   ? 
                                                  ((4U 
                                                    & (IData)(vlSelfRef.top__DOT__u_exe__DOT__alu_op))
                                                    ? 0U
                                                    : 
                                                   ((2U 
                                                     & (IData)(vlSelfRef.top__DOT__u_exe__DOT__alu_op))
                                                     ? 
                                                    ((1U 
                                                      & (IData)(vlSelfRef.top__DOT__u_exe__DOT__alu_op))
                                                      ? 
                                                     VL_SHIFTRS_III(32,32,5, vlSelfRef.top__DOT__u_exe__DOT__alu_a, 
                                                                    (0x1fU 
                                                                     & vlSelfRef.top__DOT__u_exe__DOT__alu_b))
                                                      : 
                                                     (vlSelfRef.top__DOT__u_exe__DOT__alu_a 
                                                      >> 
                                                      (0x1fU 
                                                       & vlSelfRef.top__DOT__u_exe__DOT__alu_b)))
                                                     : 
                                                    ((1U 
                                                      & (IData)(vlSelfRef.top__DOT__u_exe__DOT__alu_op))
                                                      ? 
                                                     (vlSelfRef.top__DOT__u_exe__DOT__alu_a 
                                                      << 
                                                      (0x1fU 
                                                       & vlSelfRef.top__DOT__u_exe__DOT__alu_b))
                                                      : 
                                                     ((vlSelfRef.top__DOT__u_exe__DOT__alu_a 
                                                       >> 0x1fU)
                                                       ? 
                                                      ((vlSelfRef.top__DOT__u_exe__DOT__alu_b 
                                                        >> 0x1fU) 
                                                       && (vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__add_res 
                                                           >> 0x1fU))
                                                       : 
                                                      ((vlSelfRef.top__DOT__u_exe__DOT__alu_b 
                                                        >> 0x1fU) 
                                                       || (vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__add_res 
                                                           >> 0x1fU))))))
                                                   : 
                                                  ((4U 
                                                    & (IData)(vlSelfRef.top__DOT__u_exe__DOT__alu_op))
                                                    ? 
                                                   ((2U 
                                                     & (IData)(vlSelfRef.top__DOT__u_exe__DOT__alu_op))
                                                     ? 
                                                    ((1U 
                                                      & (IData)(vlSelfRef.top__DOT__u_exe__DOT__alu_op))
                                                      ? 
                                                     (1U 
                                                      & ((vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__add_res 
                                                          >> 0x1fU) 
                                                         ^ 
                                                         (((vlSelfRef.top__DOT__u_exe__DOT__alu_a 
                                                            >> 0x1fU) 
                                                           == 
                                                           (vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__adder_inst__DOT__B1 
                                                            >> 0x1fU)) 
                                                          & ((vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__add_res 
                                                              >> 0x1fU) 
                                                             != 
                                                             (vlSelfRef.top__DOT__u_exe__DOT__alu_a 
                                                              >> 0x1fU)))))
                                                      : 
                                                     (1U 
                                                      & (~ (IData)(
                                                                   (0U 
                                                                    != vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__add_res)))))
                                                     : 
                                                    ((1U 
                                                      & (IData)(vlSelfRef.top__DOT__u_exe__DOT__alu_op))
                                                      ? 
                                                     (vlSelfRef.top__DOT__u_exe__DOT__alu_a 
                                                      ^ vlSelfRef.top__DOT__u_exe__DOT__alu_b)
                                                      : 
                                                     (vlSelfRef.top__DOT__u_exe__DOT__alu_a 
                                                      | vlSelfRef.top__DOT__u_exe__DOT__alu_b)))
                                                    : 
                                                   ((2U 
                                                     & (IData)(vlSelfRef.top__DOT__u_exe__DOT__alu_op))
                                                     ? 
                                                    ((1U 
                                                      & (IData)(vlSelfRef.top__DOT__u_exe__DOT__alu_op))
                                                      ? 
                                                     (vlSelfRef.top__DOT__u_exe__DOT__alu_a 
                                                      & vlSelfRef.top__DOT__u_exe__DOT__alu_b)
                                                      : 
                                                     (~ vlSelfRef.top__DOT__u_exe__DOT__alu_a))
                                                     : vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__add_res)));
    vlSelfRef.top__DOT__jump_target = 0U;
    vlSelfRef.top__DOT__snpc = ((IData)(4U) + vlSelfRef.top__DOT__pc);
    if ((4U & (IData)(vlSelfRef.top__DOT__inst_type))) {
        if ((1U & (~ ((IData)(vlSelfRef.top__DOT__inst_type) 
                      >> 1U)))) {
            if ((1U & (IData)(vlSelfRef.top__DOT__inst_type))) {
                vlSelfRef.top__DOT__jump_target = vlSelfRef.top__DOT__u_exe__DOT__alu_result;
            }
        }
    } else if ((2U & (IData)(vlSelfRef.top__DOT__inst_type))) {
        if ((1U & (IData)(vlSelfRef.top__DOT__inst_type))) {
            vlSelfRef.top__DOT__jump_target = vlSelfRef.top__DOT__u_exe__DOT__alu_result;
        }
    } else if ((1U & (IData)(vlSelfRef.top__DOT__inst_type))) {
        if ((0x67U == (0x7fU & vlSelfRef.top__DOT__inst))) {
            vlSelfRef.top__DOT__jump_target = (0xfffffffeU 
                                               & vlSelfRef.top__DOT__u_exe__DOT__alu_result);
        }
    }
    vlSelfRef.top__DOT__dnpc = ((IData)(vlSelfRef.top__DOT__jump_en)
                                 ? vlSelfRef.top__DOT__jump_target
                                 : vlSelfRef.top__DOT__snpc);
    Vtop___024root____Vdpiimwrap_top__DOT__update_inst_npc_TOP(vlSelfRef.top__DOT__inst, vlSelfRef.top__DOT__dnpc);
    vlSelfRef.top__DOT__u_exe__DOT__mem_rdata = 0U;
    if ((4U & (IData)(vlSelfRef.top__DOT__inst_type))) {
        if ((2U & (IData)(vlSelfRef.top__DOT__inst_type))) {
            if ((1U & (IData)(vlSelfRef.top__DOT__inst_type))) {
                vlSelfRef.top__DOT__wdata = 0U;
                Vtop___024root____Vdpiimwrap_top__DOT__u_exe__DOT__NPCINV_TOP(vlSelfRef.top__DOT__pc);
            } else {
                vlSelfRef.top__DOT__wdata = 0U;
            }
        } else {
            vlSelfRef.top__DOT__wdata = ((1U & (IData)(vlSelfRef.top__DOT__inst_type))
                                          ? vlSelfRef.top__DOT__snpc
                                          : vlSelfRef.top__DOT__u_exe__DOT__alu_result);
        }
    } else if ((2U & (IData)(vlSelfRef.top__DOT__inst_type))) {
        if ((1U & (IData)(vlSelfRef.top__DOT__inst_type))) {
            vlSelfRef.top__DOT__wdata = 0U;
        } else {
            if ((0U == (7U & (vlSelfRef.top__DOT__inst 
                              >> 0xcU)))) {
                Vtop___024root____Vdpiimwrap_top__DOT__u_exe__DOT__pmem_write_npc_TOP(
                                                                                (vlSelfRef.top__DOT__rdata1 
                                                                                + vlSelfRef.top__DOT__imm), 
                                                                                (0xffU 
                                                                                & vlSelfRef.top__DOT__rdata2), 1U);
            } else if ((1U == (7U & (vlSelfRef.top__DOT__inst 
                                     >> 0xcU)))) {
                Vtop___024root____Vdpiimwrap_top__DOT__u_exe__DOT__pmem_write_npc_TOP(
                                                                                (vlSelfRef.top__DOT__rdata1 
                                                                                + vlSelfRef.top__DOT__imm), 
                                                                                (0xffffU 
                                                                                & vlSelfRef.top__DOT__rdata2), 3U);
            } else if ((2U == (7U & (vlSelfRef.top__DOT__inst 
                                     >> 0xcU)))) {
                Vtop___024root____Vdpiimwrap_top__DOT__u_exe__DOT__pmem_write_npc_TOP(
                                                                                (vlSelfRef.top__DOT__rdata1 
                                                                                + vlSelfRef.top__DOT__imm), vlSelfRef.top__DOT__rdata2, 0xfU);
            }
            vlSelfRef.top__DOT__wdata = 0U;
        }
    } else if ((1U & (IData)(vlSelfRef.top__DOT__inst_type))) {
        if ((0x67U == (0x7fU & vlSelfRef.top__DOT__inst))) {
            vlSelfRef.top__DOT__wdata = vlSelfRef.top__DOT__snpc;
        } else if ((3U == (0x7fU & vlSelfRef.top__DOT__inst))) {
            Vtop___024root____Vdpiimwrap_top__DOT__pmem_read_npc_TOP(
                                                                     (vlSelfRef.top__DOT__rdata1 
                                                                      + vlSelfRef.top__DOT__imm), vlSelfRef.__Vfunc_top__DOT__u_exe__DOT__pmem_read_npc__8__Vfuncout);
            vlSelfRef.top__DOT__u_exe__DOT__mem_rdata 
                = vlSelfRef.__Vfunc_top__DOT__u_exe__DOT__pmem_read_npc__8__Vfuncout;
            if ((0x4000U & vlSelfRef.top__DOT__inst)) {
                if ((0x2000U & vlSelfRef.top__DOT__inst)) {
                    Vtop___024root____Vdpiimwrap_top__DOT__u_exe__DOT__NPCINV_TOP(vlSelfRef.top__DOT__pc);
                    vlSelfRef.top__DOT__wdata = 0U;
                } else {
                    vlSelfRef.top__DOT__wdata = ((0x1000U 
                                                  & vlSelfRef.top__DOT__inst)
                                                  ? 
                                                 (0xffffU 
                                                  & vlSelfRef.top__DOT__u_exe__DOT__mem_rdata)
                                                  : 
                                                 (0xffU 
                                                  & vlSelfRef.top__DOT__u_exe__DOT__mem_rdata));
                }
            } else if ((0x2000U & vlSelfRef.top__DOT__inst)) {
                if ((0x1000U & vlSelfRef.top__DOT__inst)) {
                    Vtop___024root____Vdpiimwrap_top__DOT__u_exe__DOT__NPCINV_TOP(vlSelfRef.top__DOT__pc);
                    vlSelfRef.top__DOT__wdata = 0U;
                } else {
                    vlSelfRef.top__DOT__wdata = vlSelfRef.top__DOT__u_exe__DOT__mem_rdata;
                }
            } else if ((0x1000U & vlSelfRef.top__DOT__inst)) {
                vlSelfRef.top__DOT__wdata = (((- (IData)(
                                                         (1U 
                                                          & (vlSelfRef.top__DOT__u_exe__DOT__mem_rdata 
                                                             >> 0xfU)))) 
                                              << 0x10U) 
                                             | (0xffffU 
                                                & vlSelfRef.top__DOT__u_exe__DOT__mem_rdata));
            } else {
                Vtop___024root____Vdpiimwrap_top__DOT__u_exe__DOT__NPCINV_TOP(vlSelfRef.top__DOT__pc);
                vlSelfRef.top__DOT__wdata = 0U;
            }
        } else if ((0x4000U & vlSelfRef.top__DOT__inst)) {
            vlSelfRef.top__DOT__wdata = vlSelfRef.top__DOT__u_exe__DOT__alu_result;
        } else if ((0x2000U & vlSelfRef.top__DOT__inst)) {
            if ((0x1000U & vlSelfRef.top__DOT__inst)) {
                vlSelfRef.top__DOT__wdata = vlSelfRef.top__DOT__u_exe__DOT__alu_result;
            } else {
                Vtop___024root____Vdpiimwrap_top__DOT__u_exe__DOT__NPCINV_TOP(vlSelfRef.top__DOT__pc);
                vlSelfRef.top__DOT__wdata = 0U;
            }
        } else {
            vlSelfRef.top__DOT__wdata = vlSelfRef.top__DOT__u_exe__DOT__alu_result;
        }
    } else if ((0x4000U & vlSelfRef.top__DOT__inst)) {
        vlSelfRef.top__DOT__wdata = vlSelfRef.top__DOT__u_exe__DOT__alu_result;
    } else if ((0x2000U & vlSelfRef.top__DOT__inst)) {
        if ((0x1000U & vlSelfRef.top__DOT__inst)) {
            vlSelfRef.top__DOT__wdata = vlSelfRef.top__DOT__u_exe__DOT__alu_result;
        } else {
            Vtop___024root____Vdpiimwrap_top__DOT__u_exe__DOT__NPCINV_TOP(vlSelfRef.top__DOT__pc);
            vlSelfRef.top__DOT__wdata = 0U;
        }
    } else {
        vlSelfRef.top__DOT__wdata = vlSelfRef.top__DOT__u_exe__DOT__alu_result;
    }
    if ((((0U != (0x1fU & (vlSelfRef.top__DOT__inst 
                           >> 7U))) & ((0U == (IData)(vlSelfRef.top__DOT__inst_type)) 
                                       | ((1U == (IData)(vlSelfRef.top__DOT__inst_type)) 
                                          | ((4U == (IData)(vlSelfRef.top__DOT__inst_type)) 
                                             | (5U 
                                                == (IData)(vlSelfRef.top__DOT__inst_type)))))) 
         & (0U != (0x1fU & (vlSelfRef.top__DOT__inst 
                            >> 7U))))) {
        Vtop___024root____Vdpiimwrap_top__DOT__u_gpr__DOT__write_gpr_npc_TOP(
                                                                             (0x1fU 
                                                                              & (vlSelfRef.top__DOT__inst 
                                                                                >> 7U)), vlSelfRef.top__DOT__wdata);
    }
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

VL_ATTR_COLD void Vtop___024root____Vm_traceActivitySetAll(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root____Vm_traceActivitySetAll\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__Vm_traceActivity[0U] = 1U;
    vlSelfRef.__Vm_traceActivity[1U] = 1U;
}

VL_ATTR_COLD void Vtop___024root___ctor_var_reset(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___ctor_var_reset\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelf->clk = VL_RAND_RESET_I(1);
    vlSelf->reset = VL_RAND_RESET_I(1);
    vlSelf->top__DOT__reset_sync = VL_RAND_RESET_I(1);
    vlSelf->top__DOT__rdata1 = VL_RAND_RESET_I(32);
    vlSelf->top__DOT__rdata2 = VL_RAND_RESET_I(32);
    vlSelf->top__DOT__wdata = VL_RAND_RESET_I(32);
    vlSelf->top__DOT__imm = VL_RAND_RESET_I(32);
    vlSelf->top__DOT__inst_type = VL_RAND_RESET_I(3);
    vlSelf->top__DOT__pc = VL_RAND_RESET_I(32);
    vlSelf->top__DOT__snpc = VL_RAND_RESET_I(32);
    vlSelf->top__DOT__dnpc = VL_RAND_RESET_I(32);
    vlSelf->top__DOT__inst = VL_RAND_RESET_I(32);
    vlSelf->top__DOT__jump_target = VL_RAND_RESET_I(32);
    vlSelf->top__DOT__jump_en = VL_RAND_RESET_I(1);
    for (int __Vi0 = 0; __Vi0 < 32; ++__Vi0) {
        vlSelf->top__DOT__u_gpr__DOT__regfile[__Vi0] = VL_RAND_RESET_I(32);
    }
    vlSelf->top__DOT__u_gpr__DOT__unnamedblk1__DOT__i = 0;
    vlSelf->top__DOT__u_exe__DOT__alu_a = VL_RAND_RESET_I(32);
    vlSelf->top__DOT__u_exe__DOT__alu_b = VL_RAND_RESET_I(32);
    vlSelf->top__DOT__u_exe__DOT__alu_result = VL_RAND_RESET_I(32);
    vlSelf->top__DOT__u_exe__DOT__alu_op = VL_RAND_RESET_I(4);
    vlSelf->top__DOT__u_exe__DOT__mem_rdata = 0;
    vlSelf->top__DOT__u_exe__DOT__u_alu__DOT__add_res = VL_RAND_RESET_I(32);
    vlSelf->top__DOT__u_exe__DOT__u_alu__DOT__adder_inst__DOT__B1 = VL_RAND_RESET_I(32);
    vlSelf->__Vfunc_top__DOT__pmem_read_npc__0__Vfuncout = 0;
    vlSelf->__Vfunc_top__DOT__u_exe__DOT__pmem_read_npc__8__Vfuncout = 0;
    vlSelf->__Vtableidx1 = 0;
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = VL_RAND_RESET_I(1);
    for (int __Vi0 = 0; __Vi0 < 2; ++__Vi0) {
        vlSelf->__Vm_traceActivity[__Vi0] = 0;
    }
}
