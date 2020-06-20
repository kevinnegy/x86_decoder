#include "opcode.h"
#include <stdio.h>
#include <string.h>

// Assumes check_prefix has determined the correct opcode location
void check_opcode(struct x86_instr * inst){
    // TODO handle more than 1 byte of opcode
    unsigned char opcode = inst->byte_code[inst->opcode_ptr]; 
    switch(opcode){
    case(op_call):
        strcat(inst->x86_string, "call ");
        goto exit;
    case(op_mov):
        strcat(inst->x86_string, "mov ");
        goto exit;
    }
    if(opcode >> 3 == op_pop >> 3){
        strcat(inst->x86_string, "pop ");
        goto exit;
    }
    else if(opcode >> 4 == op_push >> 4){
        strcat(inst->x86_string, "push ");
        goto exit;
    }
    switch(opcode){
    default:
        printf("check_opcode: invalid opcode\n");
        goto exit;
    }
exit:
    return;
}


