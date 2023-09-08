//===-- SmolMCExpr.cpp - Smol2 specific MC expression classes ------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the implementation of the assembly expression modifiers
// accepted by the Smol2 architecture.
//
//===----------------------------------------------------------------------===//

#include "SmolMCExpr.h"

#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCStreamer.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "smolexpr"

const SmolMCExpr *SmolMCExpr::create(const MCExpr *Expr, VariantKind Kind,
                                    MCContext &Ctx) {
  return new (Ctx) SmolMCExpr(Expr, Kind);
}

void SmolMCExpr::printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const {
  VariantKind Kind = getVariantKind();
  bool HasVariant = (Kind != VK_Smol_None);
  
  if (HasVariant) {
    OS << '%' << getVariantKindName(getVariantKind()) << '(';
  }

  Expr->print(OS, MAI);

  if (HasVariant) {
    OS << ')';
  }
}

bool SmolMCExpr::evaluateAsRelocatableImpl(MCValue &Res,
                                           const MCAsmLayout *Layout,
                                           const MCFixup *Fixup) const {
  // TODO: this is copy pasted from RISC-V; figure out what this means

  // Explicitly drop the layout and assembler to prevent any symbolic folding in
  // the expression handling.  This is required to preserve symbolic difference
  // expressions to emit the paired relocations.
  if (!getSubExpr()->evaluateAsRelocatable(Res, nullptr, nullptr))
    return false;

  Res =
      MCValue::get(Res.getSymA(), Res.getSymB(), Res.getConstant(), getKind());
  // Custom fixup types are not valid with symbol difference expressions.
  return Res.getSymB() ? getKind() == VK_Smol_None : true;
}

void SmolMCExpr::visitUsedExpr(MCStreamer &Streamer) const {
  Streamer.visitUsedExpr(*getSubExpr());
}

static void fixELFSymbolsInTLSFixupsImpl(const MCExpr *Expr, MCAssembler &Asm) {
  switch (Expr->getKind()) {
  default:
  case MCExpr::Target:
    llvm_unreachable("Can't handle nested target expression");
    break;
  case MCExpr::Constant:
    break;

  case MCExpr::Binary: {
    const MCBinaryExpr *BE = cast<MCBinaryExpr>(Expr);
    fixELFSymbolsInTLSFixupsImpl(BE->getLHS(), Asm);
    fixELFSymbolsInTLSFixupsImpl(BE->getRHS(), Asm);
    break;
  }

  // case MCExpr::SymbolRef: {
  //   // We're known to be under a TLS fixup, so any symbol should be
  //   // modified. There should be only one.
  //   const MCSymbolRefExpr &SymRef = *cast<MCSymbolRefExpr>(Expr);
  //   cast<MCSymbolELF>(SymRef.getSymbol()).setType(ELF::STT_TLS);
  //   break;
  // }

  case MCExpr::Unary:
    fixELFSymbolsInTLSFixupsImpl(cast<MCUnaryExpr>(Expr)->getSubExpr(), Asm);
    break;
  }
}

void SmolMCExpr::fixELFSymbolsInTLSFixups(MCAssembler &Asm) const {
  fixELFSymbolsInTLSFixupsImpl(getSubExpr(), Asm);
}

StringRef SmolMCExpr::getVariantKindName(VariantKind Kind) {
  switch (Kind) {
  case VK_Smol_Invalid:
  case VK_Smol_None:
    llvm_unreachable("Invalid ELF symbol kind");
  case VK_Smol_LO24:
    return "lo24";
  case VK_Smol_HI8:
    return "hi8";
  }
  llvm_unreachable("Invalid ELF symbol kind");
}