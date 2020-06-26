#ifndef X86_DECODER_H
#define x86_DECODER_H

#include "instruction.h"
#include "decode.h"

void disassemble_x86_instr(char * bytes, int mode){
    if(bytes == NULL){
        fprintf(stderr, "%s: Instruction bytes array is null.", __func__);
        return;
    }

    set_bit_mode(mode);
    decode(bytes);
    return;
}




#endif
