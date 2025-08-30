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
        vlSelfRef.__Vm_traceActivity[1U] = 1U;
    }
}

void Vtop___024root____Vdpiimwrap_top__DOT__pmem_read_npc_TOP(IData/*31:0*/ raddr, IData/*31:0*/ &pmem_read_npc__Vfuncrtn);
void Vtop___024root____Vdpiimwrap_top__DOT__u_decode__DOT__NPCTRAP_TOP();
extern const VlUnpacked<CData/*2:0*/, 128> Vtop__ConstPool__TABLE_hd866e14c_0;
void Vtop___024root____Vdpiimwrap_top__DOT__update_inst_npc_TOP(IData/*31:0*/ inst, IData/*31:0*/ dnpc);
void Vtop___024root____Vdpiimwrap_top__DOT__u_exe__DOT__NPCINV_TOP(IData/*31:0*/ pc);
void Vtop___024root____Vdpiimwrap_top__DOT__u_exe__DOT__pmem_write_npc_TOP(IData/*31:0*/ waddr, IData/*31:0*/ wdata, CData/*7:0*/ wmask);
void Vtop___024root____Vdpiimwrap_top__DOT__u_gpr__DOT__write_gpr_npc_TOP(CData/*4:0*/ idx, IData/*31:0*/ data);

VL_INLINE_OPT void Vtop___024root___nba_sequent__TOP__0(Vtop___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root___nba_sequent__TOP__0\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*0:0*/ __VdlySet__top__DOT__u_gpr__DOT__regfile__v0;
    __VdlySet__top__DOT__u_gpr__DOT__regfile__v0 = 0;
    IData/*31:0*/ __VdlyVal__top__DOT__u_gpr__DOT__regfile__v32;
    __VdlyVal__top__DOT__u_gpr__DOT__regfile__v32 = 0;
    CData/*4:0*/ __VdlyDim0__top__DOT__u_gpr__DOT__regfile__v32;
    __VdlyDim0__top__DOT__u_gpr__DOT__regfile__v32 = 0;
    CData/*0:0*/ __VdlySet__top__DOT__u_gpr__DOT__regfile__v32;
    __VdlySet__top__DOT__u_gpr__DOT__regfile__v32 = 0;
    // Body
    __VdlySet__top__DOT__u_gpr__DOT__regfile__v0 = 0U;
    __VdlySet__top__DOT__u_gpr__DOT__regfile__v32 = 0U;
    if (vlSelfRef.top__DOT__reset_sync) {
        vlSelfRef.top__DOT__u_gpr__DOT__unnamedblk1__DOT__i = 0x20U;
        __VdlySet__top__DOT__u_gpr__DOT__regfile__v0 = 1U;
        vlSelfRef.top__DOT__pc = 0x80000000U;
    } else {
        if ((((0U != (0x1fU & (vlSelfRef.top__DOT__inst 
                               >> 7U))) & ((0U == (IData)(vlSelfRef.top__DOT__inst_type)) 
                                           | ((1U == (IData)(vlSelfRef.top__DOT__inst_type)) 
                                              | ((4U 
                                                  == (IData)(vlSelfRef.top__DOT__inst_type)) 
                                                 | (5U 
                                                    == (IData)(vlSelfRef.top__DOT__inst_type)))))) 
             & (0U != (0x1fU & (vlSelfRef.top__DOT__inst 
                                >> 7U))))) {
            __VdlyVal__top__DOT__u_gpr__DOT__regfile__v32 
                = vlSelfRef.top__DOT__wdata;
            __VdlyDim0__top__DOT__u_gpr__DOT__regfile__v32 
                = (0x1fU & (vlSelfRef.top__DOT__inst 
                            >> 7U));
            __VdlySet__top__DOT__u_gpr__DOT__regfile__v32 = 1U;
        }
        vlSelfRef.top__DOT__pc = vlSelfRef.top__DOT__dnpc;
    }
    if (__VdlySet__top__DOT__u_gpr__DOT__regfile__v0) {
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[0U] = 0U;
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[1U] = 0U;
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[2U] = 0U;
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[3U] = 0U;
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[4U] = 0U;
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[5U] = 0U;
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[6U] = 0U;
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[7U] = 0U;
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[8U] = 0U;
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[9U] = 0U;
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[0xaU] = 0U;
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[0xbU] = 0U;
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[0xcU] = 0U;
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[0xdU] = 0U;
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[0xeU] = 0U;
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[0xfU] = 0U;
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[0x10U] = 0U;
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[0x11U] = 0U;
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[0x12U] = 0U;
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[0x13U] = 0U;
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[0x14U] = 0U;
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[0x15U] = 0U;
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[0x16U] = 0U;
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[0x17U] = 0U;
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[0x18U] = 0U;
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[0x19U] = 0U;
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[0x1aU] = 0U;
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[0x1bU] = 0U;
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[0x1cU] = 0U;
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[0x1dU] = 0U;
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[0x1eU] = 0U;
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[0x1fU] = 0U;
    }
    if (__VdlySet__top__DOT__u_gpr__DOT__regfile__v32) {
        vlSelfRef.top__DOT__u_gpr__DOT__regfile[__VdlyDim0__top__DOT__u_gpr__DOT__regfile__v32] 
            = __VdlyVal__top__DOT__u_gpr__DOT__regfile__v32;
    }
    vlSelfRef.top__DOT__reset_sync = vlSelfRef.reset;
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
            VL_FATAL_MT("/home/saten/Code/Other/ysyx-workbench/npc/vsrc/top.sv", 85, "", "NBA region did not converge.");
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
                VL_FATAL_MT("/home/saten/Code/Other/ysyx-workbench/npc/vsrc/top.sv", 85, "", "Active region did not converge.");
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
    if (VL_UNLIKELY(((vlSelfRef.reset & 0xfeU)))) {
        Verilated::overWidthError("reset");}
}
#endif  // VL_DEBUG
