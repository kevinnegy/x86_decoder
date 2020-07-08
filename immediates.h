#ifndef IMMEDIATES_H
#define IMMEDIATES_H

#include <stdlib.h>

int64_t get_displacement(unsigned char * inst, int disp_len, int instr_len);
unsigned long long get_immediate(unsigned char * inst, int imm_len);
void get_ones_comp_disp(unsigned char *inst, int disp_len, int instr_len);

#endif
