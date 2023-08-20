add_llvm_component_group(Smol)

set(LLVM_TARGET_DEFINITIONS Smol.td)

tablegen(LLVM SmolGenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM SmolGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM SmolGenCallingConv.inc -gen-callingconv)
tablegen(LLVM SmolGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM SmolGenDisassemblerTables.inc -gen-disassembler)
tablegen(LLVM SmolGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM SmolGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM SmolGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM SmolGenSubtargetInfo.inc -gen-subtarget)

add_public_tablegen_target(SmolCommonTableGen)

add_llvm_target(SmolCodeGen
  SmolAsmPrinter.cpp
  SmolInstrInfo.cpp
  SmolISelDAGToDAG.cpp
  # SmolISelLowering.cpp
  SmolFrameLowering.cpp
  SmolRegisterInfo.cpp
  SmolTargetMachine.cpp
  # SmolMCInstLower.cpp
  SmolTargetObjectFile.cpp

  LINK_COMPONENTS
  AsmPrinter
  CodeGen
  CodeGenTypes
  Core
  MC
  SelectionDAG
  SmolDesc
  SmolInfo
  Support
  Target
  TargetParser

  ADD_TO_COMPONENT
  Smol
  )

# add_subdirectory(AsmParser)
# add_subdirectory(Disassembler)
add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
