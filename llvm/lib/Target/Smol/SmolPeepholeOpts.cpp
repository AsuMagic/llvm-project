//===-- SmolPeepholeOpts.cpp - Smol2 Peephole Optimiztions ----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Late peephole optimizations for Smol2.
///
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/SmolMCTargetDesc.h"
#include "Smol.h"
#include "SmolInstrInfo.h"
#include "SmolRegisterInfo.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineOperand.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/Pass.h"
#include "llvm/Support/CommandLine.h"
using namespace llvm;

#define DEBUG_TYPE "smol-peephole"

static cl::opt<bool> DisableSmolMergedALUTestOpt(
    "disable-smol-merged-alu-test-opt", cl::Hidden,
    cl::desc("Smol2: Disable optimizations that merge tests with certain ALU "
             "operations"),
    cl::init(false));

namespace {
class SmolPeephole final : public MachineFunctionPass {
private:
  StringRef getPassName() const override {
    return "Smol2 late peephole optimizer";
  }

  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesCFG();
    // AU.addRequired<TargetLibraryInfoWrapperPass>();
    MachineFunctionPass::getAnalysisUsage(AU);
  }

  bool runOnMachineFunction(MachineFunction &MF) override;

  /// try transforming the following sequence:
  ///
  /// Matches `iaddsi $x, y; tnei $t, $x, 0`
  ///
  /// Into `iaddsi_tnz $x, y, $t`
  bool tryMergeAddTNZ(MachineFunction &MF, MachineBasicBlock &MBB,
                      MachineInstr &MI);

  const SmolInstrInfo *TII;
  const SmolRegisterInfo *TRI;
  MachineRegisterInfo *MRI;

public:
  static char ID;
  SmolPeephole() : MachineFunctionPass(ID) {}
};

char SmolPeephole::ID = 0;

} // namespace

INITIALIZE_PASS(SmolPeephole, DEBUG_TYPE, "Smol2 peephole optimizations", false,
                false)

FunctionPass *llvm::createSmolPeephole() { return new SmolPeephole(); }

bool SmolPeephole::tryMergeAddTNZ(MachineFunction &MF, MachineBasicBlock &MBB,
                                  MachineInstr &TestMI) {
  assert(TestMI.getOpcode() == Smol::TNEI);
  assert(TestMI.getNumOperands() == 3);

  const MachineOperand &TestBit = TestMI.getOperand(0);
  const MachineOperand &TestLHS = TestMI.getOperand(1);
  const MachineOperand &TestRHS = TestMI.getOperand(2);

  // Match RHS == 0 (in `tnei $t, $x, 0`)
  if (TestRHS.getType() != MachineOperand::MO_Immediate ||
      TestRHS.getImm() != 0) {
    return false;
  }

  // Match LHS == reg (should always be the case)
  if (TestLHS.getType() != MachineOperand::MO_Register) {
    return false;
  }

  // Look up whatever instruction assigned the virtual register behind the LHS,
  // which might be the ADDSI we're looking for
  MachineInstr &SrcMI = *MRI->getUniqueVRegDef(TestLHS.getReg());

  // Match LHS source as ADDSI
  if (SrcMI.getOpcode() != Smol::IADDSI) {
    return false;
  }

  const MachineOperand &AddOutput = SrcMI.getOperand(0);
  const MachineOperand &AddLHS = SrcMI.getOperand(1);
  const MachineOperand &AddRHS = SrcMI.getOperand(2);

  MachineInstr &InsertPoint = SrcMI;

  BuildMI(*TestMI.getParent(), InsertPoint, TestMI.getDebugLoc(),
          TII->get(Smol::IADDSITNZ))
      .addReg(AddOutput.getReg(), RegState::Define)
      .addReg(TestBit.getReg(), RegState::Define)
      .addReg(AddLHS.getReg())
      .addImm(AddRHS.getImm());

  SrcMI.eraseFromParent();
  TestMI.eraseFromParent();

  return true;
}

bool SmolPeephole::runOnMachineFunction(MachineFunction &MF) {
  LLVM_DEBUG({
    dbgs() << "********** Peephole **********\n"
           << "********** Function: " << MF.getName() << '\n';
  });

  TII = static_cast<const SmolInstrInfo *>(MF.getSubtarget().getInstrInfo());
  TRI = static_cast<const SmolRegisterInfo *>(
      MF.getSubtarget().getRegisterInfo());
  MRI = &MF.getRegInfo();

  bool Changed = false;

  for (auto &MBB : MF) {
    for (auto &MI : make_early_inc_range(MBB)) {
      switch (MI.getOpcode()) {
      case Smol::TNEI: {
        Changed |= tryMergeAddTNZ(MF, MBB, MI);
        break;
      }
      }
    }
  }

  return Changed;
}