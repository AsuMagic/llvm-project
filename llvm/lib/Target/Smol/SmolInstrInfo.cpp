#include "SmolInstrInfo.h"

#include "SmolSubtarget.h"

#define DEBUG_TYPE "riscw-instrinfo"

#define GET_INSTRINFO_CTOR_DTOR
#include "SmolGenInstrInfo.inc"

namespace llvm
{

SmolInstrInfo::SmolInstrInfo(const SmolSubtarget &STI)
    : SmolGenInstrInfo(),
      Subtarget(STI)
{
}

}