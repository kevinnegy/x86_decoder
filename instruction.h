#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_INSTR_LEN 15 
#define BYTE 8 

int DEFAULT_BIT_MODE;

/*** One byte opcode ***/
struct rex_one_op_modrm{
    u_int8_t rex_prefix: 4;
    u_int8_t rex_w: 1;
    u_int8_t rex_r: 1;
    u_int8_t rex_x: 1;
    u_int8_t rex_b: 1;
    u_int8_t opcode: 8;
    u_int8_t modrm: 2;
    u_int8_t reg: 3;
    u_int8_t rm: 3;
};

struct one_op_modrm{
    u_int8_t opcode: 8;
    u_int8_t modrm: 2;
    u_int8_t reg: 3;
    u_int8_t rm: 3;
};

struct one_op_disp_imm{
    u_int8_t opcode: 8;
    union{
        unsigned char disp8[1];
        unsigned char disp16[2];
        unsigned char disp32[4];
        unsigned char imm8[1];
        unsigned char imm16[2];
        unsigned char imm32[4];
    };
};

/*** Two byte opcode ***/
struct rex_two_op_modrm{
    u_int8_t rex_prefix: 4;
    u_int8_t rex_w: 1;
    u_int8_t rex_r: 1;
    u_int8_t rex_x: 1;
    u_int8_t rex_b: 1;
    u_int8_t opcode_1: 8;
    u_int8_t opcode_2: 8;
    u_int8_t modrm: 2;
    u_int8_t reg: 3;
    u_int8_t rm: 3;
};

struct two_op_modrm{
    u_int8_t opcode_1: 8;
    u_int8_t opcode_2: 8;
    u_int8_t modrm: 2;
    u_int8_t reg: 3;
    u_int8_t rm: 3;
};

struct two_op_disp_imm{
    u_int8_t opcode_1: 8;
    u_int8_t opcode_2: 8;
    union{
        unsigned char disp8[1];
        unsigned char disp16[2];
        unsigned char disp32[4];
        unsigned char imm8[1];
        unsigned char imm16[2];
        unsigned char imm32[4];
    };
};

// intel syntax has destination first like mips
void set_bit_mode(int mode);
void decode_x86_inst(unsigned char * inst);

#endif
