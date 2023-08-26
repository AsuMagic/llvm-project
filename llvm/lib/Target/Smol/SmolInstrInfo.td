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
// Useful util transforms (mostly immediate transforms)
//===----------------------------------------------------------------------====/

def LO24 : SDNodeXForm<imm, [{
  // Transformation function: get the low 24 bits.
  return CurDAG->getTargetConstant(
    N->getZExtValue() & 0x00FFFFFF, SDLoc(N), MVT::i32);
}]>;

def HI8 : SDNodeXForm<imm, [{
  // Transformation function: shift the u8 immediate value down.
  return CurDAG->getTargetConstant(
    (unsigned)N->getZExtValue() >> 24, SDLoc(N), MVT::i32);
}]>;

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

def uimm6 : Operand<i32>, ImmLeaf<i32, [{return isUInt<6>(Imm);}]> {
  // let ParserMatchClass = UImm6AsmOperand;
  // let DecoderMathod = "decodeUImmOperand<6>";
  let MCOperandPredicate = [{
    int64_t Imm;
    if (!MCOp.evaluateAsConstantImm(Imm))
      return false;
    return isUInt<6>(Imm);
  }];
}

def uimm7s1 : Operand<i32>, ImmLeaf<i32,
    [{return isUInt<7>(Imm) && (Imm % 2) == 0;}]> {
  // let ParserMatchClass = UImm6AsmOperand;
  // let DecoderMathod = "decodeUImmOperand<6>";
  let MCOperandPredicate = [{
    int64_t Imm;
    if (!MCOp.evaluateAsConstantImm(Imm))
      return false;
    return isUInt<7>(Imm);
  }];
}

def uimm8s2 : Operand<i32>, ImmLeaf<i32,
    [{return isUInt<8>(Imm) && (Imm % 4) == 0;}]> {
  // let ParserMatchClass = UImm6AsmOperand;
  // let DecoderMathod = "decodeUImmOperand<6>";
  let MCOperandPredicate = [{
    int64_t Imm;
    if (!MCOp.evaluateAsConstantImm(Imm))
      return false;
    return isUInt<8>(Imm);
  }];
}

