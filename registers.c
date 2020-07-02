#include <stdio.h>
#include <assert.h>
#include "registers.h"

static char * get_register_16(int index){
    char * strings[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di", "r8w", "r9w", "r10w", "r11w", "r12w", "r13w", "r14w", "r15w"};
    return strings[index];
}

static char * get_register_32(int index){
    char * strings[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi", "r8d", "r9d", "r10d", "r11d", "r12d", "r13d", "r14d", "r15d"};
    return strings[index];
}

static char * get_register_64(int index){
    char * strings[] = {"rax", "rcx", "rdx", "rbx", "rsp", "rbp", "rsi", "rdi", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"};
    return strings[index];
}


static char * get_register_8(int index){
    char * strings[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};
    return strings[index];
}

static char * get_register_8_rex(int index){
    char * strings[] = {"al", "cl", "dl", "bl", "spl", "bpl", "sil", "dil", "r8l", "r9l", "r10l", "r11l", "r12l", "r13l", "r14l", "r15l"};
    return strings[index];
}

static char * get_sregister(int index){
    char * strings[] = {"es", "cs", "ss", "ds", "fs", "gs"};
    return strings[index];
}

static char * get_cntrl_register(int index){
    char * strings[] = {"cr0", "cr1", "cr2", "cr3", "cr4", "cr5", "cr6", "cr7", "cr8"};
    return strings[index];
}

char * get_register(int index, int mode, int rex){
    if(index < 0 || index > 15)
        assert(0);

    if(mode == 8 && rex == 0){
        if(index > 7) assert(0);
        return get_register_8(index);
    }
    else if(mode == 8)
        return get_register_8_rex(index);
    else if(mode == 16)
        return get_register_16(index);
    else if(mode == 32)
        return get_register_32(index);
    else if(mode == 64)
        return get_register_64(index);
    else if(mode == 1){ // internal identification of sregs
        if(index > 5) assert(0);
        return get_sregister(index);
    }
    else if(mode == 2){ // internal identification of cntrl_regs
        if(index > 9) assert(0);
        return get_cntrl_register(index);
    }
    return NULL;        
}
