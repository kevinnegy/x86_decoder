#ifndef OPCODE_H
#define OPCODE_H

#include "instruction.h"

/** Opcodes **/
// Can be 1,2, or 3 bytes. A fourth can be in ModRM
enum opcodes{
    op_call = 0xe8,
    op_mov = 0x89,
    op_pop = 0x58, // pop is 58-5f, last 3 bits are for register
    op_push = 0x50, // push is 50-57, last 3 bits are for register
    op_pusha = 0x60,
    op_popa = 0x61,
    
};

void check_opcode(struct x86_instr * inst, int opcode_index);

// In 2 byte mode, it must have one of the following (3 byte is the same except a third opcode):
    // escape opcode 0F primary opcode and then a second opcode
    // mandatory prefix (66, F2, or F3), then escape, then second opcode

#endif
