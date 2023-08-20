//===-- SmolSubtarget.cpp - Smol2 Subtarget Information -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the Smol2 specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#include "Smol.h"
#include "SmolSubtarget.h"
#include "SmolMachineFunction.h"
#include "SmolRegisterInfo.h"
#include "SmolTargetMachine.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "smol-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "SmolGenSubtargetInfo.inc"

SmolSubtarget::SmolSubtarget(const Triple &TT, StringRef CPU, StringRef FS,
                               const TargetMachine &TM)
    : SmolGenSubtargetInfo(TT, CPU, /*TuneCPU*/ CPU, FS),
      TSInfo(),
      InstrInfo(initializeSubtargetDependencies(TT, CPU, FS, TM)),
      FrameLowering(*this),
      TLInfo(TM, *this),
      RegInfo(*this) { }


SmolSubtarget &
SmolSubtarget::initializeSubtargetDependencies(const Triple &TT, StringRef CPU,
                                                StringRef FS,
                                                const TargetMachine &TM) {
  std::string CPUName = CPU.str();
  if (CPUName.empty())
    CPUName = "generic";

  // Parse features string.
  ParseSubtargetFeatures(CPUName, /*TuneCPU*/ CPUName, FS);

  return *this;
}