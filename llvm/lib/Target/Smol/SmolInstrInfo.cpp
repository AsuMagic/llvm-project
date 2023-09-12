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

static void parseCondBranch(MachineInstr &LastInst, MachineBasicBlock *&Target,
                            SmallVectorImpl<MachineOperand> &Cond) {
  assert(LastInst.isConditionalBranch());

  // brcond cond, iftrue
  Target = LastInst.getOperand(1).getMBB();
  Cond.push_back(LastInst.getOperand(0));
}

bool SmolInstrInfo::analyzeBranch(MachineBasicBlock &MBB,
                                  MachineBasicBlock *&TBB,
                                  MachineBasicBlock *&FBB,
                                  SmallVectorImpl<MachineOperand> &Cond,
                                  bool AllowModify) const {
  TBB = FBB = nullptr;
  Cond.clear();

  // 1. If this block ends with no branches (it just falls through to its succ)
  //    just return false, leaving TBB/FBB null.
  const MachineBasicBlock::iterator LastI = MBB.getLastNonDebugInstr();
  if (LastI == MBB.end() || !isUnpredicatedTerminator(*LastI)) {
    return false;
  }

  const auto Terms = MBB.terminators();
  const auto NumTerms = std::distance(Terms.begin(), Terms.end());
  assert(NumTerms > 0);

  // Can't do much with an indirect branch
  if (std::any_of(Terms.begin(), Terms.end(), [&](const MachineInstr &Term) {
        return Term.getDesc().isIndirectBranch();
      })) {
    return true;
  }

  // Nothing we know how to optimize
  if (NumTerms > 2) {
    return true;
  }

  // 2. If this block ends with only an unconditional branch, it sets TBB to be
  //    the destination block.
  if (NumTerms == 1 && Terms.begin()->getDesc().isUnconditionalBranch()) {
    TBB = getBranchDestBlock(*Terms.begin());
    return false;
  }

  // 3. If this block ends with a conditional branch and it falls through to a
  //    successor block, it sets TBB to be the branch destination block and a
  //    list of operands that evaluate the condition. These operands can be
  //    passed to other TargetInstrInfo methods to create new branches.
  if (NumTerms == 1 && Terms.begin()->getDesc().isConditionalBranch()) {
    parseCondBranch(*Terms.begin(), TBB, Cond);
    return false;
  }

  // 4. If this block ends with a conditional branch followed by an
  //    unconditional branch, it returns the 'true' destination in TBB, the
  //    'false' destination in FBB, and a list of operands that evaluate the
  //    condition.  These operands can be passed to other TargetInstrInfo
  //    methods to create new branches.
  if (NumTerms == 2 && Terms.begin()->getDesc().isConditionalBranch() &&
      std::next(Terms.begin())->getDesc().isUnconditionalBranch()) {
    parseCondBranch(*Terms.begin(), TBB, Cond);
    FBB = getBranchDestBlock(*std::next(Terms.begin()));
    return false;
  }

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
  assert(/* Uncond branch */ Cond.size() == 0 ||
         /* Cond branch where Cond={predicate} */ Cond.size() == 1);

  if (Cond.empty()) {
    // Emit unconditional branch to the TBB
    MachineInstr &MI = *BuildMI(&MBB, DL, get(Smol::PseudoBR)).addMBB(TBB);
    if (BytesAdded != nullptr) {
      *BytesAdded += getInstSizeInBytes(MI);
    }
    return 1;
  }

  // From this point on, it's a conditional op (Cond.size() == 1)
  // Emit a conditional branch to the TBB in any case

  MachineInstr &CondMI =
      *BuildMI(&MBB, DL, get(Smol::CJI)).add(Cond[0]).addMBB(TBB);

  if (BytesAdded != nullptr) {
    *BytesAdded += getInstSizeInBytes(CondMI);
  }

  // If no FBB then the MBB just falls through
  if (FBB == nullptr) {
    return 1;
  }

  // Otherwise we emit a branch to the FBB
  MachineInstr &MI = *BuildMI(&MBB, DL, get(Smol::PseudoBR)).addMBB(FBB);
  if (BytesAdded != nullptr) {
    *BytesAdded += getInstSizeInBytes(MI);
  }

  return 2;
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