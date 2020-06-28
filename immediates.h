#ifndef IMMEDIATES_H
#define IMMEDIATES_H

#include <stdlib.h>

int64_t get_displacement(unsigned char * inst, int op_byte_num, int disp_len, int instr_len);
unsigned long long get_immediate(unsigned char * inst, int op_byte_num, int imm_len);

#endif
