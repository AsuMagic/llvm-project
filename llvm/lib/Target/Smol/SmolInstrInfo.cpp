#include "SmolInstrInfo.h"

#define DEBUG_TYPE "riscw-instrinfo"

#define GET_INSTRINFO_CTOR_DTOR
#include "SmolGenInstrInfo.inc"

namespace llvm
{

SmolInstrInfo::SmolInstrInfo()
    : SmolGenInstrInfo()
{
}

}