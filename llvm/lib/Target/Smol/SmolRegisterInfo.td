//===-- SparcRegisterInfo.td - Sparc Register defs ---------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Declarations that describe the Sparc register file
//===----------------------------------------------------------------------===//

class SmolReg<bits<16> Enc, string n> : Register<n> {
  let HWEncoding = Enc;
  let Namespace = "Smol";
}

// Registers are identified with 5-bit ID numbers.
// Ri - 32-bit integer registers
class Ri<bits<16> Enc, string n> : SmolReg<Enc, n>;

// Integer registers
def R0  : Ri< 0, "R0">, DwarfRegNum<[0]>;
def R1  : Ri< 1, "R1">, DwarfRegNum<[1]>;
def R2  : Ri< 2, "R2">, DwarfRegNum<[2]>;
def R3  : Ri< 3, "R3">, DwarfRegNum<[3]>;
def R4  : Ri< 4, "R4">, DwarfRegNum<[4]>;
def R5  : Ri< 5, "R5">, DwarfRegNum<[5]>;
def R6  : Ri< 6, "R6">, DwarfRegNum<[6]>;
def R7  : Ri< 7, "R7">, DwarfRegNum<[7]>;
def R8  : Ri< 8, "R8">, DwarfRegNum<[8]>;
def R9  : Ri< 9, "R9">, DwarfRegNum<[9]>;
def R10 : Ri<10, "R10">, DwarfRegNum<[10]>;
def R11 : Ri<11, "R11">, DwarfRegNum<[11]>;
def R12 : Ri<12, "R12">, DwarfRegNum<[12]>;

def RRET : Ri<13, "RRET">, DwarfRegNum<[13]>;
def RPL  : Ri<14, "RPL">, DwarfRegNum<[14]>;
def RPS  : Ri<15, "RPS">, DwarfRegNum<[15]>;

// Register classes.

// The order of registers represents the preferred allocation sequence.
// Registers are listed in the order caller-save, callee-save, specials.

def GPR : RegisterClass<"Smol", [i32], 16,
    (add // Caller saved
         R0, R1, R2, R3, R4,
         // Callee saved
         R5, R6, R7, R8, R9, R10, R11, R12,
         // Specific purpose
         RRET, RPL, RPS)>;

// FIXME: nothing to say RRET/RPL/RPS are not allocatable?

// TODO: how to represent R2 and Rh2? (thumb might be helpful here)