#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "instruction.h"
#include "prefix.h"
#include "opcode.h"

u_int8_t get_byte(unsigned char * inst, int byte){
    return (u_int8_t) inst[byte];
}

void decode_x86_inst(unsigned char * inst){
    assert(inst != NULL); 

    u_int8_t byte;
    int rex_exists, prefix_exists = 1, byte_num = -1;
    
    while(prefix_exists){
        byte_num++;
        byte = get_byte(inst, byte_num);
        prefix_exists = check_prefix(byte);
    }

    rex_exists = check_rex(byte);
    if(rex_exists)
        check_opcode_rex(inst, byte_num);
    else
        check_opcode(inst, byte_num);
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







