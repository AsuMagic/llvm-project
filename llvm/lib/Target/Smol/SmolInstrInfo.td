//===-- SmolInstrInfo.td - Target Description for Smol2 Target ------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file describes the Smol2 instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

include "SmolInstrFormats.td"

//===----------------------------------------------------------------------====/
// Opcodes
//===----------------------------------------------------------------------====/

def OPC_L8           : SmolOpcode<"L8",          0b00000000>;
def OPC_L16          : SmolOpcode<"L16",         0b00000001>;
def OPC_L32          : SmolOpcode<"L32",         0b00000010>;
def OPC_CLR          : SmolOpcode<"CLR",         0b00000011>;
def OPC_L8OW         : SmolOpcode<"L8OW",        0b00000100>;
def OPC_L16OW        : SmolOpcode<"L16OW",       0b00000101>;
def OPC_L32OW        : SmolOpcode<"L32OW",       0b00000110>;
def OPC_LR           : SmolOpcode<"LR",          0b00000111>;
def OPC_LS8          : SmolOpcode<"LS8",         0b00001000>;
def OPC_LS16         : SmolOpcode<"LS16",        0b00001001>;
def OPC_LS8OW        : SmolOpcode<"LS8OW",       0b00001010>;
def OPC_LS16OW       : SmolOpcode<"LS16OW",      0b00001011>;
def OPC_L8O          : SmolOpcode<"L8O",         0b00001100>;
def OPC_L16O         : SmolOpcode<"L16O",        0b00010000>;
def OPC_L32O         : SmolOpcode<"L32O",        0b00010100>;
def OPC_LS8O         : SmolOpcode<"LS8O",        0b00011000>;
def OPC_LS16O        : SmolOpcode<"LS16O",       0b00011100>;
def OPC_LSI          : SmolOpcode<"LSI",         0b00100000>;
def OPC_LSIH         : SmolOpcode<"LSIH",        0b00110000>;
def OPC_LSIW         : SmolOpcode<"LSIW",        0b01000000>;
def OPC_LIPREL       : SmolOpcode<"LIPREL",      0b01010000>;
def OPC_S8           : SmolOpcode<"S8",          0b01100000>;
def OPC_S16          : SmolOpcode<"S16",         0b01100001>;
def OPC_S32          : SmolOpcode<"S32",         0b01100010>;
def OPC_PUSH         : SmolOpcode<"PUSH",        0b01100011>;
def OPC_S8OW         : SmolOpcode<"S8OW",        0b01100100>;
def OPC_S16OW        : SmolOpcode<"S16OW",       0b01100101>;
def OPC_S32OW        : SmolOpcode<"S32OW",       0b01100110>;
def OPC_BRK          : SmolOpcode<"BRK",         0b01100111>;
def OPC_S8O          : SmolOpcode<"S8O",         0b01101000>;
def OPC_S16O         : SmolOpcode<"S16O",        0b01101100>;
def OPC_S32O         : SmolOpcode<"S32O",        0b01110000>;
def OPC_TLTU         : SmolOpcode<"TLTU",        0b01110100>;
def OPC_TLTS         : SmolOpcode<"TLTS",        0b01110101>;
def OPC_TGEU         : SmolOpcode<"TGEU",        0b01110110>;
def OPC_TGES         : SmolOpcode<"TGES",        0b01110111>;
def OPC_TE           : SmolOpcode<"TE",          0b01111000>;
def OPC_TNE          : SmolOpcode<"TNE",         0b01111001>;
def OPC_TLTSI        : SmolOpcode<"TLTSI",       0b01111010>;
def OPC_TGESI        : SmolOpcode<"TGESI",       0b01111011>;
def OPC_TEI          : SmolOpcode<"TEI",         0b01111100>;
def OPC_TNEI         : SmolOpcode<"TNEI",        0b01111101>;
def OPC_TBZ          : SmolOpcode<"TBZ",         0b01111110>;
def OPC_PLL32        : SmolOpcode<"PLL32",       0b10000000>;
def OPC_J            : SmolOpcode<"J",           0b10010000>;
def OPC_CJ           : SmolOpcode<"CJ",          0b10010001>;
def OPC_JAL          : SmolOpcode<"JAL",         0b10010010>;
def OPC_JALI         : SmolOpcode<"JALI",        0b10100000>;
def OPC_CJI          : SmolOpcode<"CJI",         0b10110000>;
def OPC_BSEXT8       : SmolOpcode<"BSEXT8",      0b11000000>;
def OPC_BSEXT16      : SmolOpcode<"BSEXT16",     0b11000001>;
def OPC_BZEXT8       : SmolOpcode<"BZEXT8",      0b11000010>;
def OPC_BZEXT16      : SmolOpcode<"BZEXT16",     0b11000011>;
def OPC_INEG         : SmolOpcode<"INEG",        0b11000100>;
def OPC_ISUB         : SmolOpcode<"ISUB",        0b11000101>;
def OPC_IADD         : SmolOpcode<"IADD",        0b11000110>;
def OPC_IADDSI       : SmolOpcode<"IADDSI",      0b11000111>;
def OPC_IADDSIW      : SmolOpcode<"IADDSIW",     0b11001000>;
def OPC_IADDSITNZ    : SmolOpcode<"IADDSITNZ",   0b11001001>;
def OPC_BAND         : SmolOpcode<"BAND",        0b11001010>;
def OPC_BOR          : SmolOpcode<"BOR",         0b11001011>;
def OPC_BXOR         : SmolOpcode<"BXOR",        0b11001100>;
def OPC_BSL          : SmolOpcode<"BSL",         0b11001101>;
def OPC_BSR          : SmolOpcode<"BSR",         0b11001110>;
def OPC_BASR         : SmolOpcode<"BASR",        0b11001111>;
def OPC_BSLI         : SmolOpcode<"BSLI",        0b11010000>;
def OPC_BSRITLSB     : SmolOpcode<"BSRITLSB",    0b11010010>;
def OPC_BASRI        : SmolOpcode<"BASRI",       0b11010100>;

