//===-- SmolTargetMachine.cpp - Define TargetMachine for Sparc -----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
//
//===----------------------------------------------------------------------===//

#include "SmolTargetMachine.h"
// #include "Smol.h"
#include "SmolISelDAGToDAG.h"
#include "SmolMachineFunction.h"
#include "SmolTargetObjectFile.h"
#include "TargetInfo/SmolTargetInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/MC/TargetRegistry.h"
#include <optional>
using namespace llvm;

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeSmolTarget() {
  // Register the target.
  RegisterTargetMachine<SmolTargetMachine> X(getTheSmolTarget());

  // PassRegistry &PR = *PassRegistry::getPassRegistry();
  // initializeSmolDAGToDAGISelPass(PR);
}

/// Create an ILP32 architecture model
SmolTargetMachine::SmolTargetMachine(const Target &T, const Triple &TT,
                                     StringRef CPU, StringRef FS,
                                     const TargetOptions &Options,
                                     std::optional<Reloc::Model> RM,
                                     std::optional<CodeModel::Model> CM,
                                     CodeGenOpt::Level OL, bool JIT)
    : LLVMTargetMachine(T, "e-p:32:32-S32-n8:16:32", TT, CPU, FS, Options,
                        Reloc::Static, // TODO: PIC_ probably better?
                        CodeModel::Small,
                        OL),
      TLOF(std::make_unique<SmolTargetObjectFile>()) {
  initAsmInfo();
}

SmolTargetMachine::~SmolTargetMachine() = default;

MachineFunctionInfo *SmolTargetMachine::createMachineFunctionInfo(
    BumpPtrAllocator &Allocator, const Function &F,
    const TargetSubtargetInfo *STI) const {
  return SmolFunctionInfo::create<SmolFunctionInfo>(Allocator, F, STI);
}

namespace {
/// Smol2 Code Generator Pass Configuration Options.
class SmolPassConfig : public TargetPassConfig {
public:
  SmolPassConfig(SmolTargetMachine &TM, PassManagerBase &PM)
    : TargetPassConfig(TM, PM) {}

  SmolTargetMachine &getSmolTargetMachine() const {
    return getTM<SmolTargetMachine>();
  }

  void addIRPasses() override;
  bool addInstSelector() override;
  void addPreEmitPass() override;
};
} // namespace

TargetPassConfig *SmolTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new SmolPassConfig(*this, PM);
}

void SmolPassConfig::addIRPasses() {
  addPass(createAtomicExpandPass());

  TargetPassConfig::addIRPasses();
}

bool SmolPassConfig::addInstSelector() {
  addPass(new SmolDAGToDAGISel(getSmolTargetMachine(), getOptLevel()));
  return false;
}

void SmolPassConfig::addPreEmitPass(){
}
