#ifndef MODRM_H
#define MODRM_H

void check_modrm_inst(unsigned char * inst, int opcode_index);
void check_rex_modrm_inst(unsigned char * inst, int rex_index);
void check_modrm_reg(unsigned char * inst, int opcode_index);
void check_modrm_rm(unsigned char * inst, int opcode_index);
void check_rex_modrm_reg(unsigned char * inst, int rex_index);
void check_rex_modrm_rm(unsigned char * inst, int rex_index);

/** ModR/M **/
// ModR/M byte - addressing mode byte:
    // mod first two bits - something
    // reg/opcode 3bits - a register or further opcode
    // r/m 3 bits - another register or further opcode

// some ModR/M bytes require a second byte SIB 
// Scaled index byte:
    // scale
    // index - index register
    // base - base register


#endif
