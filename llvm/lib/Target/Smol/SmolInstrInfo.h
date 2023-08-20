//===-- SmolInstrInfo.h - Smol2 Instruction Information ---------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the Smol2 implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SMOL_SMOLINSTRINFO_H
#define LLVM_LIB_TARGET_SMOL_SMOLINSTRINFO_H

#include "SmolRegisterInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "SmolGenInstrInfo.inc"

namespace llvm {

class SmolSubtarget;

class SmolInstrInfo : public SmolGenInstrInfo {
public:
  explicit SmolInstrInfo(const SmolSubtarget &STI);

protected:
  const SmolSubtarget &Subtarget;
};

}

#endif
