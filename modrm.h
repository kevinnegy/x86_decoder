#ifndef MODRM_H
#define MODRM_H

#include "prefix.h"

#define MODRM 0xc0
#define REG   0x38
#define RM    0x07

// If 64-bit mode, without rex byte, most operands are 32 bit and addressing is 64 bits
    // if rex byte exists and w = 1, operand is 64 bit
    // if operand_size_override is on, (no rex byte should be used) switch to 16 bit operand
    // if address_size_override is on, (no rex byte should be used) switch to 16 bit addressing 
void check_modrm_rm(unsigned char * inst, int operand_size, int address_size, struct prefixes * prfx);
void check_modrm_reg(unsigned char * inst, int operand_size, int address_size, struct prefixes * prfx);

/** ModR/M **/
// ModR/M byte - addressing mode byte:
    // mod first two bits - something
    // reg/opcode 3bits - a register or further opcode
    // r/m 3 bits - another register or further opcode

// some ModR/M bytes require a second byte SIB 
// Scaled index byte:
    // scale
    // index - index register
    // base - base register


#endif
