//===---- SmolISelDAGToDAG.h - A Dag to Dag Inst Selector for Smol2 -------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines an instruction selector for the RISCW target.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_RISCW_RISCWISELDAGTODAG_H
#define LLVM_LIB_TARGET_RISCW_RISCWISELDAGTODAG_H

#include "SmolTargetMachine.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/SelectionDAGISel.h"

// FIXME: wtf is this include hell
#define GET_INSTRINFO_ENUM
#include "SmolGenInstrInfo.inc"

namespace llvm {
class SmolDAGToDAGISel : public SelectionDAGISel {
public:
  static char ID;

  explicit SmolDAGToDAGISel(SmolTargetMachine &TM, CodeGenOpt::Level OL)
      : SelectionDAGISel(ID, TM, OL) {}

  // Pass Name
  StringRef getPassName() const override {
    return "Smol2 DAG->DAG Pattern Instruction Selection";
  }

  bool SelectAddrRegImm(SDValue Addr, SDValue &Base, SDValue &Offset, bool SExt,
                        int64_t TargetSize);

  // TODO: find a wa not to require that; can we forward constants to the
  // complex pattern function in SmolInstrFormat so that we can just give
  // sext & target size?
  bool SelectAddrRegImmU6(SDValue Addr, SDValue &Base, SDValue &Offset) {
    return SelectAddrRegImm(Addr, Base, Offset, false, 6);
  }
  bool SelectAddrRegImmU7(SDValue Addr, SDValue &Base, SDValue &Offset) {
    return SelectAddrRegImm(Addr, Base, Offset, false, 7);
  }
  bool SelectAddrRegImmU8(SDValue Addr, SDValue &Base, SDValue &Offset) {
    return SelectAddrRegImm(Addr, Base, Offset, false, 8);
  }
  bool SelectAddrRegImmS16(SDValue Addr, SDValue &Base, SDValue &Offset) {
    return SelectAddrRegImm(Addr, Base, Offset, true, 16);
  }
  bool SelectAddrRegImmS17(SDValue Addr, SDValue &Base, SDValue &Offset) {
    return SelectAddrRegImm(Addr, Base, Offset, true, 17);
  }
  bool SelectAddrRegImmS18(SDValue Addr, SDValue &Base, SDValue &Offset) {
    return SelectAddrRegImm(Addr, Base, Offset, true, 18);
  }

  // bool SelectAddr

  bool runOnMachineFunction(MachineFunction &MF) override;

  void Select(SDNode *Node) override;

#include "SmolGenDAGISel.inc"
};
} // namespace llvm

#endif // end LLVM_LIB_TARGET_RISCW_RISCWISELDAGTODAG_H