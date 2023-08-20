add_llvm_component_library(LLVMSmolDesc
  SmolInstPrinter.cpp
  SmolMCAsmInfo.cpp
  SmolMCTargetDesc.cpp

  LINK_COMPONENTS
  MC
  Support
  TargetParser

  ADD_TO_COMPONENT
  Smol
)
