#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "instruction.h"
#include "prefix.h"
#include "opcode.h"

void decode_x86_inst(unsigned char * inst){
    assert(inst != NULL); 
    check_prefix(inst);
    return;
}

void set_bit_mode(int mode){
    switch(mode){
        case 16:
            DEFAULT_BIT_MODE = 16;
            break;
        case 32:
            DEFAULT_BIT_MODE = 32;
            break;
        case 64:
            DEFAULT_BIT_MODE = 64;
            break;
        default:
            fprintf(stderr, "%s: %s %d\n", __func__, "invalid bit mode", mode);
            fprintf(stderr, "Setting default to 64\n");
            DEFAULT_BIT_MODE = 64;
    }
    return;
}







