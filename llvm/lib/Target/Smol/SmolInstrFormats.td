//===-- SmolInstrFormats.td - Smol2 Instruction Formats ----*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

class InstFormat<bits<4> val> {
  bits<4> Value = val;
}

def InstFormatPseudo  : InstFormat<0>;
def InstFormatR4      : InstFormat<1>;
def InstFormatR4R4    : InstFormat<2>;
def InstFormatR4I4    : InstFormat<3>;
def InstFormatR4I5    : InstFormat<4>;
def InstFormatR4I8    : InstFormat<5>;
def InstFormatR4I8E16 : InstFormat<6>;
def InstFormatR4R4E16 : InstFormat<7>;
def InstFormatRh2R2I6 : InstFormat<8>;
def InstFormatI12     : InstFormat<9>;
def InstFormatI12E16  : InstFormat<10>;
def InstFormatNone    : InstFormat<11>;

class SmolOpcode<string name, bits<8> val> {
  string Name = name;
  bits<8> Value = val;
}

class SmolInst<dag outs, dag ins, string opcodestr, string argstr,
               list<dag> pattern, InstFormat format> : Instruction {
  let Namespace = "Smol";

  bits<8> Opcode = 0;

  dag OutOperandList = outs;
  dag InOperandList = ins;
  let AsmString = opcodestr # "\t" # argstr;
  let Pattern = pattern;

  let TSFlags{3-0} = format.Value;
}

class SmolInst16<dag outs, dag ins, string opcodestr, string argstr,
                 list<dag> pattern, InstFormat format>
    : SmolInst<outs, ins, opcodestr, argstr, pattern, format> {
  field bits<16> Inst;

  let Size = 2;
}

class SmolInst32<dag outs, dag ins, string opcodestr, string argstr,
                 list<dag> pattern, InstFormat format>
    : SmolInst<outs, ins, opcodestr, argstr, pattern, format> {
  field bits<32> Inst;

  let Size = 4;
}

// TODO: the following are closer to be sort of instruction categories that
// would be long to InstrInfo than to InstrFormats.
// Ideally, we would only have one class per encoding format.

// TODO: Additionally, R4 could be merged into R4R4 as InstrInfo can just
// discard the second operand and zero it out.

class SmolRegLoadInst<SmolOpcode opcode, dag outs, dag ins, string opcodestr,
                      string argstr>
    : SmolInst16<outs, ins, opcodestr, argstr, [], InstFormatR4R4> {
  bits<4> src;
  bits<4> dst;

  let Inst{15-8} = opcode.Value;
  let Inst{7-4} = dst;
  let Inst{3-0} = src;
  let Opcode = opcode.Value;
}

class SmolMemLoadInst<SmolOpcode opcode, dag outs, dag ins, string opcodestr,
                      string argstr>
    : SmolInst16<outs, ins, opcodestr, argstr, [], InstFormatR4R4> {
  bits<4> addr;
  bits<4> dst;

  let Inst{15-8} = opcode.Value;
  let Inst{7-4} = dst;
  let Inst{3-0} = addr;
  let Opcode = opcode.Value;
}

class SmolMemLoadWideOffsetInst<SmolOpcode opcode, dag outs, dag ins,
                                string opcodestr, string argstr>
    : SmolInst32<outs, ins, opcodestr, argstr, [], InstFormatR4R4E16> {
  bits<4> addr;
  bits<16> offset;
  bits<4> dst;

  let Inst{31-16} = offset;
  let Inst{15-8} = opcode.Value;
  let Inst{7-4} = dst;
  let Inst{3-0} = addr;
  let Opcode = opcode.Value;
}

class SmolMemLoadShortOffsetInst<SmolOpcode opcode, dag outs, dag ins,
                                 string opcodestr, string argstr>
    : SmolInst32<outs, ins, opcodestr, argstr, [], InstFormatR4R4E16> {
  bits<2> addr;
  bits<6> offset;
  bits<2> dst;

  let Inst{15-10} = opcode.Value{7-2};
  let Inst{9-4} = offset;
  let Inst{3-2} = dst;
  let Inst{1-0} = addr;
  let Opcode = opcode.Value;
}

class SmolImmByteLoadInst<SmolOpcode opcode, dag outs, dag ins,
                          string opcodestr, string argstr>
    : SmolInst16<outs, ins, opcodestr, argstr, [], InstFormatR4I8> {
  bits<8> imm;
  bits<4> dst;

  let Inst{15-12} = opcode.Value{7-4};
  let Inst{11-4} = imm;
  let Inst{3-0} = dst;
  let Opcode = opcode.Value;
}

class SmolImmI24LoadInst<SmolOpcode opcode, dag outs, dag ins,
                         string opcodestr, string argstr>
    : SmolInst32<outs, ins, opcodestr, argstr, [], InstFormatR4I8E16> {
  bits<24> imm;
  bits<4> dst;

  let Inst{31-16} = imm{23-8};
  let Inst{15-12} = opcode.Value{7-4};
  let Inst{11-4} = imm{7-0};
  let Inst{3-0} = dst;
  let Opcode = opcode.Value;
}

