#ifndef SIB_H
#define SIB_H

#include "prefix.h"

// [scale * index] + [base]
#define SIB_SS    0xc0 // scale
#define SIB_INDEX 0x38 // index 
#define SIB_BASE  0x07 // base 

void check_sib(unsigned char * inst, int operand_size, struct prefixes * prfx); 

#endif 
