// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_fst_c.h"
#include "Vtop__Syms.h"


void Vtop___024root__trace_chg_0_sub_0(Vtop___024root* vlSelf, VerilatedFst::Buffer* bufp);

void Vtop___024root__trace_chg_0(void* voidSelf, VerilatedFst::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root__trace_chg_0\n"); );
    // Init
    Vtop___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vtop___024root*>(voidSelf);
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    if (VL_UNLIKELY(!vlSymsp->__Vm_activity)) return;
    // Body
    Vtop___024root__trace_chg_0_sub_0((&vlSymsp->TOP), bufp);
}

void Vtop___024root__trace_chg_0_sub_0(Vtop___024root* vlSelf, VerilatedFst::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root__trace_chg_0_sub_0\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode + 1);
    // Body
    if (VL_UNLIKELY((vlSelfRef.__Vm_traceActivity[1U]))) {
        bufp->chgBit(oldp+0,(vlSelfRef.top__DOT__reset_sync));
        bufp->chgCData(oldp+1,((0x1fU & (vlSelfRef.top__DOT__inst 
                                         >> 0xfU))),5);
        bufp->chgCData(oldp+2,((0x1fU & (vlSelfRef.top__DOT__inst 
                                         >> 0x14U))),5);
        bufp->chgCData(oldp+3,((0x1fU & (vlSelfRef.top__DOT__inst 
                                         >> 7U))),5);
        bufp->chgIData(oldp+4,(vlSelfRef.top__DOT__rdata1),32);
        bufp->chgIData(oldp+5,(vlSelfRef.top__DOT__rdata2),32);
        bufp->chgIData(oldp+6,(vlSelfRef.top__DOT__wdata),32);
        bufp->chgBit(oldp+7,(((0U != (0x1fU & (vlSelfRef.top__DOT__inst 
                                               >> 7U))) 
                              & ((0U == (IData)(vlSelfRef.top__DOT__inst_type)) 
                                 | ((1U == (IData)(vlSelfRef.top__DOT__inst_type)) 
                                    | ((4U == (IData)(vlSelfRef.top__DOT__inst_type)) 
                                       | (5U == (IData)(vlSelfRef.top__DOT__inst_type))))))));
        bufp->chgCData(oldp+8,((0x7fU & vlSelfRef.top__DOT__inst)),7);
        bufp->chgCData(oldp+9,((vlSelfRef.top__DOT__inst 
                                >> 0x19U)),7);
        bufp->chgCData(oldp+10,((7U & (vlSelfRef.top__DOT__inst 
                                       >> 0xcU))),3);
        bufp->chgIData(oldp+11,(vlSelfRef.top__DOT__imm),32);
        bufp->chgCData(oldp+12,(vlSelfRef.top__DOT__inst_type),3);
        bufp->chgIData(oldp+13,(vlSelfRef.top__DOT__pc),32);
        bufp->chgIData(oldp+14,(vlSelfRef.top__DOT__snpc),32);
        bufp->chgIData(oldp+15,(vlSelfRef.top__DOT__dnpc),32);
        bufp->chgIData(oldp+16,(vlSelfRef.top__DOT__inst),32);
        bufp->chgIData(oldp+17,(vlSelfRef.top__DOT__jump_target),32);
        bufp->chgBit(oldp+18,(vlSelfRef.top__DOT__jump_en));
        bufp->chgIData(oldp+19,(vlSelfRef.top__DOT__u_exe__DOT__alu_a),32);
        bufp->chgIData(oldp+20,(vlSelfRef.top__DOT__u_exe__DOT__alu_b),32);
        bufp->chgIData(oldp+21,(vlSelfRef.top__DOT__u_exe__DOT__alu_result),32);
        bufp->chgCData(oldp+22,(vlSelfRef.top__DOT__u_exe__DOT__alu_op),4);
        bufp->chgIData(oldp+23,(vlSelfRef.top__DOT__u_exe__DOT__mem_rdata),32);
        bufp->chgBit(oldp+24,((1U & (~ (IData)((0U 
                                                != vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__add_res))))));
        bufp->chgBit(oldp+25,((((vlSelfRef.top__DOT__u_exe__DOT__alu_a 
                                 >> 0x1fU) == (vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__adder_inst__DOT__B1 
                                               >> 0x1fU)) 
                               & ((vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__add_res 
                                   >> 0x1fU) != (vlSelfRef.top__DOT__u_exe__DOT__alu_a 
                                                 >> 0x1fU)))));
        bufp->chgIData(oldp+26,(vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__add_res),32);
        bufp->chgIData(oldp+27,((~ vlSelfRef.top__DOT__u_exe__DOT__alu_a)),32);
        bufp->chgIData(oldp+28,((vlSelfRef.top__DOT__u_exe__DOT__alu_a 
                                 & vlSelfRef.top__DOT__u_exe__DOT__alu_b)),32);
        bufp->chgIData(oldp+29,((vlSelfRef.top__DOT__u_exe__DOT__alu_a 
                                 | vlSelfRef.top__DOT__u_exe__DOT__alu_b)),32);
        bufp->chgIData(oldp+30,((vlSelfRef.top__DOT__u_exe__DOT__alu_a 
                                 ^ vlSelfRef.top__DOT__u_exe__DOT__alu_b)),32);
        bufp->chgIData(oldp+31,((vlSelfRef.top__DOT__u_exe__DOT__alu_a 
                                 << (0x1fU & vlSelfRef.top__DOT__u_exe__DOT__alu_b))),32);
        bufp->chgIData(oldp+32,((vlSelfRef.top__DOT__u_exe__DOT__alu_a 
                                 >> (0x1fU & vlSelfRef.top__DOT__u_exe__DOT__alu_b))),32);
        bufp->chgIData(oldp+33,(VL_SHIFTRS_III(32,32,5, vlSelfRef.top__DOT__u_exe__DOT__alu_a, 
                                               (0x1fU 
                                                & vlSelfRef.top__DOT__u_exe__DOT__alu_b))),32);
        bufp->chgBit(oldp+34,((1U & ((vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__add_res 
                                      >> 0x1fU) ^ (
                                                   ((vlSelfRef.top__DOT__u_exe__DOT__alu_a 
                                                     >> 0x1fU) 
                                                    == 
                                                    (vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__adder_inst__DOT__B1 
                                                     >> 0x1fU)) 
                                                   & ((vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__add_res 
                                                       >> 0x1fU) 
                                                      != 
                                                      (vlSelfRef.top__DOT__u_exe__DOT__alu_a 
                                                       >> 0x1fU)))))));
        bufp->chgBit(oldp+35,(((vlSelfRef.top__DOT__u_exe__DOT__alu_a 
                                >> 0x1fU) ? ((vlSelfRef.top__DOT__u_exe__DOT__alu_b 
                                              >> 0x1fU) 
                                             && (vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__add_res 
                                                 >> 0x1fU))
                                : ((vlSelfRef.top__DOT__u_exe__DOT__alu_b 
                                    >> 0x1fU) || (vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__add_res 
                                                  >> 0x1fU)))));
        bufp->chgBit(oldp+36,((0U != (IData)(vlSelfRef.top__DOT__u_exe__DOT__alu_op))));
        bufp->chgBit(oldp+37,((1U & (IData)((1ULL & 
                                             (((QData)((IData)(vlSelfRef.top__DOT__u_exe__DOT__alu_a)) 
                                               + ((QData)((IData)(vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__adder_inst__DOT__B1)) 
                                                  + (QData)((IData)(
                                                                    (0U 
                                                                     != (IData)(vlSelfRef.top__DOT__u_exe__DOT__alu_op)))))) 
                                              >> 0x20U))))));
        bufp->chgIData(oldp+38,(vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__adder_inst__DOT__B1),32);
        bufp->chgIData(oldp+39,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[0]),32);
        bufp->chgIData(oldp+40,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[1]),32);
        bufp->chgIData(oldp+41,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[2]),32);
        bufp->chgIData(oldp+42,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[3]),32);
        bufp->chgIData(oldp+43,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[4]),32);
        bufp->chgIData(oldp+44,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[5]),32);
        bufp->chgIData(oldp+45,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[6]),32);
        bufp->chgIData(oldp+46,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[7]),32);
        bufp->chgIData(oldp+47,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[8]),32);
        bufp->chgIData(oldp+48,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[9]),32);
        bufp->chgIData(oldp+49,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[10]),32);
        bufp->chgIData(oldp+50,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[11]),32);
        bufp->chgIData(oldp+51,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[12]),32);
        bufp->chgIData(oldp+52,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[13]),32);
        bufp->chgIData(oldp+53,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[14]),32);
        bufp->chgIData(oldp+54,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[15]),32);
        bufp->chgIData(oldp+55,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[16]),32);
        bufp->chgIData(oldp+56,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[17]),32);
        bufp->chgIData(oldp+57,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[18]),32);
        bufp->chgIData(oldp+58,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[19]),32);
        bufp->chgIData(oldp+59,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[20]),32);
        bufp->chgIData(oldp+60,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[21]),32);
        bufp->chgIData(oldp+61,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[22]),32);
        bufp->chgIData(oldp+62,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[23]),32);
        bufp->chgIData(oldp+63,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[24]),32);
        bufp->chgIData(oldp+64,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[25]),32);
        bufp->chgIData(oldp+65,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[26]),32);
        bufp->chgIData(oldp+66,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[27]),32);
        bufp->chgIData(oldp+67,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[28]),32);
        bufp->chgIData(oldp+68,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[29]),32);
        bufp->chgIData(oldp+69,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[30]),32);
        bufp->chgIData(oldp+70,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[31]),32);
        bufp->chgIData(oldp+71,(vlSelfRef.top__DOT__u_gpr__DOT__unnamedblk1__DOT__i),32);
    }
    bufp->chgBit(oldp+72,(vlSelfRef.clk));
    bufp->chgBit(oldp+73,(vlSelfRef.reset));
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
