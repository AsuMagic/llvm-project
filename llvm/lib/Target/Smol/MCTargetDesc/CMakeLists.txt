add_llvm_component_library(LLVMSmolDesc
  SmolInstPrinter.cpp
  SmolMCAsmInfo.cpp
  SmolMCTargetDesc.cpp
  SmolMCExpr.cpp

  LINK_COMPONENTS
  MC
  SmolInfo
  Support
  TargetParser

  ADD_TO_COMPONENT
  Smol
)
