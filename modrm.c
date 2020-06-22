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
            char * reg_1 = get_register(op_1, w, 64);
            char * reg_2 = get_register(op_2, w, 64);
            strcat(inst->x86_string, reg_2); // TODO verify order and should this go here or return registers and let opcode function handle string building? probably latter
            strcat(inst->x86_string, ", ");
            strcat(inst->x86_string, reg_1);
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
        case(3):
            // TODO Get extended bits from REX prefix
            strcat(inst->x86_string, get_register(op_2, w, 32)); // TODO verify order and should this go here or return registers and let opcode function handle string building? probably latter
            strcat(inst->x86_string, ", ");
            strcat(inst->x86_string, get_register(op_1, w, 32));

    }
    return;
}

static void check_modrm_16(struct x86_instr * inst){
    unsigned char modrm = inst->modrm;
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

