#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_INSTR_LEN 15 
#define BYTE 8 
int DEFAULT_BIT_MODE;

// intel syntax has destination first like mips
void set_bit_mode(int mode);
void decode_x86_inst(unsigned char * inst);

#endif
