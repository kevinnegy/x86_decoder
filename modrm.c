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

void check_modrm_rm_64(unsigned char * inst, int rex){
    return;
}

void check_modrm_rm_32(unsigned char * inst){
    return;
}

// TODO handle mm(/r) MM0 and xmm(/r) XMM0 (check modrm table)
void check_modrm_inst_16(unsigned char *inst, int bit_mode){
    int modrm = inst[0];
    char * reg, * rm;
    u_int8_t mod = (modrm & MODRM) >> 6;
    u_int8_t modrm_reg = (modrm & REG) >> 3;
    u_int8_t modrm_rm = (modrm & RM);

    reg = get_register(modrm_reg, bit_mode);

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
                    printf("disp 0x%ld\n", get_displacement(&inst[1], bit_mode, 0));
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
                    printf("memory access [%s] +", get_effective_addr_16(modrm_rm));
                    printf("disp 0x%ld\n", get_displacement(&inst[1], 8, 0));
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
                    printf("memory access [%s] +", get_effective_addr_16(modrm_rm));
                    printf("disp 0x%ld\n", get_displacement(&inst[1], bit_mode, 0));
                    break;
                default:
                    assert(0);
            }

            break;

        case 3:
            // both operands are registers
            rm = get_register(modrm_rm, bit_mode);
            break;

        default:
            assert(0);
    }

    return;
}

void check_modrm_inst_32(unsigned char *inst){
    int modrm = inst[0];
    int bit_mode = 32; 
    char * reg, * rm;
    u_int8_t mod = (modrm & MODRM) >> 6;
    u_int8_t modrm_reg = (modrm & REG) >> 3;
    u_int8_t modrm_rm = (modrm & RM);

    reg = get_register(modrm_reg, bit_mode);

    switch(mod){
        case 0:
            switch(modrm_rm){
                case 0:
                case 1:
                case 2:
                case 3:
                    printf("memory access [%s]\n", get_register(modrm_rm, bit_mode));
                    break;
                case 4:
                    check_sib_32(&inst[1]); 
                    break;
                case 5:
                    printf("memory access [rip+0x%lx]\n", get_displacement(&inst[1], bit_mode, 0));
                    break;
                case 6:
                case 7:
                    printf("memory access [%s]\n", get_register(modrm_rm, bit_mode));
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
                    printf("memory access [%s] + ", get_register(modrm_rm, bit_mode));
                    printf("0x%ld\n", get_displacement(&inst[1], 8, 0));
                    break;
                case 4:
                    check_sib_32(&inst[1]); 
                    printf("0x%ld\n", get_displacement(&inst[1], 8, 0));
                    break;
                case 5:
                case 6:
                case 7:
                    printf("memory access [%s] + ", get_register(modrm_rm, bit_mode));
                    printf("0x%ld\n", get_displacement(&inst[1], 8, 0));
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
                    printf("memory access [%s]\n", get_register(modrm_rm, bit_mode));
                    printf("+ 0x%ld\n", get_displacement(&inst[1], bit_mode, 0));
                    break;
                case 4:
                    check_sib_32(&inst[1]); 
                    printf("0x%ld\n", get_displacement(&inst[2], bit_mode, 0));
                    break;
                case 5:
                case 6:
                case 7:
                    printf("memory access [%s]\n", get_register(modrm_rm, bit_mode));
                    printf("+ 0x%ld\n", get_displacement(&inst[1], bit_mode, 0));
                    break;
                default:
                    assert(0);
            }

            break;

        case 3:
            rm = get_register(modrm_rm, bit_mode);
            break;

        default:
            assert(0);
    }
    return;
}

void check_modrm_inst_64(unsigned char * inst, int rex){
    int modrm = inst[0];
    int bit_mode = 64;
    char * reg, *rm;
    
    // REX bits need to be in 4th position from right 
    u_int8_t rex_r = (rex & REX_R) << 1; 
    u_int8_t rex_x = (rex & REX_X) << 2;
    u_int8_t rex_b = (rex & REX_B) << 3;

    u_int8_t mod = (modrm & MODRM) >> 6;
    u_int8_t modrm_reg = (modrm & REG) >> 3;
    u_int8_t modrm_rm = (modrm & RM);

    // Get extension bit from rex prefix
    u_int8_t reg_ext = modrm_reg | rex_r;
    u_int8_t rm_ext = modrm_rm | rex_b;

    reg = get_register(modrm_reg, bit_mode);

    switch(mod){
        case 0:
            switch(modrm_rm){
                case 0:
                case 1:
                case 2:
                case 3:
                    printf("memory access [%s]\n", get_register(rm_ext, bit_mode));
                    break;
                case 4:
                    check_sib_64(&inst[1], rex); 
                    break;
                case 5:
                    printf("memory access [rip+0x%lx]\n", get_displacement(&inst[1], 32, 0));
                    break;
                case 6:
                case 7:
                    printf("memory access [%s]\n", get_register(rm_ext, bit_mode));
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
                    printf("memory access [%s] + ", get_register(rm_ext, bit_mode));
                    printf("0x%ld\n", get_displacement(&inst[1], 8, 0));
                    break;
                case 4:
                    check_sib_64(&inst[1], rex); 
                    printf("0x%ld\n", get_displacement(&inst[2], 8, 0));
                    break;
                case 5:
                case 6:
                case 7:
                    printf("memory access [%s] + ", get_register(rm_ext, bit_mode));
                    printf("0x%ld\n", get_displacement(&inst[1], 8, 0));
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
                    printf("memory access [%s] + ", get_register(rm_ext, bit_mode));
                    printf("0x%ld\n", get_displacement(&inst[1], 32, 0));
                    break;
                case 4:
                    check_sib_64(&inst[1], rex); 
                    printf("0x%ld\n", get_displacement(&inst[2], 8, 0));
                    break;
                case 5:
                case 6:
                case 7:
                    printf("memory access [%s] + ", get_register(rm_ext, bit_mode));
                    printf("0x%ld\n", get_displacement(&inst[1], 32, 0));
                    break;
                default:
                    assert(0);
            }

            break;

        case 3:
            rm = get_register(rm_ext, bit_mode);
            break;
        default:
            assert(0);
}
return;
}

