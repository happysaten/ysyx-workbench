// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Tracing implementation internals
#include "verilated_fst_c.h"
#include "Vtop__Syms.h"


VL_ATTR_COLD void Vtop___024root__trace_init_sub__TOP__0(Vtop___024root* vlSelf, VerilatedFst* tracep) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root__trace_init_sub__TOP__0\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    const int c = vlSymsp->__Vm_baseCode;
    // Body
    tracep->declBit(c+73,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+74,0,"reset",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->pushPrefix("top", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBit(c+73,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+74,0,"reset",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+1,0,"reset_sync",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+2,0,"rs1",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+3,0,"rs2",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+4,0,"rd",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+5,0,"rdata1",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+6,0,"rdata2",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+7,0,"wdata",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBit(c+8,0,"we",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+9,0,"opcode",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 6,0);
    tracep->declBus(c+10,0,"funct7",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 6,0);
    tracep->declBus(c+11,0,"funct3",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+12,0,"imm",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+13,0,"inst_type",1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+75,0,"RESET_PC",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBus(c+14,0,"pc",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+15,0,"snpc",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+16,0,"dnpc",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+17,0,"inst",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+18,0,"jump_target",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBit(c+19,0,"jump_en",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->pushPrefix("u_decode", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+14,0,"pc",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+17,0,"inst",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+9,0,"opcode",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 6,0);
    tracep->declBus(c+11,0,"funct3",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+10,0,"funct7",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 6,0);
    tracep->declBus(c+4,0,"rd",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+2,0,"rs1",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+3,0,"rs2",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+12,0,"imm",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+13,0,"inst_type",1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->popPrefix();
    tracep->pushPrefix("u_exe", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+9,0,"opcode",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 6,0);
    tracep->declBus(c+11,0,"funct3",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+10,0,"funct7",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 6,0);
    tracep->declBus(c+5,0,"src1",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+6,0,"src2",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+12,0,"imm",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+14,0,"pc",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+15,0,"snpc",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+13,0,"inst_type",1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 2,0);
    tracep->declBus(c+7,0,"dest",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+18,0,"jump_target",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBit(c+19,0,"jump_en",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+20,0,"alu_a",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+21,0,"alu_b",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+22,0,"alu_result",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+23,0,"alu_op",2, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 3,0);
    tracep->declBus(c+24,0,"mem_rdata",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->pushPrefix("u_alu", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+76,0,"WIDTH",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBus(c+77,0,"SHIFT_WIDTH",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBus(c+20,0,"A",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+21,0,"B",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+23,0,"ALUop",2, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 3,0);
    tracep->declBus(c+22,0,"Result",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBit(c+25,0,"Zero",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+26,0,"Overflow",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+27,0,"add_res",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+28,0,"not_res",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+29,0,"and_res",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+30,0,"or_res",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+31,0,"xor_res",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+32,0,"sll_res",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+33,0,"srl_res",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+34,0,"sra_res",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBit(c+35,0,"slt_res",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+36,0,"sltu_res",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+25,0,"eq_res",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->pushPrefix("adder_inst", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+76,0,"WIDTH",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::PARAMETER, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->declBus(c+20,0,"A",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+21,0,"B",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBit(c+37,0,"is_sub",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+38,0,"Carry",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+25,0,"Zero",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+26,0,"Overflow",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+27,0,"Result",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+39,0,"B1",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->pushPrefix("u_gpr", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBit(c+73,0,"clk",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+1,0,"reset",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBit(c+8,0,"we",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1);
    tracep->declBus(c+4,0,"waddr",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+7,0,"wdata",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+2,0,"raddr1",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+3,0,"raddr2",-1, VerilatedTraceSigDirection::INPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 4,0);
    tracep->declBus(c+5,0,"rdata1",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->declBus(c+6,0,"rdata2",-1, VerilatedTraceSigDirection::OUTPUT, VerilatedTraceSigKind::WIRE, VerilatedTraceSigType::LOGIC, false,-1, 31,0);
    tracep->pushPrefix("regfile", VerilatedTracePrefixType::ARRAY_UNPACKED);
    for (int i = 0; i < 32; ++i) {
        tracep->declBus(c+40+i*1,0,"",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::LOGIC, true,(i+0), 31,0);
    }
    tracep->popPrefix();
    tracep->pushPrefix("unnamedblk1", VerilatedTracePrefixType::SCOPE_MODULE);
    tracep->declBus(c+72,0,"i",-1, VerilatedTraceSigDirection::NONE, VerilatedTraceSigKind::VAR, VerilatedTraceSigType::INT, false,-1, 31,0);
    tracep->popPrefix();
    tracep->popPrefix();
    tracep->popPrefix();
}

VL_ATTR_COLD void Vtop___024root__trace_init_top(Vtop___024root* vlSelf, VerilatedFst* tracep) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root__trace_init_top\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    Vtop___024root__trace_init_sub__TOP__0(vlSelf, tracep);
}

VL_ATTR_COLD void Vtop___024root__trace_const_0(void* voidSelf, VerilatedFst::Buffer* bufp);
VL_ATTR_COLD void Vtop___024root__trace_full_0(void* voidSelf, VerilatedFst::Buffer* bufp);
void Vtop___024root__trace_chg_0(void* voidSelf, VerilatedFst::Buffer* bufp);
void Vtop___024root__trace_cleanup(void* voidSelf, VerilatedFst* /*unused*/);

VL_ATTR_COLD void Vtop___024root__trace_register(Vtop___024root* vlSelf, VerilatedFst* tracep) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root__trace_register\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    tracep->addConstCb(&Vtop___024root__trace_const_0, 0U, vlSelf);
    tracep->addFullCb(&Vtop___024root__trace_full_0, 0U, vlSelf);
    tracep->addChgCb(&Vtop___024root__trace_chg_0, 0U, vlSelf);
    tracep->addCleanupCb(&Vtop___024root__trace_cleanup, vlSelf);
}

VL_ATTR_COLD void Vtop___024root__trace_const_0_sub_0(Vtop___024root* vlSelf, VerilatedFst::Buffer* bufp);

VL_ATTR_COLD void Vtop___024root__trace_const_0(void* voidSelf, VerilatedFst::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root__trace_const_0\n"); );
    // Init
    Vtop___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vtop___024root*>(voidSelf);
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    Vtop___024root__trace_const_0_sub_0((&vlSymsp->TOP), bufp);
}

VL_ATTR_COLD void Vtop___024root__trace_const_0_sub_0(Vtop___024root* vlSelf, VerilatedFst::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root__trace_const_0_sub_0\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode);
    // Body
    bufp->fullIData(oldp+75,(0x80000000U),32);
    bufp->fullIData(oldp+76,(0x20U),32);
    bufp->fullIData(oldp+77,(5U),32);
}

VL_ATTR_COLD void Vtop___024root__trace_full_0_sub_0(Vtop___024root* vlSelf, VerilatedFst::Buffer* bufp);

VL_ATTR_COLD void Vtop___024root__trace_full_0(void* voidSelf, VerilatedFst::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root__trace_full_0\n"); );
    // Init
    Vtop___024root* const __restrict vlSelf VL_ATTR_UNUSED = static_cast<Vtop___024root*>(voidSelf);
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    // Body
    Vtop___024root__trace_full_0_sub_0((&vlSymsp->TOP), bufp);
}

VL_ATTR_COLD void Vtop___024root__trace_full_0_sub_0(Vtop___024root* vlSelf, VerilatedFst::Buffer* bufp) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    Vtop___024root__trace_full_0_sub_0\n"); );
    Vtop__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    uint32_t* const oldp VL_ATTR_UNUSED = bufp->oldp(vlSymsp->__Vm_baseCode);
    // Body
    bufp->fullBit(oldp+1,(vlSelfRef.top__DOT__reset_sync));
    bufp->fullCData(oldp+2,((0x1fU & (vlSelfRef.top__DOT__inst 
                                      >> 0xfU))),5);
    bufp->fullCData(oldp+3,((0x1fU & (vlSelfRef.top__DOT__inst 
                                      >> 0x14U))),5);
    bufp->fullCData(oldp+4,((0x1fU & (vlSelfRef.top__DOT__inst 
                                      >> 7U))),5);
    bufp->fullIData(oldp+5,(vlSelfRef.top__DOT__rdata1),32);
    bufp->fullIData(oldp+6,(vlSelfRef.top__DOT__rdata2),32);
    bufp->fullIData(oldp+7,(vlSelfRef.top__DOT__wdata),32);
    bufp->fullBit(oldp+8,(((0U != (0x1fU & (vlSelfRef.top__DOT__inst 
                                            >> 7U))) 
                           & ((0U == (IData)(vlSelfRef.top__DOT__inst_type)) 
                              | ((1U == (IData)(vlSelfRef.top__DOT__inst_type)) 
                                 | ((4U == (IData)(vlSelfRef.top__DOT__inst_type)) 
                                    | (5U == (IData)(vlSelfRef.top__DOT__inst_type))))))));
    bufp->fullCData(oldp+9,((0x7fU & vlSelfRef.top__DOT__inst)),7);
    bufp->fullCData(oldp+10,((vlSelfRef.top__DOT__inst 
                              >> 0x19U)),7);
    bufp->fullCData(oldp+11,((7U & (vlSelfRef.top__DOT__inst 
                                    >> 0xcU))),3);
    bufp->fullIData(oldp+12,(vlSelfRef.top__DOT__imm),32);
    bufp->fullCData(oldp+13,(vlSelfRef.top__DOT__inst_type),3);
    bufp->fullIData(oldp+14,(vlSelfRef.top__DOT__pc),32);
    bufp->fullIData(oldp+15,(vlSelfRef.top__DOT__snpc),32);
    bufp->fullIData(oldp+16,(vlSelfRef.top__DOT__dnpc),32);
    bufp->fullIData(oldp+17,(vlSelfRef.top__DOT__inst),32);
    bufp->fullIData(oldp+18,(vlSelfRef.top__DOT__jump_target),32);
    bufp->fullBit(oldp+19,(vlSelfRef.top__DOT__jump_en));
    bufp->fullIData(oldp+20,(vlSelfRef.top__DOT__u_exe__DOT__alu_a),32);
    bufp->fullIData(oldp+21,(vlSelfRef.top__DOT__u_exe__DOT__alu_b),32);
    bufp->fullIData(oldp+22,(vlSelfRef.top__DOT__u_exe__DOT__alu_result),32);
    bufp->fullCData(oldp+23,(vlSelfRef.top__DOT__u_exe__DOT__alu_op),4);
    bufp->fullIData(oldp+24,(vlSelfRef.top__DOT__u_exe__DOT__mem_rdata),32);
    bufp->fullBit(oldp+25,((1U & (~ (IData)((0U != vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__add_res))))));
    bufp->fullBit(oldp+26,((((vlSelfRef.top__DOT__u_exe__DOT__alu_a 
                              >> 0x1fU) == (vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__adder_inst__DOT__B1 
                                            >> 0x1fU)) 
                            & ((vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__add_res 
                                >> 0x1fU) != (vlSelfRef.top__DOT__u_exe__DOT__alu_a 
                                              >> 0x1fU)))));
    bufp->fullIData(oldp+27,(vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__add_res),32);
    bufp->fullIData(oldp+28,((~ vlSelfRef.top__DOT__u_exe__DOT__alu_a)),32);
    bufp->fullIData(oldp+29,((vlSelfRef.top__DOT__u_exe__DOT__alu_a 
                              & vlSelfRef.top__DOT__u_exe__DOT__alu_b)),32);
    bufp->fullIData(oldp+30,((vlSelfRef.top__DOT__u_exe__DOT__alu_a 
                              | vlSelfRef.top__DOT__u_exe__DOT__alu_b)),32);
    bufp->fullIData(oldp+31,((vlSelfRef.top__DOT__u_exe__DOT__alu_a 
                              ^ vlSelfRef.top__DOT__u_exe__DOT__alu_b)),32);
    bufp->fullIData(oldp+32,((vlSelfRef.top__DOT__u_exe__DOT__alu_a 
                              << (0x1fU & vlSelfRef.top__DOT__u_exe__DOT__alu_b))),32);
    bufp->fullIData(oldp+33,((vlSelfRef.top__DOT__u_exe__DOT__alu_a 
                              >> (0x1fU & vlSelfRef.top__DOT__u_exe__DOT__alu_b))),32);
    bufp->fullIData(oldp+34,(VL_SHIFTRS_III(32,32,5, vlSelfRef.top__DOT__u_exe__DOT__alu_a, 
                                            (0x1fU 
                                             & vlSelfRef.top__DOT__u_exe__DOT__alu_b))),32);
    bufp->fullBit(oldp+35,((1U & ((vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__add_res 
                                   >> 0x1fU) ^ (((vlSelfRef.top__DOT__u_exe__DOT__alu_a 
                                                  >> 0x1fU) 
                                                 == 
                                                 (vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__adder_inst__DOT__B1 
                                                  >> 0x1fU)) 
                                                & ((vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__add_res 
                                                    >> 0x1fU) 
                                                   != 
                                                   (vlSelfRef.top__DOT__u_exe__DOT__alu_a 
                                                    >> 0x1fU)))))));
    bufp->fullBit(oldp+36,(((vlSelfRef.top__DOT__u_exe__DOT__alu_a 
                             >> 0x1fU) ? ((vlSelfRef.top__DOT__u_exe__DOT__alu_b 
                                           >> 0x1fU) 
                                          && (vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__add_res 
                                              >> 0x1fU))
                             : ((vlSelfRef.top__DOT__u_exe__DOT__alu_b 
                                 >> 0x1fU) || (vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__add_res 
                                               >> 0x1fU)))));
    bufp->fullBit(oldp+37,((0U != (IData)(vlSelfRef.top__DOT__u_exe__DOT__alu_op))));
    bufp->fullBit(oldp+38,((1U & (IData)((1ULL & (((QData)((IData)(vlSelfRef.top__DOT__u_exe__DOT__alu_a)) 
                                                   + 
                                                   ((QData)((IData)(vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__adder_inst__DOT__B1)) 
                                                    + (QData)((IData)(
                                                                      (0U 
                                                                       != (IData)(vlSelfRef.top__DOT__u_exe__DOT__alu_op)))))) 
                                                  >> 0x20U))))));
    bufp->fullIData(oldp+39,(vlSelfRef.top__DOT__u_exe__DOT__u_alu__DOT__adder_inst__DOT__B1),32);
    bufp->fullIData(oldp+40,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[0]),32);
    bufp->fullIData(oldp+41,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[1]),32);
    bufp->fullIData(oldp+42,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[2]),32);
    bufp->fullIData(oldp+43,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[3]),32);
    bufp->fullIData(oldp+44,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[4]),32);
    bufp->fullIData(oldp+45,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[5]),32);
    bufp->fullIData(oldp+46,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[6]),32);
    bufp->fullIData(oldp+47,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[7]),32);
    bufp->fullIData(oldp+48,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[8]),32);
    bufp->fullIData(oldp+49,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[9]),32);
    bufp->fullIData(oldp+50,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[10]),32);
    bufp->fullIData(oldp+51,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[11]),32);
    bufp->fullIData(oldp+52,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[12]),32);
    bufp->fullIData(oldp+53,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[13]),32);
    bufp->fullIData(oldp+54,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[14]),32);
    bufp->fullIData(oldp+55,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[15]),32);
    bufp->fullIData(oldp+56,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[16]),32);
    bufp->fullIData(oldp+57,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[17]),32);
    bufp->fullIData(oldp+58,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[18]),32);
    bufp->fullIData(oldp+59,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[19]),32);
    bufp->fullIData(oldp+60,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[20]),32);
    bufp->fullIData(oldp+61,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[21]),32);
    bufp->fullIData(oldp+62,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[22]),32);
    bufp->fullIData(oldp+63,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[23]),32);
    bufp->fullIData(oldp+64,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[24]),32);
    bufp->fullIData(oldp+65,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[25]),32);
    bufp->fullIData(oldp+66,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[26]),32);
    bufp->fullIData(oldp+67,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[27]),32);
    bufp->fullIData(oldp+68,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[28]),32);
    bufp->fullIData(oldp+69,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[29]),32);
    bufp->fullIData(oldp+70,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[30]),32);
    bufp->fullIData(oldp+71,(vlSelfRef.top__DOT__u_gpr__DOT__regfile[31]),32);
    bufp->fullIData(oldp+72,(vlSelfRef.top__DOT__u_gpr__DOT__unnamedblk1__DOT__i),32);
    bufp->fullBit(oldp+73,(vlSelfRef.clk));
    bufp->fullBit(oldp+74,(vlSelfRef.reset));
}
