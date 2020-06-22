#include "modrm.h"
#include "registers.h"
#include "prefix.h"
#include <string.h>

static void check_modrm_64(struct x86_instr * inst){
    unsigned char modrm = inst->modrm;
    int op_1 = (modrm >> 3) & 0x7;
    int op_2 = modrm & 0x7;
    int w = 1; // TODO figure out if w bit exists
    switch(modrm >> 6){
        case(0):
            // 0
        case(1):
            // 1
        case(2):
            // 2
        case(3): // No SIB, no REX.X bit
        {
            if(get_rex_x(inst) == 1){
                printf("%s: %s\n", __func__, "Mod = 11 but there is a REX.X bit set meaning SIB exists");
                return; 
            }
            op_1 = op_1 | (get_rex_r(inst) << 3); // Get extension bit from rex prefix
            op_2 = op_2 | (get_rex_b(inst) << 3); // Get extension bit from rex prefix
             
            // TODO Get extended bits from REX prefix
            inst->operands->num_operands = 2;
            inst->operands->operands = (char **) malloc (sizeof(char *) * 2);
            if(inst->operands->operands == NULL){
                printf("%s: %s\n", __func__, "malloc returned null");
                return;
            }
            inst->operands->operands[0] = get_register(op_1, w, 64);
            inst->operands->operands[1] = get_register(op_2, w, 64);
        }

    }
    return;
}
static void check_modrm_32(struct x86_instr * inst){
    unsigned char modrm = inst->modrm;
    int op_1 = (modrm >> 3) & 0x7;
    int op_2 = modrm & 0x7;
    int w = 1; // TODO figure out if w bit exists
    switch(modrm >> 6){
        case(0):
            // 0
        case(1):
            // 1
        case(2):
            // 2
        case(3): // No SIB, no REX.X bit
        {
            if(get_rex_x(inst) == 1){
                printf("%s: %s\n", __func__, "Mod = 11 but there is a REX.X bit set meaning SIB exists");
                return; 
            }
            op_1 = op_1 | (get_rex_r(inst) << 3); // Get extension bit from rex prefix
            op_2 = op_2 | (get_rex_b(inst) << 3); // Get extension bit from rex prefix
             
            // TODO Get extended bits from REX prefix
            inst->operands->num_operands = 2;
            inst->operands->operands = (char **) malloc (sizeof(char *) * 2);
            inst->operands->operands[0] = get_register(op_1, w, 32);
            inst->operands->operands[1] = get_register(op_2, w, 32);
        }

    }
    return;
}

static void check_modrm_16(struct x86_instr * inst){
    unsigned char modrm = inst->modrm;
    int op_1 = (modrm >> 3) & 0x7;
    int op_2 = modrm & 0x7;
    int w = 1; // TODO figure out if w bit exists
    switch(modrm >> 6){
        case(0):
            // 0
        case(1):
            // 1
        case(2):
            // 2
        case(3): // No SIB, no REX.X bit
        {
            if(get_rex_x(inst) == 1){
                printf("%s: %s\n", __func__, "Mod = 11 but there is a REX.X bit set meaning SIB exists");
                return; 
            }
            op_1 = op_1 | (get_rex_r(inst) << 3); // Get extension bit from rex prefix
            op_2 = op_2 | (get_rex_b(inst) << 3); // Get extension bit from rex prefix
             
            // TODO Get extended bits from REX prefix
            inst->operands->num_operands = 2;
            inst->operands->operands = (char **) malloc (sizeof(char *) * 2);
            inst->operands->operands[0] = get_register(op_1, w, 64);
            inst->operands->operands[1] = get_register(op_2, w, 64);
        }

    }
    return;
}

void check_modrm(struct x86_instr * inst, int mode){
    switch(mode){
        case(16):
            check_modrm_16(inst);
            break;
        case(32):
            check_modrm_32(inst);
            break;
        case(64):
            check_modrm_64(inst);
            break;
        default:
            printf("%s: %s %d\n", __func__, "invalid mode ", mode);
    }
    return;
}

