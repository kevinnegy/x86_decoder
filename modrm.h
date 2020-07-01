#ifndef MODRM_H
#define MODRM_H

#define MODRM 0xc0
#define REG   0x38
#define RM    0x07

void check_modrm_inst_16(unsigned char * inst);
void check_modrm_inst_32(unsigned char * inst);
void check_modrm_inst_64(unsigned char * inst, int rex);
void check_modrm_rm_64(unsigned char * inst, int rex);
void check_modrm_rm_32(unsigned char * inst);

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
