//===-- SmolFrameLowering.h - Define frame lowering for Smol2 ---*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the SmolTargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SMOL_SMOLFRAMELOWERING_H
#define LLVM_LIB_TARGET_SMOL_SMOLFRAMELOWERING_H

#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {

class SmolSubtarget;

class SmolFrameLowering : public TargetFrameLowering {
public:
  explicit SmolFrameLowering(const SmolSubtarget &STI)
    : TargetFrameLowering(TargetFrameLowering::StackGrowsDown,
                          /*StackAlignment*/Align(4),
                          /*LocalAreaOffset*/0,
                          /*TransAl*/Align(4)),
      Subtarget(STI)
    {}

  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;

  bool hasReservedCallFrame(const MachineFunction &MF) const override;
  MachineBasicBlock::iterator
  eliminateCallFramePseudoInstr(MachineFunction &MF, MachineBasicBlock &MBB,
                                MachineBasicBlock::iterator I) const override;

  void determineCalleeSaves(MachineFunction &MF, BitVector &SavedRegs,
                            RegScavenger *RS) const override;

  bool hasFP(const MachineFunction &MF) const override;

protected:
  const SmolSubtarget& Subtarget;
};
} // llvm namespace

#endif // end LLVM_LIB_TARGET_SMOL_SMOLFRAMELOWERING_H
