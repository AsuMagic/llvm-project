//=== SmolMachineFunctionInfo.h - Private data used for Smol2 -----*- C++ -*-=//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file declares the Smol specific subclass of MachineFunctionInfo.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SMOL_SMOLMACHINEFUNCTION_H
#define LLVM_LIB_TARGET_SMOL_SMOLMACHINEFUNCTION_H

#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/TargetSubtargetInfo.h"

namespace llvm {

class SmolFunctionInfo : public MachineFunctionInfo {
private:
  const Function &MF;

public:
  SmolFunctionInfo(const Function &MF, const TargetSubtargetInfo *STI = nullptr) : MF(MF) {}
};

} // namespace llvm

#endif // end LLVM_LIB_TARGET_SMOL_SMOLMACHINEFUNCTION_H