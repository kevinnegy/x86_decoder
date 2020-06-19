#include "opcode.h"
#include <stdio.h>

// Assumes check_prefix has determined the correct opcode location
void check_opcode(struct x86_instr * inst){
    // TODO handle more than 1 byte of opcode
    unsigned char opcode = inst->byte_code[inst->opcode_ptr]; 
    switch(opcode){
    case(op_call):
        printf("CALL\n");
        break;
    case(op_mov):
        printf("MOV\n");
        break;
    default:
        printf("check_opcode: invalid opcode\n");
        break;
    }
}