class SmolMemStoreInst<SmolOpcode opcode, dag outs, dag ins, string opcodestr,
                       string argstr>
    : SmolInst16<outs, ins, opcodestr, argstr, [], InstFormatR4R4> {
  bits<4> addr;
  bits<4> src;

  let Inst{15-8} = opcode.Value;
  let Inst{7-4} = src;
  let Inst{3-0} = addr;
  let Opcode = opcode.Value;
}

class SmolMemStoreShortOffsetInst<SmolOpcode opcode, dag outs, dag ins,
                                 string opcodestr, string argstr>
    : SmolInst32<outs, ins, opcodestr, argstr, [], InstFormatR4R4E16> {
  bits<2> addr;
  bits<6> offset;
  bits<2> src;

  let Inst{15-10} = opcode.Value{7-2};
  let Inst{9-4} = offset;
  let Inst{3-2} = src;
  let Inst{1-0} = addr;
  let Opcode = opcode.Value;
}

class SmolMemStoreWideOffsetInst<SmolOpcode opcode, dag outs, dag ins,
                                 string opcodestr, string argstr>
    : SmolInst32<outs, ins, opcodestr, argstr, [], InstFormatR4R4E16> {
  bits<4> addr;
  bits<16> offset;
  bits<4> src;

  let Inst{31-16} = offset;
  let Inst{15-8} = opcode.Value;
  let Inst{7-4} = src;
  let Inst{3-0} = addr;
  let Opcode = opcode.Value;
}

class SmolStackPushInst<SmolOpcode opcode, dag outs, dag ins, string opcodestr,
                    string argstr>
    : SmolInst16<outs, ins, opcodestr, argstr, [], InstFormatR4> {
  bits<4> src;

  let Inst{15-8} = opcode.Value;
  let Inst{3-0} = src;
  let Opcode = opcode.Value;
}

class SmolNoArgInst<SmolOpcode opcode, dag outs, dag ins, string opcodestr,
                    string argstr>
    : SmolInst16<outs, ins, opcodestr, argstr, [], InstFormatNone> {
  let Inst{15-8} = opcode.Value;
  let Opcode = opcode.Value;
}

class SmolTestRegRegInst<SmolOpcode opcode, dag outs, dag ins, string opcodestr,
                      string argstr>
    : SmolInst16<outs, ins, opcodestr, argstr, [], InstFormatR4R4> {
  bits<4> a;
  bits<4> b;

  let Inst{15-8} = opcode.Value;
  let Inst{7-4} = b;
  let Inst{3-0} = a;
  let Opcode = opcode.Value;
}

class SmolTestRegI4Inst<SmolOpcode opcode, dag outs, dag ins, string opcodestr,
                      string argstr>
    : SmolInst16<outs, ins, opcodestr, argstr, [], InstFormatR4I4> {
  bits<4> a;
  bits<4> b_imm;

  let Inst{15-8} = opcode.Value;
  let Inst{7-4} = b_imm;
  let Inst{3-0} = a;
  let Opcode = opcode.Value;
}

class SmolTestRegInst<SmolOpcode opcode, dag outs, dag ins, string opcodestr,
                      string argstr>
    : SmolInst16<outs, ins, opcodestr, argstr, [], InstFormatR4> {
  bits<4> x;

  let Inst{15-8} = opcode.Value;
  let Inst{3-0} = x;
  let Opcode = opcode.Value;
}

class SmolPoolLoadInst<SmolOpcode opcode, dag outs, dag ins, string opcodestr,
                       string argstr>
    : SmolInst16<outs, ins, opcodestr, argstr, [], InstFormatR4I8> {
  bits<4> dst;
  bits<8> offset_imm;

  let Inst{15-12} = opcode.Value{7-4};
  let Inst{11-4} = offset_imm;
  let Inst{3-0} = dst;
  let Opcode = opcode.Value;
}

class SmolJumpRegInst<SmolOpcode opcode, dag outs, dag ins, string opcodestr,
                      string argstr>
    : SmolInst16<outs, ins, opcodestr, argstr, [], InstFormatR4> {
  bits<4> target;

  let Inst{15-8} = opcode.Value;
  let Inst{3-0} = target;
  let Opcode = opcode.Value;
}

class SmolJumpLinkRegInst<SmolOpcode opcode, dag outs, dag ins,
                          string opcodestr, string argstr>
    : SmolInst16<outs, ins, opcodestr, argstr, [], InstFormatR4R4> {
  bits<4> target;
  bits<4> dst;

  let Inst{15-8} = opcode.Value;
  let Inst{7-4} = dst;
  let Inst{3-0} = target;
  let Opcode = opcode.Value;
}

