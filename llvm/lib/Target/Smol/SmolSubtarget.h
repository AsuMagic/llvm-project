
//===-- SmolSubtarget.h - Define Subtarget for the Smol2 --------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the Smol2 specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SMOL_SMOLSUBTARGET_H
#define LLVM_LIB_TARGET_SMOL_SMOLSUBTARGET_H

#include "SmolFrameLowering.h"
#include "SmolISelLowering.h"
#include "SmolInstrInfo.h"
#include "llvm/CodeGen/SelectionDAGTargetInfo.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/MC/MCInstrItineraries.h"

#define GET_SUBTARGETINFO_HEADER
#include "SmolGenSubtargetInfo.inc"

namespace llvm {
class SmolSubtarget : public SmolGenSubtargetInfo {
protected:
  SelectionDAGTargetInfo TSInfo;
  SmolInstrInfo InstrInfo;
  SmolFrameLowering FrameLowering;
  SmolTargetLowering TLInfo;
  SmolRegisterInfo RegInfo;

public:
  /// This constructor initializes the data members to match that
  /// of the specified triple.
  SmolSubtarget(const Triple &TT, StringRef CPU, StringRef FS,
                const TargetMachine &TM);

  void ParseSubtargetFeatures(StringRef CPU, StringRef TuneCPU, StringRef FS);

  SmolSubtarget &initializeSubtargetDependencies(const Triple &TT,
                                                  StringRef CPU, StringRef FS,
                                                  const TargetMachine &TM);

  const SelectionDAGTargetInfo *getSelectionDAGInfo() const override {
    return &TSInfo;
  }

  const SmolInstrInfo *getInstrInfo() const override {
    return &InstrInfo;
  }

  const TargetFrameLowering *getFrameLowering() const override {
    return &FrameLowering;
  }

  const SmolRegisterInfo *getRegisterInfo() const override {
    return &RegInfo;
  }

  const SmolTargetLowering *getTargetLowering() const override {
    return &TLInfo;
  }

  /// getMaxInlineSizeThreshold - Returns the maximum memset / memcpy size
  /// that still makes it profitable to inline the call.
  unsigned getMaxInlineSizeThreshold() const {
    return 64;
  }
};
}

#endif // end LLVM_LIB_TARGET_RISCW_RISCWSUBTARGET_H