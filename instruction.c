#include <stdio.h>
#include <stdlib.h>
#include "instruction.h"

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







