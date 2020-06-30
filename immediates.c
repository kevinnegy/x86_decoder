#include <stdio.h>
#include <assert.h>
#include "immediates.h"
#include "instruction.h"

// Use signed ints because displacement can be positive or negative in relative addressing
int32_t calc_displacement(unsigned char * displacement, int num_bytes){
    assert(num_bytes <= 4); // Displacement can be no more than 4 bytes

    int i = 0;
    int32_t disp = 0;
    for(i = 0; i< num_bytes; i++){
        unsigned char byte = displacement[i];
        disp = disp + ((int32_t)byte << (i*8));  // little endian!
    } 

    return disp; 
}

// Largest possible return is 64 bits. Each instruction can typecast back
int64_t get_displacement(unsigned char * inst, int disp_len, int instr_len){

    switch(disp_len){
        case 8:
            if(DEFAULT_BIT_MODE == 16)
                return (int16_t)(calc_displacement(inst, 1)) + instr_len;
            return (int32_t)(calc_displacement(inst, 1)) + instr_len;
        case 16:
            return calc_displacement(inst, 2) + instr_len;
        case 32:
            return calc_displacement(inst, 4) + instr_len;
        case 64:
            return (int64_t)(calc_displacement(inst, 4)) + instr_len; // Sign extend 32 to 64 bits
        default:
            assert(0);
    }
    return -1;
}

static unsigned long long calc_immediate(unsigned char * immediate, int num_bytes){
    int i = 0;
    unsigned long long imm = 0;
    for(i = 0; i< num_bytes; i++){
        unsigned char byte = immediate[i];
        imm = imm + ((unsigned long long) byte << (i*8));  // little endian!
    } 
    return imm; 
}

unsigned long long get_immediate(unsigned char * inst, int imm_len){
    switch(imm_len){
        case(8): // 1 bytes to read
            return calc_immediate(inst, 1);
        case(16): // 2 bytes to read
            return calc_immediate(inst, 2);
        case(32): // 4 bytes 
            return calc_immediate(inst, 4);
        case(64): // 8 bytes 
            return calc_immediate(inst, 8);
        default:
            assert(0);
    }
   return 0; 
}