def uimm32 : Operand<i32>, ImmLeaf<i32, [{return isUInt<32>(Imm);}]> {
  // let ParserMatchClass = UImm32AsmOperand;
  // let DecoderMathod = "decodeUImmOperand<32>";
  let MCOperandPredicate = [{
    int64_t Imm;
    if (!MCOp.evaluateAsConstantImm(Imm))
      return false;
    return isUInt<32>(Imm);
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
  // let OperandType = "OPERAND_SIMM8";
  // let OperandNamespace = "RISCVOp";
}

def simm16 : Operand<i32>, ImmLeaf<i32, [{return isInt<16>(Imm);}]> {
  // let ParserMatchClass = SImmAsmOperand<24>;
  let EncoderMethod = "getImmOpValue";
  let DecoderMethod = "decodeSImmOperand<16>";
  let MCOperandPredicate = [{
    int64_t Imm;
    if (MCOp.evaluateAsConstantImm(Imm))
      return isInt<16>(Imm);
    return MCOp.isBareSymbolRef();
  }];
  // let OperandType = "OPERAND_SIMM24";
  // let OperandNamespace = "RISCVOp";
}

def simm17s1 : Operand<i32>, ImmLeaf<i32,
    [{return isInt<17>(Imm) && (Imm % 2) == 0;}]> {
  // let ParserMatchClass = SImmAsmOperand<24>;
  let EncoderMethod = "getImmOpValue";
  let DecoderMethod = "decodeSImmOperand<17>";
  let MCOperandPredicate = [{
    int64_t Imm;
    if (MCOp.evaluateAsConstantImm(Imm))
      return isInt<17>(Imm);
    return MCOp.isBareSymbolRef();
  }];
  // let OperandType = "OPERAND_SIMM24";
  // let OperandNamespace = "RISCVOp";
}

def simm18s2 : Operand<i32>, ImmLeaf<i32,
    [{return isInt<18>(Imm) && (Imm % 4) == 0;}]> {
  // let ParserMatchClass = SImmAsmOperand<24>;
  let EncoderMethod = "getImmOpValue";
  let DecoderMethod = "decodeSImmOperand<18>";
  let MCOperandPredicate = [{
    int64_t Imm;
    if (MCOp.evaluateAsConstantImm(Imm))
      return isInt<18>(Imm);
    return MCOp.isBareSymbolRef();
  }];
  // let OperandType = "OPERAND_SIMM24";
  // let OperandNamespace = "RISCVOp";
}

def simm24 : Operand<i32>, ImmLeaf<i32, [{return isInt<24>(Imm);}]> {
  // let ParserMatchClass = SImmAsmOperand<24>;
  let EncoderMethod = "getImmOpValue";
  let DecoderMethod = "decodeSImmOperand<24>";
  let MCOperandPredicate = [{
    int64_t Imm;
    if (MCOp.evaluateAsConstantImm(Imm))
      return isInt<24>(Imm);
    return MCOp.isBareSymbolRef();
  }];
  // let OperandType = "OPERAND_SIMM24";
  // let OperandNamespace = "RISCVOp";
}


// }

//===----------------------------------------------------------------------====/
// Instruction types
//===----------------------------------------------------------------------====/

class LoadNoOff<SmolOpcode opcode, string opcodestr>
    : SmolMemLoadInst<opcode, (outs GPR:$dst), (ins GPR:$addr),
                      opcodestr, "$addr, $dst"> {
}

class Load8WideOff<SmolOpcode opcode, string opcodestr>
    : SmolMemLoad8WideOffsetInst<opcode, (outs GPR:$dst),
                                 (ins GPR:$addr, simm16:$offset),
                                 opcodestr, "$addr, $dst, $offset">;

class Load16WideOff<SmolOpcode opcode, string opcodestr>
    : SmolMemLoad16WideOffsetInst<opcode, (outs GPR:$dst),
                                 (ins GPR:$addr, simm17s1:$offset),
                                 opcodestr, "$addr, $dst, $offset">;

class Load32WideOff<SmolOpcode opcode, string opcodestr>
    : SmolMemLoad32WideOffsetInst<opcode, (outs GPR:$dst),
                                 (ins GPR:$addr, simm18s2:$offset),
                                 opcodestr, "$addr, $dst, $offset">;

class Load8ShortOff<SmolOpcode opcode, string opcodestr>
    : SmolMemLoad8ShortOffsetInst<opcode, (outs Rl2:$dst),
                                  (ins Rh2:$addr, uimm6:$offset),
                                  opcodestr, "$addr, $dst, $offset">;

class Load16ShortOff<SmolOpcode opcode, string opcodestr>
    : SmolMemLoad16ShortOffsetInst<opcode, (outs Rl2:$dst),
                                   (ins Rh2:$addr, uimm7s1:$offset),
                                   opcodestr, "$addr, $dst, $offset">;

class Load32ShortOff<SmolOpcode opcode, string opcodestr>
    : SmolMemLoad32ShortOffsetInst<opcode, (outs Rl2:$dst),
                                   (ins Rh2:$addr, uimm8s2:$offset),
                                   opcodestr, "$addr, $dst, $offset">;

class LoadSignedShortImm<SmolOpcode opcode, string opcodestr>
    : SmolImmByteLoadInst<opcode, (outs GPR:$dst), (ins simm8:$imm),
                          opcodestr, "$dst, $imm">;

class LoadSignedWideImm<SmolOpcode opcode, string opcodestr>
    : SmolImmI24LoadInst<opcode, (outs GPR:$dst), (ins simm24:$imm),
                         opcodestr, "$dst, $imm">;

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
// Lowering pattern classes
//===----------------------------------------------------------------------====/

// Addressing modes
class AddrRegImm<string name> : ComplexPattern<iPTR, 2, name>;

def AddrRegImmU6 : AddrRegImm<"SelectAddrRegImmU6">;
def AddrRegImmU7 : AddrRegImm<"SelectAddrRegImmU7">;
def AddrRegImmU8 : AddrRegImm<"SelectAddrRegImmU8">;

def AddrRegImmS16 : AddrRegImm<"SelectAddrRegImmS16">;
def AddrRegImmS17 : AddrRegImm<"SelectAddrRegImmS17">;
def AddrRegImmS18 : AddrRegImm<"SelectAddrRegImmS18">;

class PatLoadNoOff<PatFrag LoadOp, SmolInst Inst>
    : Pat<(i32 (LoadOp (i32 GPR:$addr))),
          (Inst GPR:$addr)>;

class PatLoadShortOff<PatFrag LoadOp, SmolInst Inst, Operand imm, AddrRegImm addressing>
    : Pat<(i32 (LoadOp (addressing (i32 Rh2:$rs1), imm:$imm))),
          (Inst Rh2:$rs1, imm:$imm)>;

class PatLoadWideOff<PatFrag LoadOp, SmolInst Inst, Operand imm, AddrRegImm addressing>
    : Pat<(i32 (LoadOp (addressing (i32 GPR:$rs1), imm:$imm))),
          (Inst GPR:$rs1, imm:$imm)>;

class PatGprGpr<SDPatternOperator OpNode, SmolInst Inst>
    : Pat<(OpNode GPR:$rs1, GPR:$rs2), (Inst GPR:$rs1, GPR:$rs2)>;

class PatGprImm5<SDPatternOperator OpNode, SmolInst Inst>
    : Pat<(OpNode GPR:$rs1, uimm5:$imm), (Inst GPR:$rs1, uimm5:$imm)>;

def imm32bottom6set : PatLeaf<(i32 imm), [{
  return (N->getZExtValue() & 0x3f) == 0x3f;
}]>;

def imm32bottom24clear : PatLeaf<(i32 imm), [{
  return (N->getZExtValue() & 0xffffff) == 0;
}]>;

class shiftop<SDPatternOperator operator>
  : PatFrags<(ops node:$val, node:$count),
             [(operator node:$val, node:$count),
              (operator node:$val, (and node:$count, imm32bottom6set))]>;

//===----------------------------------------------------------------------====/
// Instructions
//===----------------------------------------------------------------------====/

let mayLoad = 1, mayStore = 0 in {
  // Short offset:
  def L8 : LoadNoOff<OPC_L8, "l8">;
  def L16 : LoadNoOff<OPC_L16, "l16">;
  def LS8 : LoadNoOff<OPC_LS8, "ls8">;
  def LS16 : LoadNoOff<OPC_LS16, "ls16">;
  def L32 : LoadNoOff<OPC_L32, "l32">;

  // Short offset:
  def L8O : Load8ShortOff<OPC_L8O, "l8o">;
  def L16O : Load16ShortOff<OPC_L16O, "l16o">;
  def L32O : Load32ShortOff<OPC_L32O, "l32o">;
  def LS8O : Load8ShortOff<OPC_LS8O, "ls8o">;
  def LS16O : Load16ShortOff<OPC_LS16O, "ls16o">;
  
  // Wide offset:
  def L8OW : Load8WideOff<OPC_L8OW, "l8ow">;
  def L16OW : Load16WideOff<OPC_L16OW, "l16ow">;
  def L32OW : Load32WideOff<OPC_L32OW, "l32ow">;
  def LS8OW : Load8WideOff<OPC_LS8OW, "ls8ow">;
  def LS16OW : Load16WideOff<OPC_LS16OW, "ls16ow">;
}

let mayLoad = 0, mayStore = 0 in {
  def LSI : LoadSignedShortImm<OPC_LSI, "lsi">;
  def LSIH : LoadSignedShortImm<OPC_LSIH, "lsih">;
  def LSIW : LoadSignedWideImm<OPC_LSIW, "lsiw">;

  def LoadFullImm: Pseudo<(outs GPR:$dst), (ins uimm32:$imm),
                          [(set GPR:$dst, imm:$imm)]>;

  // FIXME: reversed ops compared to ISA def
  def LR : ALURwbRb<OPC_LR, "lr">;

  let isBranch = 1 in {
      def J : Jump<OPC_J, "j">;
  }

  let isAdd = 1, isCommutable = 1 in {
      def IADD : ALURawbRb<OPC_IADD, "iadd">;
  }

  def ISUB : ALURawbRb<OPC_ISUB, "isub">;

  let isCommutable = 1 in {
    def BAND : ALURawbRb<OPC_BAND, "band">;
    def BOR : ALURawbRb<OPC_BOR, "bor">;
    def BXOR : ALURawbRb<OPC_BXOR, "bxor">;
  }

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

//===----------------------------------------------------------------------====/
// Instruction lowering patterns
//===----------------------------------------------------------------------====/

// Memory loads
def : PatLoadNoOff<zextloadi8, L8>;
def : PatLoadNoOff<zextloadi16, L16>;
def : PatLoadNoOff<load, L32>;
def : PatLoadNoOff<sextloadi8, LS8>;
def : PatLoadNoOff<sextloadi16, LS16>;

def : PatLoadShortOff<zextloadi8, L8O, uimm6, AddrRegImmU6>;
def : PatLoadShortOff<zextloadi16, L16O, uimm7s1, AddrRegImmU7>;
def : PatLoadShortOff<load, L32O, uimm8s2, AddrRegImmU8>;
def : PatLoadShortOff<sextloadi8, LS8O, uimm6, AddrRegImmU6>;
def : PatLoadShortOff<sextloadi16, LS16O, uimm7s1, AddrRegImmU7>;

def : PatLoadWideOff<zextloadi8, L8OW, simm16, AddrRegImmS16>;
def : PatLoadWideOff<zextloadi16, L16OW, simm17s1, AddrRegImmS17>;
def : PatLoadShortOff<load, L32OW, simm18s2, AddrRegImmS18>;
def : PatLoadWideOff<sextloadi8, LS8OW, simm16, AddrRegImmS16>;
def : PatLoadWideOff<sextloadi16, LS16OW, simm17s1, AddrRegImmS17>;

// ALU
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

// Immediates
def : Pat<(simm8:$imm), (LSI imm:$imm)>;
def : Pat<(simm24:$imm), (LSIW imm:$imm)>;
def : Pat<(uimm32:$imm), (LoadFullImm imm:$imm)>;

// TODO: this is 3 cycles total; prefer literal pools and iprel when possible
// FIXME: no workies; how to specify multiple insns with 1 temp reg?
// def : Pat<(i32 imm:$imm), >;

// optimization: load of 0x??000000 is a lsih
// FIXME: not actually the case lol -- should the ISA clear 24 lower or not?
// def : Pat<(i32 imm32bottom24clear:$imm), (LSIH (HI8 imm:$imm))>;

// Branching
def : InstAlias<"ret", (J RRET), 4>;
