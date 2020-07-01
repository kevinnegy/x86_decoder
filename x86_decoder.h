#ifndef X86_DECODER_H
#define x86_DECODER_H

#define MAX_INSTR_LEN 15 
#define BYTE 8 

int DEFAULT_BIT_MODE;

// intel syntax has destination first like mips
void set_bit_mode(int mode);
void decode_x86_inst(unsigned char * inst, int mode);

#endif
