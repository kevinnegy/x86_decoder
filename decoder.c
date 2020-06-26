#include "decoder.h"
#include <assert.h>
#include <stdio.h>
#include "prefix.h"
#include "opcode.h"
u_int8_t get_byte(unsigned char * inst, int byte){
    return (u_int8_t) inst[byte];
}

u_int8_t get_byte_2(unsigned char * inst, int byte) {
    switch(byte){
        case 1:
        {
            struct byte_1 * bytes = (struct byte_1 *) inst;
            return bytes->byte;
        }
        case 2:
        {
            struct byte_2 * bytes = (struct byte_2 *) inst;
            return bytes->byte;
            
        }
        case 3:
        {
            struct byte_3 * bytes = (struct byte_3 *) inst;
            return bytes->byte;
            
        }
        case 4:
        {
            struct byte_4 * bytes = (struct byte_4 *) inst;
            return bytes->byte;
            
        }
        case 5:
        {
            struct byte_5 * bytes = (struct byte_5 *) inst;
            return bytes->byte;
            
        }
        case 6:
        {
            struct byte_6 * bytes = (struct byte_6 *) inst;
            return bytes->byte;
            
        }
        case 7:
        {
            struct byte_7 * bytes = (struct byte_7 *) inst;
            return bytes->byte;
            
        }
        case 8:
        {
            struct byte_8 * bytes = (struct byte_8 *) inst;
            return bytes->byte;
            
        }
        case 9:
        {
            struct byte_9 * bytes = (struct byte_9 *) inst;
            return bytes->byte;
            
        }
        case 10:
        {
            struct byte_10 * bytes = (struct byte_10 *) inst;
            return bytes->byte;
            
        }
        case 11:
        {
            struct byte_11 * bytes = (struct byte_11 *) inst;
            return bytes->byte;
            
        }
        case 12:
        {
            struct byte_12 * bytes = (struct byte_12 *) inst;
            return bytes->byte;
            
        }
        case 13:
        {
            struct byte_13 * bytes = (struct byte_13 *) inst;
            return bytes->byte;
            
        }
        case 14:
        {
            struct byte_14 * bytes = (struct byte_14 *) inst;
            return bytes->byte;
            
        }
        case 15:
        {
            struct byte_15 * bytes = (struct byte_15 *) inst;
            return bytes->byte;
            
        }
        default:
            assert(0 < byte && byte < 16);
    }
    return 0;
}

void decoder(unsigned char * inst){
    assert(inst != NULL); 
    u_int8_t byte;
    void * bytes;
    int prefix_exists = 1, rex_exists;
    int byte_num = -1;
    
    while(prefix_exists){
        // Check next byte
        byte_num++;
        byte = get_byte(inst, byte_num);
        printf("next byte %x\n", byte);
        prefix_exists = check_prefix(byte);
    }

    rex_exists = check_rex(byte);
    if(rex_exists){
        check_rex_inst(inst, byte_num);
    }
    else
        check_inst(inst, byte_num);

    printf("Does rex exist %d\n", rex_exists);
    
    
    

}

