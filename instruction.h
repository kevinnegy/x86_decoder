#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <stdio.h>
#include <stdlib.h>
#define MAX_INSTR_LEN 15 
int DEFAULT_BIT_MODE;

// intel syntax has destination first like mips
// bytes should be stored as unsigned char, 2 digit hexadecimals

void set_bit_mode(int mode);

#endif
