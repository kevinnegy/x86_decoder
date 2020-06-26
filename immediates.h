#ifndef IMMEDIATES_H
#define IMMEDIATES_H

#include "instruction.h"

unsigned long long get_displacement(unsigned char * inst, int op_byte_num, int disp_len, int instr_len);
unsigned long long get_immediate(unsigned char * inst, int op_byte_num, int imm_len);

#endif
