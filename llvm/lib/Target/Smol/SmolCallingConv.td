//===-- SmolCallingConv.td - Calling Conventions Sparc -----*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This describes the calling conventions for the Smol2 architecture.
//
//===----------------------------------------------------------------------===//

def CC_Smol : CallingConv<[
  CCIfByVal<CCPassByVal<4, 4>>,
  CCIfType<[i1, i8, i16], CCPromoteToType<i32>>,
  // i32 arguments get passed in integer registers if there is space.
  CCIfType<[i32], CCAssignToReg<[R0, R1, R2, R3, R4, R5, R6]>>,
  // Alternatively, they are assigned to the stack in 4-byte aligned units.
  CCAssignToStack<4, 4>
]>;

def RetCC_Smol : CallingConv<[
  CCIfType<[i1, i8, i16], CCPromoteToType<i32>>,

  // TODO: define in CPU docs that this can be done
  CCIfType<[i32], CCAssignToReg<[R0, R1, R2, R3, R4, R5, R6]>>,

  // Integer values get stored in stack slots that are 4 bytes in size and
  // 4-byte aligned
  CCIfType<[i32], CCAssignToStack<4, 4>>
]>;

// TODO: SPARC uses register windows, but we don't. Consequences? Check RV?
// // Callee-saved registers are handled by the register window mechanism.
// def CSR : CalleeSavedRegs<(add)> {
//   let OtherPreserved = (add (sequence "I%u", 0, 7),
//                             (sequence "L%u", 0, 7), O6);
// }

// Callee-saved registers for calls with ReturnsTwice attribute.
def CSR : CalleeSavedRegs<(add)> {
  let OtherPreserved = (add R7, R8, R9, R10, R11, R12); // TODO: owo what's this
}
