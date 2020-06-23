#ifndef OPCODE_H
#define OPCODE_H

#include "instruction.h"

/** Opcodes **/
// Can be 1,2, or 3 bytes. A fourth can be in ModRM
enum one_byte_opcodes{
    op_call = 0xe8,
    op_mov = 0x89,
    op_mov_8b = 0x8b,
    op_or = 0x9,
    op_pop = 0x58, // pop is 58-5f, last 3 bits are for register
    op_push = 0x50, // push is 50-57, last 3 bits are for register
    op_pusha = 0x60, // push all 
    op_popa = 0x61, // pop all
    op_shl = 0xc1, // shift logical left
    op_sub = 0x83, 
};

enum two_byte_opcodes{
    op_rdtsc = 0x31, // read time stamp counter
};

void check_opcode(struct x86_instr * inst);

// In 2 byte mode, it must have one of the following (3 byte is the same except a third opcode):
    // escape opcode 0F primary opcode and then a second opcode
    // mandatory prefix (66, F2, or F3), then escape, then second opcode

#endif
