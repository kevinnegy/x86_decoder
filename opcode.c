#include "opcode.h"
#include "decoder.h"
#include <stdio.h>
#include <string.h>


//void check_third_byte_opcode(unsigned char * inst){;
//}
//
void check_second_byte_opcode(unsigned char * inst){
//    if(inst == NULL){
//        printf("%s: %s\n", __func__, "instruction is null");
//        return;
//    }
//
//    inst->opcode[1] = inst->byte_code[inst->opcode_ptr + 1];
//    
//    if(inst->opcode[1] == 0x38 || inst->opcode[1] == 0x3a){
//        return check_third_byte_opcode(inst);
//    }
//
//    switch(inst->opcode[1]){
//    case OP_JZJE_84: //TODO are tttn encodings necessary?
//        inst->displacement_ptr = inst->opcode_ptr + 2;
//        long long disp = get_displacement(inst, DEFAULT_BIT_MODE);
//
//        // relative displacement from next instruction; this instruction is 2 bytes + disp long;
//        if(DEFAULT_BIT_MODE == 16)
//            disp = disp + 4; //TODO test
//        else
//            disp = disp + 6; 
//
//        sprintf(inst->displacement, "0x%llx", disp);
//
//        strcat(inst->x86_string, "jz "); 
//        strcat(inst->x86_string, inst->displacement);
//
//        return;
//
//    case OP_RDTSC:
//        strcat(inst->x86_string, "rdtsc");
//        return;
//
//    default:
//        printf("%s: %s %x\n", __func__, "second byte opcode invalid", inst->opcode[1]);
//    }
//    return;
}

void check_modrm_reg(){;}
void check_modrm_rm(){;}
void check_rex_modrm_reg(){;}
void check_rex_modrm_rm(){;}

void check_modrm_inst(char *inst, int opcode){
    struct one_modrm * mold = (struct one_modrm *) &inst[opcode];
    int bit_mode = 32; //TODO check w bit to determine what mode/ handle 16 bit case
    switch(mold->modrm){
        case 0:
            break;

        case 1:
            break;

        case 2:
            break;

        case 3:
            // both operands are registers
            break;
    }
 
}

void check_rex_modrm_inst(char * inst, int rex_index){
    struct rex_one_modrm * mold = (struct rex_one_modrm *) &inst[rex_index];

    switch(mold->modrm){
        case 0:
            break;

        case 1:
            break;

        case 2:
            break;

        case 3:
            // both operands are registers
            break;
    }
    return;
}
unsigned long long extend_sign_64(u_int32_t num){
    unsigned long long sign_32 = 0x1 & (num >> 31);

    if(sign_32 == 0) // positive sign extend 
        return (sign_32 << 63) + (num & 0x7fffffff);

    return 0xffffffff00000000 + num; // negative sign extend
}

// Largest possible return is 64 bits. Each instruction can typecast back
unsigned long long get_displacement(unsigned char * inst, int opcode, int disp_len, int instr_len){
    struct one_disp * mold = (struct one_disp *) inst;
    switch(disp_len){
        case 8:
            return mold->disp8 + instr_len;
        case 16:
            return mold->disp16 + instr_len;
        case 32:
            return mold->disp32 + instr_len;
        case 64:
            return extend_sign_64(mold->disp32) + instr_len;
    }
}

void check_inst(unsigned char * inst, int byte_num){
    u_int8_t opcode = inst[byte_num]; 

     // TODO handle more than 1 byte of opcode
    if(opcode == 0xf){
        return check_second_byte_opcode(inst);
    }

    switch(opcode){
    case OP_ADD_01:
        check_modrm_inst(inst, opcode);
        return;
    case OP_CALL_E8:
        printf("disp 0x%llx\n", get_displacement(inst, opcode, DEFAULT_BIT_MODE, 5));
        return;
    case OP_JMP_EB:
        printf("disp 0x%llx\n", get_displacement(inst, opcode, 8, 2));
        return;
    case OP_SHL:
        check_modrm_rm(inst, opcode);
        return;
    }
}

void check_rex_inst(unsigned char * inst, int rex_index){
    u_int8_t opcode = inst[rex_index + 1]; 

    // TODO handle more than 1 byte of opcode
    if(opcode == 0xf){
        return check_second_byte_opcode(inst);
    }

    switch(opcode){
        case OP_ADD_01:
        case OP_LEA:
        case OP_MOV:
        case OP_MOV_8B:
        case OP_OR:
            check_rex_modrm_inst(inst, rex_index);
            return;

        case OP_SHL:
        case OP_SUB_83:   // 83 \5 (use r/m not reg of modrm for register) and ib (immediate_8 byte)
            check_rex_modrm_rm(inst, rex_index);
            return;
        case OP_SUB_2B:
        case OP_TEST_85:
            check_rex_modrm_inst(inst, rex_index);
            return;
    }

    if(OP_MOV_B8 <= opcode && opcode < OP_MOV_B8 + 8){
//        // Get register from opcode
//        int bit_mode = check_bit_mode(inst);
//        if(inst->rex_ptr == -1 || !(inst->rex & REX_W))
//            bit_mode = 32; //TODO check w bit to determine what mode/ handle 16 bit case
//
//        int index = opcode & 0x7;
//        
//        // Get extension
//        index = index | (get_rex_b(inst) << 3);
//        char * reg = get_register(index, 1, bit_mode); //TODO check for w bit?
//
//        inst->immediate_ptr = inst->opcode_ptr + 1;
//        unsigned long long imm = get_immediate(inst, bit_mode); // check bit mode 16, 32, 8
//        sprintf(inst->immediate, "0x%llx", imm);
//
//        strcat(inst->x86_string, "mov ");
//        strcat(inst->x86_string, reg);
//        strcat(inst->x86_string, ", ");
//        strcat(inst->x86_string, inst->immediate);
        return;
    } 
    else if(OP_POP <= opcode &&  opcode < OP_POP + 8){
//
//        int bit_mode = check_bit_mode(inst);
//        int index = opcode & 0x7;
//        // Get extension
//        index = index | (get_rex_b(inst) << 3);
//        char * reg = get_register(index, 1, bit_mode); //TODO check for w bit?
//
//        strcat(inst->x86_string, "pop ");
//        strcat(inst->x86_string, reg);
//
        return;
    }
    else if(OP_PUSH <= opcode && opcode < OP_PUSH + 8){
//
//        int bit_mode = check_bit_mode(inst);
//        int index = opcode & 0x7;
//        index = index | (get_rex_b(inst) << 3);
//        char * reg = get_register(index, 1, bit_mode); //TODO check for w bit?
//        
//        strcat(inst->x86_string, "push ");
//        strcat(inst->x86_string, reg);
        return;
    }
    switch(opcode){
    default:
        printf("%s: %s %x\n", __func__, "invalid opcode", opcode);
        return;
    }
    return;
}

void one_operand_op(unsigned char * inst){}
