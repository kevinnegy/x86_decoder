#include <string.h>
#include <assert.h>
#include "modrm.h"
#include "registers.h"
#include "immediates.h"
#include "instruction.h"
#include "prefix.h"
#include "sib.h"

static char * get_effective_addr_16(int index){
    char * strings[] = {"bx + si", "bx + di", "bp + si", "bp + di", "si", "di", "bp", "bx"};
    return strings[index];
}

void check_modrm_rm_64(unsigned char * inst, int opcode_index){
    return;
}


// TODO handle mm(/r) MM0 and xmm(/r) XMM0 (check modrm table)
void check_modrm_inst_16(unsigned char *inst, int opcode_index){
    struct one_op_modrm * mold = (struct one_op_modrm *) &inst[opcode_index];
    int bit_mode = 16; 
    char * reg, * rm;
    u_int8_t modrm = (mold->modrm & MODRM) >> 6;
    u_int8_t modrm_reg = (mold->modrm & REG) >> 3;
    u_int8_t modrm_rm = (mold->modrm & RM);

    reg = get_register(modrm_reg, bit_mode);

    switch(modrm){
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
                    printf("disp 0x%ld\n", get_displacement(inst, opcode_index + 2, bit_mode, 0));
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
                    printf("disp 0x%ld\n", get_displacement(inst, opcode_index + 2, 8, 0));
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
                    printf("disp 0x%ld\n", get_displacement(inst, opcode_index + 2, bit_mode, 0));
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

void check_modrm_inst_32(unsigned char *inst, int opcode_index){
    struct one_op_modrm * mold = (struct one_op_modrm *) &inst[opcode_index];
    int bit_mode = 32; //TODO check w bit to determine what mode/ handle 16 bit case
    char * reg, * rm;
    u_int8_t modrm = (mold->modrm & MODRM) >> 6;
    u_int8_t modrm_reg = (mold->modrm & REG) >> 3;
    u_int8_t modrm_rm = (mold->modrm & RM);

    reg = get_register(modrm_reg, bit_mode);

    switch(modrm){
        case 0:
            switch(modrm_rm){
                case 0:
                case 1:
                case 2:
                case 3:
                    printf("memory access [%s]\n", get_register(modrm_rm, bit_mode));
                    break;
                case 4:
                    check_sib_32(inst, opcode_index); 
                    break;
                case 5:
                    printf("memory access [rip+0x%lx]\n", get_displacement(inst, opcode_index + 2, bit_mode, 0));
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
                    printf("0x%ld\n", get_displacement(inst, opcode_index + 2, 8, 0));
                    break;
                case 4:
                    check_sib_32(inst, opcode_index); 
                    printf("0x%ld\n", get_displacement(inst, opcode_index + 3, 8, 0));
                    break;
                case 5:
                case 6:
                case 7:
                    printf("memory access [%s] + ", get_register(modrm_rm, bit_mode));
                    printf("0x%ld\n", get_displacement(inst, opcode_index + 2, 8, 0));
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
                    printf("+ 0x%ld\n", get_displacement(inst, opcode_index + 2, bit_mode, 0));
                    break;
                case 4:
                    check_sib_32(inst, opcode_index); 
                    printf("0x%ld\n", get_displacement(inst, opcode_index + 3, bit_mode, 0));
                    break;
                case 5:
                case 6:
                case 7:
                    printf("memory access [%s]\n", get_register(modrm_rm, bit_mode));
                    printf("+ 0x%ld\n", get_displacement(inst, opcode_index + 2, bit_mode, 0));
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

// TODO assumes one byte opcode
void check_modrm_inst_64(unsigned char * inst, int rex_index){
    struct rex_one_op_modrm * mold = (struct rex_one_op_modrm *) &inst[rex_index];
    int bit_mode = 64;
    char * reg, *rm;
    u_int8_t rex_r = (mold->rex & REX_R) << 1; // Need the REX_R (3rd) bit shifted left to 4th position 
    u_int8_t rex_x = (mold->rex & REX_X) << 2;
    u_int8_t rex_b = (mold->rex & REX_B) << 3;
    u_int8_t modrm = (mold->modrm & MODRM) >> 6;
    u_int8_t modrm_reg = (mold->modrm & REG) >> 3;
    u_int8_t modrm_rm = (mold->modrm & RM);

    // Get extension bit from rex prefix
    u_int8_t reg_ext = modrm_reg | rex_r;
    u_int8_t rm_ext = modrm_rm | rex_b;

    reg = get_register(modrm_reg, bit_mode);

    switch(modrm){
        case 0:
            switch(modrm_rm){
                case 0:
                case 1:
                case 2:
                case 3:
                    printf("memory access [%s]\n", get_register(rm_ext, bit_mode));
                    break;
                case 4:
                    check_sib_64(inst, rex_index); 
                    break;
                case 5:
                    printf("memory access [rip+0x%lx]\n", get_displacement(inst, rex_index + 3, 32, 0));
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
                    printf("0x%ld\n", get_displacement(inst, rex_index + 3, 8, 0));
                    break;
                case 4:
                    check_sib_64(inst, rex_index); 
                    printf("0x%ld\n", get_displacement(inst, rex_index + 4, 8, 0));
                    break;
                case 5:
                case 6:
                case 7:
                    printf("memory access [%s] + ", get_register(rm_ext, bit_mode));
                    printf("0x%ld\n", get_displacement(inst, rex_index + 3, 8, 0));
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
                    printf("0x%ld\n", get_displacement(inst, rex_index + 3, 32, 0));
                    break;
                case 4:
                    check_sib_64(inst, rex_index); 
                    printf("0x%ld\n", get_displacement(inst, rex_index + 4, 8, 0));
                    break;
                case 5:
                case 6:
                case 7:
                    printf("memory access [%s] + ", get_register(rm_ext, bit_mode));
                    printf("0x%ld\n", get_displacement(inst, rex_index + 3, 32, 0));
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

