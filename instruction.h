#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#define MAX_INSTR_LEN 15 

// intel syntax has destination first like mips
// bytes should be stored as unsigned char, 2 digit hexadecimals

// Stores the byte instruction, the index of each segment, and the string assembly instruction
struct x86_instr{
    unsigned char * byte_code;
    int prefix_flag; // If it exists, must be index 0. 
    int rex_ptr;
    int opcode_ptr;
    int modrm_ptr;
    int sib_ptr;
    int displacement_ptr;
    int immediate_ptr; 
    char * x86_string;
};

struct x86_instr * create_x86_instr(char * bytes); 

#endif
