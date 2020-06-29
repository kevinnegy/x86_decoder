#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "opcode.h"
#include "instruction.h"
#include "immediates.h"
#include "modrm.h"

void check_third_opcode(unsigned char * inst, int rex_index){
    return;
}


void check_second_opcode(unsigned char * inst, int opcode_index){
    assert(inst != NULL);

    int opcode = inst[opcode_index + 1];
    
    if(opcode == 0x38 || opcode == 0x3a){
        return check_third_opcode(inst, opcode_index);
    }

    switch(opcode){
        case OP_JZJE_84: //TODO are tttn encodings necessary?
        {
            int64_t disp = get_displacement(inst, opcode_index + 2, DEFAULT_BIT_MODE, 0);

            if(DEFAULT_BIT_MODE == 16)
                disp = disp + 4; //TODO test
            else
                disp = disp + 6; 
            printf("disp 0x%lx\n", disp);

            return;
        }
        case OP_RDTSC:
            return;

        default:
            assert(0);
    }
    return;
}

void check_opcode(unsigned char * inst, int opcode_index){
    u_int8_t opcode = inst[opcode_index]; 

    if(opcode == 0xf){
        return check_second_opcode(inst, opcode_index);
    }

    switch(opcode){
        case OP_ADD_01:
            check_modrm_inst_32(inst, opcode_index);
            return;
        case OP_CALL_E8:
            printf("disp 0x%lx\n", get_displacement(inst, opcode_index + 1, DEFAULT_BIT_MODE, 5));
            return;
        case OP_JMP_EB:
            printf("disp 0x%lx\n", get_displacement(inst, opcode_index + 1, 8, 2));
            return;
        case OP_MOV:
            check_modrm_inst_32(inst, opcode_index);
            return;
        case OP_SHL:
            check_modrm_rm_64(inst, opcode_index); // TODO not 64 bit
            return;
    }

    // Opcodes whose last 3 bits are for one register
    opcode = opcode & 0xf8;
    switch(opcode){
        case OP_MOV_B8:
            return;
        case OP_POP:
            // TODO Reads from stack pointer location
        case OP_PUSH:
            // TODO Writes to stack pointer location
            return;
        default:
            printf("%s: %s %x\n", __func__, "invalid opcode", opcode);
            return;
    }
    return;
}

void check_third_opcode_rex(unsigned char * inst, int rex_index){
    return;
}

void check_second_opcode_rex(unsigned char * inst, int rex_index){
    assert(inst != NULL);

    int opcode = inst[rex_index + 2];
    
    if(opcode == 0x38 || opcode == 0x3a){
        return check_third_opcode_rex(inst, rex_index);
    }

    switch(opcode){
        default:
            assert(0);
    }
    return;
}

void check_opcode_rex(unsigned char * inst, int rex_index){
    u_int8_t opcode = inst[rex_index + 1]; 

    if(opcode == 0xf){
        return check_second_opcode_rex(inst, rex_index);
    }

    switch(opcode){
        case OP_ADD_01:
        case OP_LEA:
        case OP_MOV:
        case OP_MOV_8B:
        case OP_OR:
            check_modrm_inst_64(inst, rex_index);
            return;

        case OP_SHL:
        case OP_SUB_83:   // 83 \5 (use r/m not reg of modrm for register) and ib (immediate_8 byte)
            check_modrm_rm_64(inst, rex_index); 
            return;
        case OP_SUB_2B:
        case OP_TEST_85:
            check_modrm_inst_64(inst, rex_index);
            return;
    }

    // Opcodes whose last 3 bits are for one register
    opcode = opcode & 0xf8;
    switch(opcode){
        case OP_MOV_B8:
        case OP_POP: // TODO Reads from stack pointer location
        case OP_PUSH: // TODO Writes to stack pointer location
            return;

        default:
            printf("%s: %s %x\n", __func__, "invalid opcode", opcode);
            return;
    }
    return;
}
