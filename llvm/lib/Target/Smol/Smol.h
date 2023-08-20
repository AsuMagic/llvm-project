//=== Smol.h - Top-level interface for Smol2 representation -----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in
// the LLVM RISCW backend.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SMOL_SMOL_H
#define LLVM_LIB_TARGET_SMOL_SMOL_H

// #include "MCTargetDesc/SmolTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
  class FunctionPass;

  // Declare functions to create passes here!

} // namespace llvm

#endif // end LLVM_LIB_TARGET_SMOL_SMOL_H