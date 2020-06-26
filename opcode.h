#ifndef OPCODE_H
#define OPCODE_H

#include "instruction.h"

/** Opcodes **/
// Can be 1,2, or 3 bytes. A fourth can be in ModRM
enum one_byte_opcodes{
    OP_ADD_01 = 0x1,
    OP_CALL_E8 = 0xe8,
    OP_JMP_EB = 0xeb,
    OP_LEA = 0x8d, // load effective address
    OP_MOV = 0x89,
    OP_MOV_8B = 0x8b,
    OP_MOV_B8 = 0xb8, // mov uses last 3 bits for register, b8-bf, moves imm to reg
    OP_OR = 0x9,
    OP_POP = 0x58, // pop is 58-5f, last 3 bits are for register
    OP_PUSH = 0x50, // push is 50-57, last 3 bits are for register
    OP_PUSHA = 0x60, // push all 
    OP_POPA = 0x61, // pop all
    OP_SHL = 0xc1, // shift logical left always imm8
    OP_SUB_83 = 0x83, 
    OP_SUB_2B = 0x2b, 
    OP_TEST_85 = 0x85, // test = reg_1 & reg_2 -> SF, ZF, PF flags
};

enum two_byte_opcodes{
    OP_JZJE_84 = 0x84, // jump if equal or zero (meaning if zf flag = 1)
    OP_RDTSC = 0x31, // read time stamp counter
};

void check_rex_inst(unsigned char * inst, int rex_index);
void check_inst(unsigned char * inst, int opcode_index);

// In 2 byte mode, it must have one of the following (3 byte is the same except a third opcode):
    // escape opcode 0F primary opcode and then a second opcode
    // mandatory prefix (66, F2, or F3), then escape, then second opcode

#endif
