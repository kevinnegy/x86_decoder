#include "opcode.h"
#include <stdio.h>
#include <string.h>
#include "prefix.h"
#include "registers.h"
#include "modrm.h"

// Assumes check_prefix has determined the correct opcode location
void check_opcode(struct x86_instr * inst){
    // TODO handle more than 1 byte of opcode
    unsigned char opcode = inst->opcode[0]; 
    switch(opcode){
    case(op_call):
        strcat(inst->x86_string, "call ");
        goto exit;
    case(op_mov):
        strcat(inst->x86_string, "mov ");
        inst->modrm_ptr = inst->opcode_ptr + 1;
        inst->modrm = inst->byte_code[inst->modrm_ptr];
        if(inst->rex_ptr == -1)
            check_modrm(inst, 32); 
        else
            check_modrm(inst, 64);

        strcat(inst->x86_string, inst->operands->operands[0]);
        strcat(inst->x86_string, ", ");
        strcat(inst->x86_string, inst->operands->operands[1]);
        goto exit;

    case(op_sub):   // 83 \5 (use r/m not reg of modrm for register) and ib (immediate_8 byte)
        strcat(inst->x86_string, "sub ");
        inst->modrm_ptr = inst->opcode_ptr + 1;
        inst->modrm = inst->byte_code[inst->modrm_ptr];
        
        if(inst->rex_ptr == -1)
            check_modrm(inst, 32); 
        else
            check_modrm(inst, 64);

        strcat(inst->x86_string, inst->operands->operands[1]); 
        strcat(inst->x86_string, ", ");
        inst->immediate_ptr = inst->modrm_ptr + 1;
        inst->immediate[0] = inst->byte_code[inst->immediate_ptr];
        unsigned char x = inst->immediate[0];
        char imm[5];
        sprintf(imm, "0x%x",x);
        strcat(inst->x86_string, imm);
        
        goto exit;
    }
    if(op_pop <= opcode &&  opcode < op_pop + 8){

        int bit_mode = check_bit_mode(inst);
        int index = opcode & 0x7;
        // Get extension
        index = index | (get_rex_b(inst) << 3);
        char * reg = get_register(index, 1, bit_mode); //TODO check for w bit?

        // Build string
        strcat(inst->x86_string, "pop ");
        strcat(inst->x86_string, reg);
        goto exit;
    }
    else if(op_push <= opcode && opcode < op_push + 8){

        int bit_mode = check_bit_mode(inst);
        int index = opcode & 0x7;
        index = index | (get_rex_b(inst) << 3);
        char * reg = get_register(index, 1, bit_mode); //TODO check for w bit?
        
        // Build string
        strcat(inst->x86_string, "push ");
        strcat(inst->x86_string, reg);
        goto exit;
    }
    switch(opcode){
    default:
        printf("%s: %s %x\n", __func__, "invalid opcode", opcode);
        goto exit;
    }
exit:
    return;
}


