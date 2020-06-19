#ifndef OPCODE_H
#define OPCODE_H

#include "instruction.h"

/** Opcodes **/
// Can be 1,2, or 3 bytes. A fourth can be in ModRM
enum opcodes{
    op_call = 0xe8,
    op_mov = 0x89,
};

void check_opcode(struct x86_instr * inst);

// In 2 byte mode, it must have one of the following (3 byte is the same except a third opcode):
    // escape opcode 0F primary opcode and then a second opcode
    // mandatory prefix (66, F2, or F3), then escape, then second opcode

// Registers (Prefix E for 32bit, prefix R for 64bit) 
    // 000 - AX 
    // 001 - CX 
    // 010 - DX 
    // 011 - BX 
    // 100 - SP 
    // 101 - BP 
    // 110 - SI 
    // 111 - DI 

// If w bit exists, use above encoding when w = 1, if 0:
    // 000 - AL 
    // 001 - CL 
    // 010 - DL 
    // 011 - BL 
    // 100 - AH 
    // 101 - CH 
    // 110 - DH 
    // 111 - BH 

#endif
