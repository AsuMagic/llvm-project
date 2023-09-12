//===-- SmolMCTargetDesc.h - Smol2 Target Descriptions ----------*- C++ -*-===//
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

#ifndef LLVM_LIB_TARGET_SMOL_MCTARGETDESC_SMOLMCTARGETDESC_H
#define LLVM_LIB_TARGET_SMOL_MCTARGETDESC_SMOLMCTARGETDESC_H

// #include "SmolBaseInfo.h"

// Defines symbolic names for Smol registers. This defines a mapping from
// register name to register number.
#define GET_REGINFO_ENUM
#include "SmolGenRegisterInfo.inc"

// Defines symbolic names for the RISCW instructions.
#define GET_INSTRINFO_ENUM
#include "SmolGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "SmolGenSubtargetInfo.inc"

#endif // LLVM_LIB_TARGET_SMOL_MCTARGETDESC_SMOLMCTARGETDESC_H