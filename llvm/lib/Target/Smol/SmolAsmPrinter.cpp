//===-- SmolAsmPrinter.cpp - Smol2 LLVM Assembly Printer ------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains a printer that converts from our internal representation
// of machine-dependent LLVM code to GAS-format Smol assembly language.
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/SmolBaseInfo.h"
#include "MCTargetDesc/SmolMCExpr.h"
#include "SmolInstrInfo.h"
#include "SmolTargetMachine.h"
// #include "MCTargetDesc/SmolInstPrinter.h"
#include "TargetInfo/SmolTargetInfo.h"
#include "llvm/CodeGen/AsmPrinter.h"
#include "llvm/CodeGen/MachineInstr.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstBuilder.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

#define DEBUG_TYPE "smol-asm-printer"

namespace llvm {
class SmolAsmPrinter : public AsmPrinter {
public:
  explicit SmolAsmPrinter(TargetMachine &TM,
                          std::unique_ptr<MCStreamer> Streamer)
      : AsmPrinter(TM, std::move(Streamer)) {}

  virtual StringRef getPassName() const override {
    return "Smol2 Assembly Printer";
  }

  void emitInstruction(const MachineInstr *MI) override;

  // This function must be present as it is internally used by the
  // auto-generated function emitPseudoExpansionLowering to expand pseudo
  // instruction
  void EmitToStreamer(MCStreamer &S, const MCInst &Inst);
  // Auto-generated function in SmolGenMCPseudoLowering.inc
  bool emitPseudoExpansionLowering(MCStreamer &OutStreamer,
                                   const MachineInstr *MI);

private:
  void lowerInstruction(const MachineInstr *MI, MCInst &OutMI) const;
  bool lowerOperand(const MachineOperand &MO, MCOperand& MCOp) const;
  MCOperand lowerSymbolOperand(const MachineOperand &MO, MCSymbol *Sym) const;
};
} // namespace llvm

#define GET_INSTRINFO_ENUM
#include "SmolGenInstrInfo.inc"

#define GET_REGINFO_ENUM
#include "SmolGenRegisterInfo.inc"

// Simple pseudo-instructions have their lowering (with expansion to real
// instructions) auto-generated.
#include "SmolGenMCPseudoLowering.inc"
void SmolAsmPrinter::EmitToStreamer(MCStreamer &S, const MCInst &Inst) {
  AsmPrinter::EmitToStreamer(*OutStreamer, Inst);
}

void SmolAsmPrinter::emitInstruction(const MachineInstr *MI) {
  // Do any auto-generated pseudo lowerings.
  if (emitPseudoExpansionLowering(*OutStreamer, MI))
    return;

  // TODO: should be moved to a file dedicated to pseudo expansion?
  // if (MI->getOpcode() == Smol::LoadFullImm) {
  //   auto &Reg = MI->getOperand(0);
  //   MCOperand MCOp;
  //   lowerOperand(MI->getOperand(1), MCOp);

  //   EmitToStreamer(*OutStreamer, MCInstBuilder(Smol::LSIW)
  //                                    .addReg(Reg.getReg())
  //                                    .addImm(MCOp.getImm() & 0x00FFFFFF));
  //   EmitToStreamer(
  //       *OutStreamer,
  //       MCInstBuilder(Smol::LSIH).addReg(Reg.getReg()).addImm(MCOp.getImm() >> 24));
  //   return;
  // }

  MCInst TmpInst;
  lowerInstruction(MI, TmpInst);
  EmitToStreamer(*OutStreamer, TmpInst);
}

void SmolAsmPrinter::lowerInstruction(const MachineInstr *MI,
                                      MCInst &OutMI) const {
  OutMI.setOpcode(MI->getOpcode());

  for (const MachineOperand &MO : MI->operands()) {
    MCOperand MCOp;
    lowerOperand(MO, MCOp);
    if (MCOp.isValid())
      OutMI.addOperand(MCOp);
  }
}

bool SmolAsmPrinter::lowerOperand(const MachineOperand &MO, MCOperand& MCOp) const {
  switch (MO.getType()) {
  case MachineOperand::MO_Register:
    // Ignore all implicit register operands.
    if (MO.isImplicit()) {
      break;
    }
    MCOp = MCOperand::createReg(MO.getReg());
    break;

  case MachineOperand::MO_Immediate:
    MCOp = MCOperand::createImm(MO.getImm());
    break;

  case MachineOperand::MO_MachineBasicBlock:
    MCOp = lowerSymbolOperand(MO, MO.getMBB()->getSymbol());
    break;

  case MachineOperand::MO_GlobalAddress:
    MCOp = lowerSymbolOperand(MO, getSymbol(MO.getGlobal()));
    break;

  case MachineOperand::MO_BlockAddress:
    MCOp = lowerSymbolOperand(MO, GetBlockAddressSymbol(MO.getBlockAddress()));
    break;

  case MachineOperand::MO_ExternalSymbol:
    MCOp = lowerSymbolOperand(MO, GetExternalSymbolSymbol(MO.getSymbolName()));
    break;

  case MachineOperand::MO_ConstantPoolIndex:
    MCOp = lowerSymbolOperand(MO, GetCPISymbol(MO.getIndex()));
    break;

  case MachineOperand::MO_RegisterMask:
    break;

  default:
    report_fatal_error("unknown operand type");
  }

  return true;
}

MCOperand SmolAsmPrinter::lowerSymbolOperand(const MachineOperand &MO,
                                             MCSymbol *Sym) const {
  MCContext &Ctx = OutContext;
  SmolMCExpr::VariantKind Kind;

  switch (MO.getTargetFlags()) {
  default:
    llvm_unreachable("Unknown target flag on GV operand");
  case SmolII::MO_None:
    Kind = SmolMCExpr::VK_Smol_None;
    break;
  case SmolII::MO_LO24:
    Kind = SmolMCExpr::VK_Smol_LO24;
    break;
  case SmolII::MO_HI8:
    Kind = SmolMCExpr::VK_Smol_HI8;
    break;
  }

  const MCExpr *Expr =
      MCSymbolRefExpr::create(Sym, MCSymbolRefExpr::VK_None, Ctx);

  if (!MO.isJTI() && !MO.isMBB() && MO.getOffset()) {
    Expr = MCBinaryExpr::createAdd(
        Expr, MCConstantExpr::create(MO.getOffset(), Ctx), Ctx);
  }

  if (Kind != SmolMCExpr::VK_Smol_None) {
    const SmolMCExpr *MCE = SmolMCExpr::create(Expr, Kind, Ctx);
    Expr = MCE;
  }

  return MCOperand::createExpr(Expr);
}

// Force static initialization.
extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeSmolAsmPrinter() {
  RegisterAsmPrinter<SmolAsmPrinter> X(getTheSmolTarget());
}