//===-- SmolMCTargetDesc.cpp - Smol2 Target Descriptions ------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides Smol2 specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "SmolMCTargetDesc.h"
#include "SmolInstPrinter.h"
#include "SmolMCAsmInfo.h"
#include "TargetInfo/SmolTargetInfo.h"
#include "llvm/MC/MCELFStreamer.h"
#include "llvm/MC/MCInstrAnalysis.h"
#include "llvm/MC/MCInstPrinter.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "SmolGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "SmolGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "SmolGenRegisterInfo.inc"

static MCInstrInfo *createSmolMCInstrInfo() {
  MCInstrInfo *X = new MCInstrInfo();
  InitSmolMCInstrInfo(X);
  return X;
}

static MCRegisterInfo *createSmolMCRegisterInfo(const Triple &TT) {
  MCRegisterInfo *X = new MCRegisterInfo();
  return X;
}

static MCSubtargetInfo *
createSmolMCSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
  // std::string CPUName = CPU;
  // if (CPUName.empty())
  //   CPUName = "generic";
  std::string CPUName = "generic";
  //   return createSmolMCSubtargetInfoImpl(TT, CPUName, FS);
  return nullptr;
}

static MCInstPrinter *createSmolMCInstPrinter(const Triple &T,
                                              unsigned SyntaxVariant,
                                              const MCAsmInfo &MAI,
                                              const MCInstrInfo &MII,
                                              const MCRegisterInfo &MRI) {
  return new SmolInstPrinter(MAI, MII, MRI);
}

static MCAsmInfo *createSmolMCAsmInfo(const MCRegisterInfo &MRI,
                                       const Triple &TT,
                                       const MCTargetOptions &Options) {
  MCAsmInfo *MAI = new SmolMCAsmInfo(TT);

  unsigned WP = MRI.getDwarfRegNum(Smol::RPS, true);
  MCCFIInstruction Inst = MCCFIInstruction::cfiDefCfa(nullptr, WP, 0);
  MAI->addInitialFrameState(Inst);

  return MAI;
}

extern "C" void LLVMInitializeSmolTargetMC() {
  for (Target *T : {&getTheSmolTarget()}) {
    // Register the MC asm info.
    TargetRegistry::RegisterMCAsmInfo(*T, createSmolMCAsmInfo);

    // Register the MC instruction info.
    TargetRegistry::RegisterMCInstrInfo(*T, createSmolMCInstrInfo);

    // Register the MC register info.
    TargetRegistry::RegisterMCRegInfo(*T, createSmolMCRegisterInfo);

    // Register the MC subtarget info.
    TargetRegistry::RegisterMCSubtargetInfo(*T, createSmolMCSubtargetInfo);

    // Register the MCInstPrinter.
    TargetRegistry::RegisterMCInstPrinter(*T, createSmolMCInstPrinter);
  }
}