#include "SmolInstrInfo.h"

#include "MCTargetDesc/SmolBaseInfo.h"

#include "SmolRegisterInfo.h"
#include "SmolSubtarget.h"
#include "llvm/CodeGen/MachineBasicBlock.h"
#include "llvm/Support/ErrorHandling.h"

#define DEBUG_TYPE "riscw-instrinfo"

#define GET_INSTRINFO_CTOR_DTOR
#define GET_INSTRINFO_ENUM
#include "SmolGenInstrInfo.inc"

namespace llvm {

SmolInstrInfo::SmolInstrInfo(const SmolSubtarget &STI)
    : SmolGenInstrInfo(), Subtarget(STI) {}

void SmolInstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                                MachineBasicBlock::iterator I,
                                const DebugLoc &DL, MCRegister DestReg,
                                MCRegister SrcReg, bool KillSrc) const {
  if (Smol::GPRRegClass.contains(SrcReg) &&
      Smol::GPRRegClass.contains(DestReg)) {
    BuildMI(MBB, I, DL, get(Smol::LR), DestReg)
        .addReg(SrcReg, getKillRegState(SrcReg));
    return;
  }

  if (Smol::CondFlagsRegClass.contains(SrcReg) ||
      Smol::CondFlagsRegClass.contains(DestReg)) {
    llvm_unreachable("Currently cannot copyPhysReg condflags");
  }

  LLVM_DEBUG(dbgs() << "SrcReg.id() == " << SrcReg.id()
                    << ", DestReg.id() == " << DestReg.id() << '\n');

  llvm_unreachable("Cannot emit physreg copy instruction");
}

ArrayRef<std::pair<unsigned, const char *>>
SmolInstrInfo::getSerializableDirectMachineOperandTargetFlags() const {
  using namespace SmolII;
  static const std::pair<unsigned, const char *> TargetFlags[] = {
      {MO_LO24, "smol-lo"}, {MO_HI8, "smol-hi"}};
  return ArrayRef(TargetFlags);
}

MachineBasicBlock *
SmolInstrInfo::getBranchDestBlock(const MachineInstr &MI) const {
  assert(MI.getDesc().isBranch());

  // The branch target is always the last operand.
  int NumOp = MI.getNumExplicitOperands();
  return MI.getOperand(NumOp - 1).getMBB();
}

bool SmolInstrInfo::analyzeBranch(MachineBasicBlock &MBB,
                                  MachineBasicBlock *&TBB,
                                  MachineBasicBlock *&FBB,
                                  SmallVectorImpl<MachineOperand> &Cond,
                                  bool AllowModify) const {
  TBB = FBB = nullptr;
  Cond.clear();

  // No terminator? Fall through to the next, keep TBB/FBB null
  const MachineBasicBlock::iterator LastI = MBB.getLastNonDebugInstr();
  if (LastI == MBB.end() || !isUnpredicatedTerminator(*LastI)) {
    return false;
  }

  const auto Terms = MBB.terminators();
  const auto NumTerms = std::distance(Terms.begin(), Terms.end());
  assert(NumTerms > 0);

  // Can't do much with an indirect branch
  if (Terms.begin()->getDesc().isIndirectBranch()) {
    return true;
  }

  // Nothing we know how to optimize
  if (NumTerms > 2) {
    return true;
  }

  // Match single unconditional branch at the end of the block,
  // i.e. MBB always precedes TBB
  if (NumTerms == 1 && Terms.begin()->getDesc().isUnconditionalBranch()) {
    TBB = getBranchDestBlock(*Terms.begin());
    return false;
  }

  // // Match single conditional branch at the end of the block
  // // TODO: Means the block falls through?
  // if (NumTerms == 1 && Terms.begin()->getDesc().isConditionalBranch()) {

  // }

  // Unknown branch type at this point
  return true;
}

unsigned SmolInstrInfo::insertBranch(
    MachineBasicBlock &MBB, MachineBasicBlock *TBB, MachineBasicBlock *FBB,
    ArrayRef<MachineOperand> Cond, const DebugLoc &DL, int *BytesAdded) const {
  if (BytesAdded != nullptr) {
    *BytesAdded = 0;
  }

  assert(TBB && "insertBranch must not be told to insert a fallthrough");
  assert(Cond.size() == 0);

  if (Cond.empty()) {
    MachineInstr &MI = *BuildMI(&MBB, DL, get(Smol::PseudoBR)).addMBB(TBB);
    if (BytesAdded != nullptr) {
      *BytesAdded += getInstSizeInBytes(MI);
    }
    return 1;
  }

  return 0;
}

unsigned SmolInstrInfo::removeBranch(MachineBasicBlock &MBB,
                                     int *BytesRemoved) const {
  if (BytesRemoved != nullptr) {
    *BytesRemoved = 0;
  }

  unsigned RemovedCount = 0;

  for (;;) {
    MachineBasicBlock::iterator I = MBB.getLastNonDebugInstr();
    if (I == MBB.end()) {
      break;
    }

    if (!I->isBranch() || (!I->getDesc().isConditionalBranch() &&
                           !I->getDesc().isUnconditionalBranch())) {
      break;
    }

    if (BytesRemoved != nullptr) {
      *BytesRemoved += getInstSizeInBytes(*I);
    }

    I->eraseFromParent();
    ++RemovedCount;
  }

  return RemovedCount;
}

} // namespace llvm