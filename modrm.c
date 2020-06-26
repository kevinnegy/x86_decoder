#include <string.h>
#include "modrm.h"
#include "registers.h"
#include "immediates.h"
#include "instruction.h"

void check_modrm_reg(unsigned char * inst, int opcode_index){}
void check_modrm_rm(unsigned char * inst, int opcode_index){}
void check_rex_modrm_reg(unsigned char * inst, int rex_index){}
void check_rex_modrm_rm(unsigned char * inst, int rex_index){}

void check_modrm_inst(unsigned char *inst, int opcode_index){
    struct one_modrm * mold = (struct one_modrm *) &inst[opcode_index];
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

void check_rex_modrm_inst(unsigned char * inst, int rex_index){
    struct rex_one_modrm * mold = (struct rex_one_modrm *) &inst[rex_index];
    int reg = mold->reg;
    int rm = mold->rm;

    switch(mold->modrm){
        case 0:
            rm = rm | ((int)mold->rex_b << 3); // Get extension bit from rex prefix

            // Second operand
            if(rm == 4){
                printf("%s: %s\n", __func__, "handle sib byte here"); 
                return;
            }

            if(rm == 5){
                // TODO handle negative/64 bit
                // in 64 bit, this is [RIP + disp32]                
                unsigned long long disp = get_displacement(inst, rex_index + 1, 32, 0);

                printf("memory access [rip+0x%llx]\n", disp);
                return;
            }
            
            printf("memory access [%s]", get_register(rm, 64));
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

