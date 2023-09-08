//===-- SmolInstPrinter.cpp - Convert Smol2 MCInst to assembly syntax -----===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This class prints an Smol MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#include "SmolInstPrinter.h"

#include "SmolInstrInfo.h"
#include "SmolMCExpr.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

#define DEBUG_TYPE "smol-isel"

#define GET_INSTRINFO_ENUM
#include "SmolGenInstrInfo.inc"

#define GET_REGINFO_ENUM
#include "SmolGenRegisterInfo.inc"

#define PRINT_ALIAS_INSTR
#include "SmolGenAsmWriter.inc"

SmolInstPrinter::SmolInstPrinter(const MCAsmInfo &MAI, const MCInstrInfo &MII,
                                 const MCRegisterInfo &MRI)
    : MCInstPrinter(MAI, MII, MRI) {}

void SmolInstPrinter::printRegName(raw_ostream &OS, MCRegister RegNo) const {
  OS << StringRef(getRegisterName(RegNo)).lower();
}

void SmolInstPrinter::printInst(const MCInst *MI, uint64_t Address,
                                 StringRef Annot, const MCSubtargetInfo &STI,
                                 raw_ostream &O) {
  // Try to print any aliases first.
  if (!printAliasInstr(MI, Address, O)) {
    printInstruction(MI, Address, O);
  }

  printAnnotation(O, Annot);
}

void SmolInstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                    raw_ostream &O) {
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isReg()) {
    printRegName(O, Op.getReg());
    return;
  }

  if (Op.isImm()) {
    O << Op.getImm();
    return;
  }

  assert(Op.isExpr() && "unknown operand kind in printOperand");
  Op.getExpr()->print(O, &MAI, true);
}
