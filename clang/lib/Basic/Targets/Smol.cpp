//===--- RISCW.cpp - Implement RISCW target feature support ---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements RISCWTargetInfo objects.
//
//===----------------------------------------------------------------------===//

#include "Smol.h"
#include "clang/Basic/MacroBuilder.h"
#include "llvm/ADT/StringSwitch.h"

using namespace clang;
using namespace clang::targets;

const char *const SmolTargetInfo::GCCRegNames[] = {
  // Integer registers
  "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11",
  "r12",

  // Misc GPRs
  "rret", "rpl", "rps"
};

const TargetInfo::GCCRegAlias GCCRegAliases[] = {
    // we don't have any alias but i'm too lazy to figure out why it's unhappy
    // otherwise, so let's pretend we have a very real gcc that has r13=rret
    {{"rret"}, "r13"}
};

ArrayRef<const char *> SmolTargetInfo::getGCCRegNames() const {
  return llvm::ArrayRef(GCCRegNames);
}

ArrayRef<TargetInfo::GCCRegAlias> SmolTargetInfo::getGCCRegAliases() const {
  return llvm::ArrayRef(GCCRegAliases);
}

void SmolTargetInfo::getTargetDefines(const LangOptions &Opts,
                                       MacroBuilder &Builder) const {
  Builder.defineMacro("__SMOL__");
}