//===----------------------------------------------------------------------====/
// Operand types
//===----------------------------------------------------------------------====/

// let OperandNamespace = "SmolOp" in {

def UImm5AsmOperand : AsmOperandClass {
  let Name = "UImm5";
  let RenderMethod = "addImmOperands";
  let DiagnosticType = "InvalidUImm5";
}

def uimm5 : Operand<i32>, ImmLeaf<i32, [{return isUInt<5>(Imm);}]> {
  let ParserMatchClass = UImm5AsmOperand;
  // let DecoderMathod = "decodeUImmOperand<5>";
  let MCOperandPredicate = [{
    int64_t Imm;
    if (!MCOp.evaluateAsConstantImm(Imm))
      return false;
    return isUInt<5>(Imm);
  }];
}

def simm8 : Operand<i32>, ImmLeaf<i32, [{return isInt<8>(Imm);}]> {
  // let ParserMatchClass = SImmAsmOperand<8>;
  let EncoderMethod = "getImmOpValue";
  let DecoderMethod = "decodeSImmOperand<12>";
  let MCOperandPredicate = [{
    int64_t Imm;
    if (MCOp.evaluateAsConstantImm(Imm))
      return isInt<8>(Imm);
    return MCOp.isBareSymbolRef();
  }];
  let OperandType = "OPERAND_SIMM8";
  // let OperandNamespace = "RISCVOp";
}

// }

//===----------------------------------------------------------------------====/
// Instruction types
//===----------------------------------------------------------------------====/

class LoadSignedShortImm<SmolOpcode opcode, string opcodestr>
    : SmolImmByteLoadInst<opcode, (outs GPR:$a_wb),
                                  (ins GPR:$a_dst, simm8:$b_imm),
                                     opcodestr, "$dst, $b_imm"> {
  let Constraints = "$dst = $a_wb";
}

class ALURawbRb<SmolOpcode opcode, string opcodestr>
    : SmolALUWbConstrainedRegRegInst<opcode, (outs GPR:$a_wb),
                                     (ins GPR:$a_dst, GPR:$b),
                                     opcodestr, "$a_dst, $b"> {
  let Constraints = "$a_dst = $a_wb";
}

