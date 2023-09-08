//===-- SmolMCExpr.h - Smol2 specific MC expression classes----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file describes Smol2 specific MCExprs, used for modifiers like
// "%hi" or "%lo" etc.,
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SMOL_MCTARGETDESC_RISCVMCEXPR_H
#define LLVM_LIB_TARGET_SMOL_MCTARGETDESC_RISCVMCEXPR_H

#include "llvm/MC/MCExpr.h"

namespace llvm {

class SmolMCExpr : public MCTargetExpr {
public:
  enum VariantKind {
    VK_Smol_None,
    VK_Smol_LO24,
    VK_Smol_HI8,
    VK_Smol_Invalid
  };

private:
  const MCExpr *Expr;
  const VariantKind Kind;

  int64_t evaluateAsInt64(int64_t Value) const;

  explicit SmolMCExpr(const MCExpr *Expr, VariantKind Kind):
      Expr(Expr), Kind(Kind) {}

public:
  static const SmolMCExpr *create(const MCExpr *Expr, VariantKind Kind,
                                  MCContext &Ctx);

  VariantKind getVariantKind() const { return Kind; }

  const MCExpr *getSubExpr() const { return Expr; }

  void printImpl(raw_ostream &OS, const MCAsmInfo *MAI) const override;
  bool evaluateAsRelocatableImpl(MCValue &Res, const MCAsmLayout *Layout,
                                 const MCFixup *Fixup) const override;
  void visitUsedExpr(MCStreamer &Streamer) const override;
  MCFragment *findAssociatedFragment() const override {
    return getSubExpr()->findAssociatedFragment();
  }

  void fixELFSymbolsInTLSFixups(MCAssembler &Asm) const override;

  static StringRef getVariantKindName(VariantKind Kind);
};

} // namespace llvm

#endif