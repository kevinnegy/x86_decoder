#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "instruction.h"

void build_x86_string(struct x86_instr * inst);

void check_sib(struct x86_instr * inst);
void check_displacement(struct x86_instr * inst);
void check_immediate(struct x86_instr * inst);


// Copies user-provided bytecode into new x86 struct
// TODO maybe change this to something else. Is a function necessary?
struct x86_instr * create_x86_instr(char * bytes){ 

    struct x86_instr * inst = (struct x86_instr *) malloc(sizeof(struct x86_instr));
    
    inst->prefix_flag = 0; // If checkprefix finds a prefix, this will be set to 1
    inst->rex_ptr = 0; 
    inst->opcode_ptr = 0; 
    inst->modrm_ptr = 0; 
    inst->sib_ptr = 0; 
    inst->immediate_ptr = 0; 
    inst->displacement_ptr = 0; 

    inst->byte_code = (unsigned char *) malloc(sizeof(unsigned char) * MAX_INSTR_LEN); 
    memcpy(inst->byte_code, bytes, MAX_INSTR_LEN); 

    inst->x86_string = (char *) malloc(sizeof(char) * 50);

    return inst;

}

void delete_x86_instr(struct x86_instr * inst){
    free(inst->byte_code);
    free(inst->x86_string);
    free(inst);

    return;
}

char * get_string(struct x86_instr * inst){
    return inst->x86_string;
}


/** Displacement/Immediate **/
// Address displacement - 1, 2, or 4 bytes or none
// Immediate - 1,2,4, or none



