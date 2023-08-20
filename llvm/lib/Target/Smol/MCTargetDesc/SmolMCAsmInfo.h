//===-- SmolMCAsmInfo.h - Smol2 Asm Info -------------------------*- C++ -*--===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the SmolMCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_SMOL_MCTARGETDESC_SMOLMCASMINFO_H
#define LLVM_LIB_TARGET_SMOL_MCTARGETDESC_SMOLMCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
  class Triple;

class SmolMCAsmInfo : public MCAsmInfoELF {
  void anchor() override;

public:
  explicit SmolMCAsmInfo(const Triple &TheTriple);
};

} // namespace llvm

#endif // end LLVM_LIB_TARGET_RISCW_MCTARGETDESC_RISCWMCASMINFO_H