class ALURawbRimm5<SmolOpcode opcode, string opcodestr>
    : SmolALURegI5Inst<opcode, (outs GPR:$a_wb),
                       (ins GPR:$a_dst, uimm5:$b_imm),
                       opcodestr, "$a_dst, $b_imm"> {
  let Constraints = "$a_dst = $a_wb";
}

class ALURwbRb<SmolOpcode opcode, string opcodestr>
    : SmolALURegRegInst<opcode, (outs GPR:$dst), (ins GPR:$a),
                        opcodestr, "$dst, $a">;

class Jump<SmolOpcode opcode, string opcodestr>
    : SmolJumpRegInst<opcode, (outs), (ins GPR:$target), opcodestr, "$target">;

// Procedure return
// TODO: where does this go in the file to be clean
def smol_ret : SDNode<"SmolISD::Ret", SDTNone,
                      [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

//===----------------------------------------------------------------------====/
// Lowering patterns
//===----------------------------------------------------------------------====/

class PatGprGpr<SDPatternOperator OpNode, SmolInst Inst>
    : Pat<(OpNode GPR:$rs1, GPR:$rs2), (Inst GPR:$rs1, GPR:$rs2)>;

class PatGprImm5<SDPatternOperator OpNode, SmolInst Inst>
    : Pat<(OpNode GPR:$rs1, uimm5:$imm), (Inst GPR:$rs1, uimm5:$imm)>;

//===----------------------------------------------------------------------====/
// Instructions
//===----------------------------------------------------------------------====/

let mayLoad = 0, mayStore = 0 in {
    def LSI : ALURwbRb<OPC_LSI, "lsi">;

    // FIXME: reversed ops compared to ISA def
    def LR : ALURwbRb<OPC_LR, "lr">;

    let isBranch = 1 in {
        def J : Jump<OPC_J, "j">;
    }

    let isAdd = 1, isCommutable = 1 in {
        def IADD : ALURawbRb<OPC_IADD, "iadd">;
    }

    def ISUB : ALURawbRb<OPC_ISUB, "isub">;

    def BAND : ALURawbRb<OPC_BAND, "band">;
    def BOR : ALURawbRb<OPC_BOR, "bor">;
    def BXOR : ALURawbRb<OPC_BXOR, "bxor">;

    def BSL : ALURawbRb<OPC_BSL, "bsl">;
    def BSLI : ALURawbRimm5<OPC_BSLI, "bsli">;

    def BSR : ALURawbRb<OPC_BSR, "bsr">;
    def BSRITLSB : ALURawbRimm5<OPC_BSRITLSB, "bsri_tlsb">;

    def BASR : ALURawbRb<OPC_BASR, "basr">;
    def BASRI : ALURawbRb<OPC_BASRI, "basri">;

    let isReturn = 1, isBarrier = 1, isTerminator = 1 in
    def PseudoRET : Pseudo<(outs), (ins), [(smol_ret)]>,
                    PseudoInstExpansion<(J RRET)>;
}

def imm32bottom6set : PatLeaf<(i32 imm), [{
  return (N->getZExtValue() & 0x3f) == 0x3f;
}]>;

class shiftop<SDPatternOperator operator>
  : PatFrags<(ops node:$val, node:$count),
             [(operator node:$val, node:$count),
              (operator node:$val, (and node:$count, imm32bottom6set))]>;

def : PatGprGpr<add, IADD>;
def : PatGprGpr<sub, ISUB>;
def : PatGprGpr<and, BAND>;
def : PatGprGpr<or, BOR>;
def : PatGprGpr<xor, BXOR>;
def : PatGprGpr<shiftop<shl>, BSL>;
def : PatGprImm5<shiftop<shl>, BSLI>;
def : PatGprGpr<shiftop<srl>, BSR>;
def : PatGprImm5<shiftop<srl>, BSRITLSB>; // FIXME: T-bit side effect!
def : PatGprGpr<shiftop<sra>, BASR>;
def : PatGprImm5<shiftop<sra>, BASRI>;

def : Pat<(simm8:$imm), (LSI imm:$imm)>;

def : InstAlias<"ret", (J RRET), 4>;
