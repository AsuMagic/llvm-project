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
#include "llvm/Support/KnownBits.h"

using namespace llvm;

#define DEBUG_TYPE "riscw-isel"

char SmolDAGToDAGISel::ID = 0;

bool SmolDAGToDAGISel::runOnMachineFunction(MachineFunction &MF) {
  return SelectionDAGISel::runOnMachineFunction(MF);
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