//===-- SmolTargetInfo.cpp - Sparc Target Implementation -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "TargetInfo/SmolTargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
using namespace llvm;

Target &llvm::getTheSmolTarget() {
  static Target TheSmolTarget;
  return TheSmolTarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeSmolTargetInfo() {
  RegisterTarget<Triple::smol, /*HasJIT=*/false> X(getTheSmolTarget(),
                                                    "smol", "Smol2", "Smol");
}
