#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "modrm.h"
#include "registers.h"
#include "immediates.h"
#include "prefix.h"
#include "sib.h"

static char * get_effective_addr_16(int index){
    char * strings[] = {"bx + si", "bx + di", "bp + si", "bp + di", "si", "di", "bp", "bx"};
    return strings[index];
}

void check_modrm_reg(unsigned char * inst, int operand_size, int address_size, int rex){
    int modrm = inst[0];
    int reg_num = (modrm & REG) >> 3;
    char * reg;

    if(rex != 0 && rex & REX_W){
        assert(operand_size == 64);
        // REX bits need to be in 4th position from right 
        u_int8_t rex_r = (rex & REX_R) << 1; 

        // Get extension bit from rex prefix
        reg_num = reg_num| rex_r;
    } 

    reg = get_register(reg_num, operand_size, rex);
}

// Should not be called when instruction has REX.W bit set
void modrm_rm_16(unsigned char * inst, int operand_size, int address_size){
    int modrm = inst[0];
    char * rm;
    u_int8_t mod = (modrm & MODRM) >> 6;
    u_int8_t modrm_rm = (modrm & RM);

    switch(mod){
        case 0:
            switch(modrm_rm){
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                    printf("memory access [%s]\n", get_effective_addr_16(modrm_rm));
                    break;
                case 6:
                    printf("disp 0x%lx\n", get_displacement(&inst[1], operand_size, 0));
                    break;
                case 7:
                    printf("memory access [%s]\n", get_effective_addr_16(modrm_rm));
                    break;
                default:
                    assert(0);
            }

            break;

        case 1:
            switch(modrm_rm){
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                    printf("memory access [%s", get_effective_addr_16(modrm_rm));
                    get_ones_comp_disp(&inst[1], 8, 0);
                    break;
                default:
                    assert(0);
            }

            break;

        case 2:
            switch(modrm_rm){
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                    printf("memory access [%s", get_effective_addr_16(modrm_rm));
                    get_ones_comp_disp(&inst[1], operand_size, 0);
                    break;
                default:
                    assert(0);
            }

            break;

        case 3:
            // both operands are registers
            rm = get_register(modrm_rm, operand_size, 0);
            break;

        default:
            assert(0);
    }

    return;
}

void check_modrm_rm(unsigned char * inst, int operand_size, int address_size, int rex){
    if(address_size == 16){
        assert(rex & REX_W == 0); 
        return modrm_rm_16(inst, operand_size, address_size);
    }

    int modrm = inst[0];
    char * rm;
    u_int8_t mod = (modrm & MODRM) >> 6;
    u_int8_t modrm_rm = (modrm & RM);

    if(rex & REX_W){
        assert(operand_size == 64);
    }

    // Get extension bit from rex prefix (even if REX.W not set, registers can still be extended in 64 bit mode
    u_int8_t rex_b = (rex & REX_B) << 3;
    modrm_rm = modrm_rm | rex_b;

    int modrm_rm_no_rex = modrm_rm & 0x7;
    switch(mod){
        case 0:
            switch(modrm_rm_no_rex){
                case 0:
                case 1:
                case 2:
                case 3:
                    printf("memory access [%s]\n", get_register(modrm_rm, operand_size, rex));
                    break;
                case 4:
                    printf("memory access [");
                    check_sib(&inst[1], operand_size, rex); 
                    printf("]");
                    break;
                case 5:
                    printf("memory access [rip");
                    get_ones_comp_disp(&inst[1], 32, 0);
                    break;
                case 6:
                case 7:
                    printf("memory access [%s]\n", get_register(modrm_rm, operand_size, rex));
                    break;
                default:
                    assert(0);
            }

            break;

        case 1:
            switch(modrm_rm_no_rex){
                case 0:
                case 1:
                case 2:
                case 3:
                    printf("memory access [%s", get_register(modrm_rm, operand_size, rex));
                    get_ones_comp_disp(&inst[1], 8, 0);
                    break;
                case 4:
                    printf("memory access [");
                    check_sib(&inst[1], operand_size, rex); 
                    get_ones_comp_disp(&inst[2], 8, 0);
                    break;
                case 5:
                case 6:
                case 7:
                    printf("memory access [%s", get_register(modrm_rm, operand_size, rex));
                    get_ones_comp_disp(&inst[1], 8, 0);
                    break;
                default:
                    assert(0);
            }

            break;

        case 2:
            switch(modrm_rm_no_rex){
                case 0:
                case 1:
                case 2:
                case 3:
                    printf("memory access [%s", get_register(modrm_rm, operand_size, rex));
                    get_ones_comp_disp(&inst[1], 32, 0);
                    break;
                case 4:
                    printf("memory access [");
                    check_sib(&inst[1], operand_size, rex); 
                    get_ones_comp_disp(&inst[2], 32, 0);
                    break;
                case 5:
                case 6:
                case 7:
                    printf("memory access [%s", get_register(modrm_rm, operand_size, rex));
                    get_ones_comp_disp(&inst[1], 32, 0);
                    break;
                default:
                    assert(0);
            }

            break;

        case 3:
            rm = get_register(modrm_rm, operand_size, rex);
            break;

        default:
            assert(0);
    }
    return;
}
