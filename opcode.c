#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "opcode.h"
#include "instruction.h"
#include "immediates.h"
#include "modrm.h"

void check_third_opcode(unsigned char * inst, int rex){
    assert(inst != NULL);
    return;
}


void check_second_opcode(unsigned char * inst, int rex){
    assert(inst != NULL);

    int opcode = inst[0];
    
    if(opcode == 0x38 || opcode == 0x3a){
        check_third_opcode(&inst[1], rex);
        return;
    }

    switch(opcode){
        case OP_JZJE_84: //TODO are tttn encodings necessary?
        {
            int64_t disp = get_displacement(&inst[1], DEFAULT_BIT_MODE, 0);

            if(DEFAULT_BIT_MODE == 16)
                disp = disp + 4; 
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

// TODO handle 16 bit cases
void check_opcode(unsigned char * inst, int rex){
    assert(inst != NULL);
    u_int8_t opcode = inst[0]; 

    if(opcode == 0xf){
        check_second_opcode(&inst[1], rex);
        return;
    }

    switch(opcode){
        // TODO verify that all these instructions handle rex byte correctly
        case OP_MOV_8C:
        case OP_MOV_8E:
            // TODO handle sregister (see opcode.h)
            return;
        case OP_MOV_88:
        case OP_MOV_8A:
            // TODO if REX, registers cannot be AH, BH, CH, DH
            check_modrm_inst_16(&inst[1], 8);
            return; 
        case OP_ADD_01:
        case OP_LEA:
        case OP_MOV_89:
        case OP_MOV_8B:
        case OP_OR:
        case OP_SUB_2B:
        case OP_TEST_85:
            if(rex != 0)
                check_modrm_inst_64(&inst[1], rex);
            else
                check_modrm_inst_32(&inst[1]);
            return;
        case OP_CALL_E8:
            printf("disp 0x%lx\n", get_displacement(&inst[1], DEFAULT_BIT_MODE, 5));
            return;
        case OP_JMP_EB:
            printf("disp 0x%lx\n", get_displacement(&inst[1], 8, 2));
            return;
        case OP_SHL:
        case OP_SUB_83:   // 83 \5 (use r/m not reg of modrm for register) and ib (immediate_8 byte)
            // TODO does this need an immediate call here or will modrm take care of it?
            if(rex != 0)
                check_modrm_rm_64(&inst[1], rex); 
            else
                check_modrm_rm_32(&inst[1]); 
            return;
    }

    // Opcodes whose last 3 bits are for one register
    opcode = opcode & 0xf8;
    switch(opcode){
        case OP_POP:
        case OP_PUSH:
            if(DEFAULT_BIT_MODE == 64)
                printf("memory access [RSP]\n"); 
            else if(DEFAULT_BIT_MODE == 32)
                printf("memory access [ESP]\n"); 
            else if(DEFAULT_BIT_MODE == 16)
                printf("memory access [SP]\n"); 
                
            return;
        default:
            assert(0);
            return;
    }
    return;
}
