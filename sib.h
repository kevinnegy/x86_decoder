#ifndef SIB_H
#define SIB_H


// [scale * index] + [base]
#define SIB_SS    0xc0 // scale
#define SIB_INDEX 0x38 // index 
#define SIB_BASE  0x07 // base 

void check_sib_32(unsigned char * inst);
void check_sib_64(unsigned char * inst, int rex); 

#endif 
