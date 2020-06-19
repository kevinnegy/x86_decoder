#include <stdlib.h>
#include <stdio.h>
#include "instruction.h"
#include "prefix.h"
#include "opcode.h"

//MOV
//                        100 - rsp 111 - rdi (mov rdi to rsp)
//    0100 1000 1000 1001 1110 0111
//         1R0B 1000 1001 11          - qwordreg 1 to qwordreg2
//         

int main(){
    unsigned char * bytes = (unsigned char *) malloc(sizeof(unsigned char) * MAX_INSTR_LEN);
    bytes[0] = 0xe8;
    bytes[1] = 0x89;
    bytes[2] = 0xe7;
    struct x86_instr * inst = create_x86_instr(bytes);
    check_prefix(inst);
    check_opcode(inst);
    
    int i = 0;
    for(i = 0; i<3; i++){
        printf("%2hhx ", inst->byte_code[i]);
    }
    return 0;
}
