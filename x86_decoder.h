#ifndef X86_DECODER_H
#define x86_DECODER_H

#include "instruction.h"
#include "prefix.h"
#include "opcode.h"
#include "modrm.h"

char * disassemble_x86_instr(char * bytes){
    struct x86_instr * inst = create_x86_instr(bytes);
    check_prefix(inst);
    check_opcode(inst);
    char * x86_string = (char *) malloc(sizeof(char) * 50);
    strcpy(x86_string, get_string(inst));
    
    // Destroy instruction pointer
    delete_x86_instr(inst);
    return x86_string;

}


#endif
