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

/*
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

  // Unknown branch type at this point
  return true;
}
*/
} // namespace llvm