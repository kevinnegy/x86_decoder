#ifndef X86_DECODER_H
#define x86_DECODER_H

#include "instruction.h"
#include "prefix.h"
#include "opcode.h"
#include "modrm.h"

char * disassemble_x86_instr(char * bytes){
    struct x86_instr * inst = create_x86_instr(bytes);
    int opcode_index = find_opcode(inst);
    check_opcode(inst, opcode_index);
    char * x86_string = (char *) malloc(sizeof(char) * 50);
    strcpy(x86_string, get_string(inst));
    
    // Destroy instruction pointer
    delete_x86_instr(inst);
    return x86_string;
}

void x86_decoder_init(int mode){
    set_bit_mode(mode);
    return;
}



#endif
