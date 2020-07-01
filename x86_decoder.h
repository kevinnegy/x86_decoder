#ifndef X86_DECODER_H
#define x86_DECODER_H

#include <stdio.h>
#include "instruction.h"
#include "prefix.h"

void decode_x86_inst(unsigned char * inst, int mode){
    if(inst == NULL){
        fprintf(stderr, "%s: Instruction bytes array is null.", __func__);
        return;
    }

    set_bit_mode(mode);
    check_prefix(inst);
    return;
}

#endif
