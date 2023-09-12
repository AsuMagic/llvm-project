//===-- SmolISelDAGToDAG.cpp - A Dag to Dag Inst Selector for Smol2 -------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines an instruction selector for the Smol target.
//
//===----------------------------------------------------------------------===//

#include "SmolISelDAGToDAG.h"
#include "SmolRegisterInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/CodeGen/SelectionDAGNodes.h"
#include "llvm/Support/KnownBits.h"
#include <cstdint>

using namespace llvm;

#define DEBUG_TYPE "riscw-isel"

char SmolDAGToDAGISel::ID = 0;

bool SmolDAGToDAGISel::runOnMachineFunction(MachineFunction &MF) {
  return SelectionDAGISel::runOnMachineFunction(MF);
}

bool SmolDAGToDAGISel::selectAddrRegImm(SDValue Addr, SDValue &Base,
                                        SDValue &Offset, bool SExt,
                                        int64_t TargetSize) {
  SDLoc DL(Addr);
  MVT VT = Addr.getSimpleValueType();

  if (CurDAG->isBaseWithConstantOffset(Addr)) {
    auto *const CNode = cast<ConstantSDNode>(Addr.getOperand(1));

    uint64_t CVal;
    bool Fits;

    if (SExt) {
      CVal = CNode->getSExtValue();
      Fits = (isIntN(TargetSize, CVal));
    } else {
      CVal = CNode->getZExtValue();
      Fits = (isUIntN(TargetSize, CVal));
    }
    
    if (Fits) {
      Base = Addr.getOperand(0);
      Offset = CurDAG->getTargetConstant(CVal, DL, VT);
      return true;
    }
  }

  return false;
}

void SmolDAGToDAGISel::Select(SDNode *Node) {
  // If we have a custom node, we already have selected!
  if (Node->isMachineOpcode()) {
    LLVM_DEBUG(errs() << "== "; Node->dump(CurDAG); errs() << "\n");
    Node->setNodeId(-1);
    return;
  }

  // Instruction Selection not handled by the auto-generated tablegen selection
  // should be handled here.
  unsigned Opcode = Node->getOpcode();
  SDLoc DL(Node);

  // Select the default instruction
  SelectCode(Node);
}