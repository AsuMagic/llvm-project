#include "SmolInstrInfo.h"

#include "SmolSubtarget.h"

#define DEBUG_TYPE "riscw-instrinfo"

#define GET_INSTRINFO_CTOR_DTOR
#define GET_INSTRINFO_ENUM
#include "SmolGenInstrInfo.inc"

namespace llvm
{

SmolInstrInfo::SmolInstrInfo(const SmolSubtarget &STI)
    : SmolGenInstrInfo(),
      Subtarget(STI)
{
}

void SmolInstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                                MachineBasicBlock::iterator I,
                                const DebugLoc &DL, MCRegister DestReg,
                                MCRegister SrcReg, bool KillSrc) const {
  BuildMI(MBB, I, DL, get(Smol::LR), DestReg)
      .addReg(SrcReg, getKillRegState(SrcReg));
}

}