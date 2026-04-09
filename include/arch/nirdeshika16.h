/* 
 * nirdeshika16.h - Nirdeshika-16 ISA Definitions
 * This file defines the Opcodes, Registers, and Instruction Format.
 * Architect: Rahat Hasan | Akkhar Labs
 */

#ifndef NIRDESHIKA16_H
#define NIRDESHIKA16_H

/* --- Opcode Table (ওকোড সারণী) --- 
 * 4-bit Opcodes for Nirdeshika-16 ISA
 */
typedef enum {
    OP_NOP   = 0x0, // শূণ্য - No Operation
    OP_MOV   = 0x1, // স্থানান্তর - Move register to register
    OP_LOAD  = 0x2, // আদান - Load from RAM to Register
    OP_STORE = 0x3, // প্রদান - Store from Register to RAM
    OP_ADD   = 0x4, // সংযোজন - Addition
    OP_SUB   = 0x5, // বিয়োজন - Subtraction
    OP_MUL   = 0x6, // গুনন - Multiplication
    OP_DIV   = 0x7, // বিভাজন - Division
    OP_AND   = 0x8, // উভয় - Bitwise AND
    OP_OR    = 0x9, // অথবা - Bitwise OR
    OP_XOR   = 0xA, // বিচিত্র - Bitwise XOR
    OP_NOT   = 0xB, // বিপরীত - Bitwise NOT
    OP_JMP   = 0xC, // প্রস্থান - Unconditional Jump
    OP_JZ    = 0xD, // সাম্য-প্রস্থান - Jump if Zero
    OP_OUT   = 0xE, // প্রদর্শন - Output to port/screen
    OP_HALT  = 0xF  // থামো - Halt Processor
} Opcode;

/* --- Register ID Table (সাময়িক আইডি সারণী) --- 
 * 4-bit General Purpose Registers (GPR)
 */
typedef enum {
    REG_S0 = 0x0, REG_S1 = 0x1, REG_S2 = 0x2, REG_S3 = 0x3,
    REG_S4 = 0x4, REG_S5 = 0x5, REG_S6 = 0x6, REG_S7 = 0x7,
    REG_S8 = 0x8, REG_S9 = 0x9, REG_S10 = 0xA, REG_S11 = 0xB,
    REG_S12 = 0xC, REG_S13 = 0xD, REG_S14 = 0xE, REG_S15 = 0xF
} Register;

/* --- Special Purpose Registers (বিশেষ সাময়িক) --- */
#define REG_PC    REG_S12 // ন-নি (Program Counter)
#define REG_SP    REG_S13 // শ-নি (Stack Pointer)
#define REG_FLAGS REG_S14 // অ-সা (Status Flags)

/* --- Instruction Format (4-4-8) --- 
 * [Opcode: 15-12] [Register: 11-8] [Immediate/Address: 7-0]
 */
#define BUILD_INSTR(op, reg, val) ( ((op & 0xF) << 12) | ((reg & 0xF) << 8) | (val & 0xFF) )

/* Macros to extract data from an instruction */
#define GET_OPCODE(instr) ((instr >> 12) & 0xF)
#define GET_REG(instr)    ((instr >> 8) & 0xF)
#define GET_VAL(instr)    (instr & 0xFF)

#endif // NIRDESHIKA16_H