class SmolJumpLinkI28Inst<SmolOpcode opcode, dag outs, dag ins,
                          string opcodestr, string argstr>
    : SmolInst32<outs, ins, opcodestr, argstr, [], InstFormatI12E16> {
  bits<28> relative_target;

  let Inst{31-16} = relative_target{27-12};
  let Inst{15-12} = opcode.Value{7-4};
  let Inst{11-0} = relative_target{11-0};
  let Opcode = opcode.Value;
}

class SmolJumpCondI12Inst<SmolOpcode opcode, dag outs, dag ins, string opcodestr,
                      string argstr>
    : SmolInst16<outs, ins, opcodestr, argstr, [], InstFormatI12> {
  bits<12> relative_target;
  bits<0> cond;

  let Inst{15-12} = opcode.Value{7-4};
  let Inst{11-0} = relative_target{11-0};
  let Opcode = opcode.Value;
}

class SmolALUWbConstrainedRegRegInst<SmolOpcode opcode, dag outs, dag ins, string opcodestr,
                        string argstr>
    : SmolInst16<outs, ins, opcodestr, argstr, [], InstFormatR4R4> {
  bits<4> a_dst;
  bits<4> b;
  bits<0> cond;

  let Inst{15-8} = opcode.Value;
  let Inst{7-4} = b;
  let Inst{3-0} = a_dst;
  let Opcode = opcode.Value;
}

class SmolALURegRegInst<SmolOpcode opcode, dag outs, dag ins, string opcodestr,
                        string argstr>
    : SmolInst16<outs, ins, opcodestr, argstr, [], InstFormatR4R4> {
  bits<4> dst;
  bits<4> a;

  let Inst{15-8} = opcode.Value;
  let Inst{7-4} = a;
  let Inst{3-0} = dst;
  let Opcode = opcode.Value;
}

class SmolALURegRegCondInst<SmolOpcode opcode, dag outs, dag ins, string opcodestr,
                        string argstr>
    : SmolInst16<outs, ins, opcodestr, argstr, [], InstFormatR4R4> {
  bits<4> dst;
  bits<4> a;
  bits<0> cond;

  let Inst{15-8} = opcode.Value;
  let Inst{7-4} = a;
  let Inst{3-0} = dst;
  let Opcode = opcode.Value;
}

class SmolALURegI4Inst<SmolOpcode opcode, dag outs, dag ins, string opcodestr,
                        string argstr>
    : SmolInst16<outs, ins, opcodestr, argstr, [], InstFormatR4I4> {
  bits<4> a_dst;
  bits<4> b_imm;

  let Inst{15-8} = opcode.Value;
  let Inst{7-4} = b_imm;
  let Inst{3-0} = a_dst;
  let Opcode = opcode.Value;
}

class SmolALURegI4TwbInst<SmolOpcode opcode, dag outs, dag ins, string opcodestr,
                        string argstr>
    : SmolInst16<outs, ins, opcodestr, argstr, [], InstFormatR4I4> {
  bits<4> a_dst;
  bits<4> b_imm;
  bits<0> cond_wb;

  let Inst{15-8} = opcode.Value;
  let Inst{7-4} = b_imm;
  let Inst{3-0} = a_dst;
  let Opcode = opcode.Value;
}

class SmolALURegI5Inst<SmolOpcode opcode, dag outs, dag ins, string opcodestr,
                        string argstr>
    : SmolInst16<outs, ins, opcodestr, argstr, [], InstFormatR4I5> {
  bits<4> a_dst;
  bits<5> b_imm;

  let Inst{15-9} = opcode.Value{7-1};
  let Inst{8-4} = b_imm;
  let Inst{3-0} = a_dst;
  let Opcode = opcode.Value;
}

class SmolALURegI8Inst<SmolOpcode opcode, dag outs, dag ins, string opcodestr,
                        string argstr>
    : SmolInst16<outs, ins, opcodestr, argstr, [], InstFormatR4I8> {
  bits<4> a_dst;
  bits<8> b_imm;

  let Inst{15-12} = opcode.Value{7-4};
  let Inst{11-4} = b_imm;
  let Inst{3-0} = a_dst;
  let Opcode = opcode.Value;
}

class SmolALUWideAddInst<SmolOpcode opcode, dag outs, dag ins, string opcodestr,
                         string argstr>
    : SmolInst32<outs, ins, opcodestr, argstr, [], InstFormatR4R4E16> {
  bits<4> dst;
  bits<4> a;
  bits<16> b_imm;

  let Inst{31-16} = b_imm;
  let Inst{15-8} = opcode.Value;
  let Inst{7-4} = a;
  let Inst{3-0} = dst;
  let Opcode = opcode.Value;
}

// Pseudo instructions
class Pseudo<dag outs, dag ins, list<dag> pattern, string opcodestr = "",
             string argstr = "">
    : SmolInst<outs, ins, opcodestr, argstr, pattern, InstFormatPseudo>,
      Sched<[]> {
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}