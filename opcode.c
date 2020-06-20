#include "opcode.h"
#include <stdio.h>
#include <string.h>

// Assumes check_prefix has determined the correct opcode location
void check_opcode(struct x86_instr * inst, int opcode_index){
    // TODO handle more than 1 byte of opcode
    unsigned char opcode = inst->byte_code[opcode_index]; 
    switch(opcode){
    case(op_call):
        strcat(inst->x86_string, "call ");
        goto exit;
    case(op_mov):
        strcat(inst->x86_string, "mov ");
        goto exit;
    }
    if(op_pop <= opcode &&  opcode < op_pop + 8){
        strcat(inst->x86_string, "pop ");
        goto exit;
    }
    else if(op_push <= opcode && opcode < op_push + 8){
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


