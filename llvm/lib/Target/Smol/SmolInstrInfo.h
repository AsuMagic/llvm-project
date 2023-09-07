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

  void copyPhysReg(MachineBasicBlock &MBB, MachineBasicBlock::iterator MI,
                   const DebugLoc &DL, MCRegister DestReg, MCRegister SrcReg,
                   bool KillSrc) const override;

  ArrayRef<std::pair<unsigned, const char *>>
  getSerializableDirectMachineOperandTargetFlags() const override;

  // bool analyzeBranch(MachineBasicBlock &MBB, MachineBasicBlock *&TBB,
  //                    MachineBasicBlock *&FBB,
  //                    SmallVectorImpl<MachineOperand> &Cond,
  //                    bool AllowModify) const override;

protected:
  const SmolSubtarget &Subtarget;
};

} // namespace llvm

#endif
