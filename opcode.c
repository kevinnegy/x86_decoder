#include "opcode.h"
#include <stdio.h>
#include <string.h>
#include "prefix.h"
#include "registers.h"
#include "modrm.h"
#include "immediates.h"

void check_third_byte_opcode(struct x86_instr * inst){;
}

void check_second_byte_opcode(struct x86_instr * inst){
    if(inst == NULL){
        printf("%s: %s\n", __func__, "instruction is null");
        return;
    }

    inst->opcode[1] = inst->byte_code[inst->opcode_ptr + 1];
    
    if(inst->opcode[1] == 0x38 || inst->opcode[1] == 0x3a){
        return check_third_byte_opcode(inst);
    }

    switch(inst->opcode[1]){
    case(op_rdtsc):
        strcat(inst->x86_string, "rdtsc");
        return;
    default:
        printf("%s: %s %x\n", __func__, "second byte opcode invalid", inst->opcode[1]);
    }
    return;
}

// Assumes check_prefix has determined the correct opcode location
void check_opcode(struct x86_instr * inst){
    // TODO handle more than 1 byte of opcode
    unsigned char opcode = inst->opcode[0]; 
    if(opcode == 0xf){
        return check_second_byte_opcode(inst);
    }

    switch(opcode){
    case(op_call):
        inst->displacement_ptr = inst->opcode_ptr + 1;
        long long disp = get_displacement(inst, DEFAULT_BIT_MODE);

        // relative displacement from next instruction; this call instruction is 5 bytes long; next instruction 5 bytes from now
        disp = disp + 5; 
        sprintf(inst->displacement, "0x%llx", disp);

        strcat(inst->x86_string, "call ");
        strcat(inst->x86_string, inst->displacement);

        goto exit;
    case(op_lea):
        inst->modrm_ptr = inst->opcode_ptr + 1;
        inst->modrm = inst->byte_code[inst->modrm_ptr];

        if(inst->rex_ptr == -1)
            check_modrm(inst, 32); 
        else
            check_modrm(inst, 64);

        strcat(inst->x86_string, "lea ");
        strcat(inst->x86_string, inst->operands->operands[0]);
        strcat(inst->x86_string, ", ");
        strcat(inst->x86_string, inst->operands->operands[1]);

        goto exit;


    case(op_mov):
        inst->modrm_ptr = inst->opcode_ptr + 1;
        inst->modrm = inst->byte_code[inst->modrm_ptr];

        if(inst->rex_ptr == -1)
            check_modrm(inst, 32); 
        else
            check_modrm(inst, 64);

        strcat(inst->x86_string, "mov ");
        strcat(inst->x86_string, inst->operands->operands[1]);
        strcat(inst->x86_string, ", ");
        strcat(inst->x86_string, inst->operands->operands[0]);

        goto exit;

    case(op_mov_8b):
        inst->modrm_ptr = inst->opcode_ptr + 1;
        inst->modrm = inst->byte_code[inst->modrm_ptr];

        if(inst->rex_ptr == -1)
            check_modrm(inst, 32); 
        else
            check_modrm(inst, 64);

        strcat(inst->x86_string, "mov ");
        strcat(inst->x86_string, inst->operands->operands[0]);
        strcat(inst->x86_string, ", ");
        strcat(inst->x86_string, inst->operands->operands[1]);

        goto exit;

    case(op_or):
        inst->modrm_ptr = inst->opcode_ptr + 1;
        inst->modrm = inst->byte_code[inst->modrm_ptr];

        if(inst->rex_ptr == -1)
            check_modrm(inst, 32); 
        else
            check_modrm(inst, 64);

        strcat(inst->x86_string, "or ");
        strcat(inst->x86_string, inst->operands->operands[1]);
        strcat(inst->x86_string, ", ");
        strcat(inst->x86_string, inst->operands->operands[0]);

        goto exit;

    case(op_shl):
        inst->modrm_ptr = inst->opcode_ptr + 1;
        inst->modrm = inst->byte_code[inst->modrm_ptr];
        
        if(inst->rex_ptr == -1)
            check_modrm(inst, 32); 
        else
            check_modrm(inst, 64);

        inst->immediate_ptr = inst->modrm_ptr + 1;
        long long imm8 = get_immediate(inst, 8);
        sprintf(inst->immediate, "0x%llx", imm8);

        strcat(inst->x86_string, "shl ");
        strcat(inst->x86_string, inst->operands->operands[1]); 
        strcat(inst->x86_string, ", ");
        strcat(inst->x86_string, inst->immediate);

        goto exit;
        
    case(op_sub):   // 83 \5 (use r/m not reg of modrm for register) and ib (immediate_8 byte)
        inst->modrm_ptr = inst->opcode_ptr + 1;
        inst->modrm = inst->byte_code[inst->modrm_ptr];
        
        if(inst->rex_ptr == -1)
            check_modrm(inst, 32); 
        else
            check_modrm(inst, 64);

        inst->immediate_ptr = inst->modrm_ptr + 1;
        inst->immediate[0] = inst->byte_code[inst->immediate_ptr];
        unsigned char x = inst->immediate[0];
        char imm[5];
        sprintf(imm, "0x%x",x);

        strcat(inst->x86_string, "sub ");
        strcat(inst->x86_string, inst->operands->operands[1]); 
        strcat(inst->x86_string, ", ");
        strcat(inst->x86_string, imm);
        
        goto exit;
    case(op_sub_2b):
        inst->modrm_ptr = inst->opcode_ptr + 1;
        inst->modrm = inst->byte_code[inst->modrm_ptr];

        if(inst->rex_ptr == -1)
            check_modrm(inst, 32); 
        else
            check_modrm(inst, 64);

        strcat(inst->x86_string, "sub ");
        strcat(inst->x86_string, inst->operands->operands[0]);
        strcat(inst->x86_string, ", ");
        strcat(inst->x86_string, inst->operands->operands[1]);

        goto exit;

    }

    if(op_pop <= opcode &&  opcode < op_pop + 8){

        int bit_mode = check_bit_mode(inst);
        int index = opcode & 0x7;
        // Get extension
        index = index | (get_rex_b(inst) << 3);
        char * reg = get_register(index, 1, bit_mode); //TODO check for w bit?

        strcat(inst->x86_string, "pop ");
        strcat(inst->x86_string, reg);

        goto exit;
    }
    else if(op_push <= opcode && opcode < op_push + 8){

        int bit_mode = check_bit_mode(inst);
        int index = opcode & 0x7;
        index = index | (get_rex_b(inst) << 3);
        char * reg = get_register(index, 1, bit_mode); //TODO check for w bit?
        
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

void two_regs_ops(struct x86_instr * inst, const char * op){
    // mov, or
}
