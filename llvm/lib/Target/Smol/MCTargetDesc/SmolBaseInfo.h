//===-- SmolBaseInfo.h - Top level definitions for Smol2 MC -----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains small standalone enum definitions for the Smol2 target
// useful for the compiler back-end and the MC libraries.
//
//===----------------------------------------------------------------------===//
#ifndef LLVM_LIB_TARGET_SMOL_MCTARGETDESC_SMOLBASEINFO_H
#define LLVM_LIB_TARGET_SMOL_MCTARGETDESC_SMOLBASEINFO_H

namespace llvm {

// SmolII - This namespace holds all of the target specific flags that
// instruction info tracks. All definitions must match SmolInstrFormats.td.
namespace SmolII {

enum {
  MO_None = 0,
  MO_LO24 = 1,
  MO_HI8 = 2
};

} // namespace SmolII
} // namespace llvm

#endif