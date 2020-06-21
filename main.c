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

void test1(unsigned char * bytes){
    struct x86_instr * inst = create_x86_instr(bytes);
    int opcode_index = find_opcode(inst);
    check_opcode(inst, opcode_index);
    
    int i = 0;
    for(i = 0; i<3; i++){
        printf("%2hhx ", inst->byte_code[i]);
    }
    printf("\n");
    printf("my string:\n%s\n", inst->x86_string);
    delete_x86_instr(inst);
}

int main(){
    unsigned char * bytes = (unsigned char *) malloc(sizeof(unsigned char) * MAX_INSTR_LEN);
    if(bytes == NULL){
        printf("malloc failed in main()\n");
        return -1;
    }

    bytes[0] = 0x48;
    bytes[1] = 0x89;
    bytes[2] = 0xe7;
    test1(bytes);
    printf("mov rdi, rsp\n");

    bytes[0] = 0xe8;
    bytes[1] = 0x08;
    bytes[2] = 0x0e;
    test1(bytes);
    printf("call 0x\n");

    bytes[0] = 0x40;
    bytes[1] = 0x55;
    bytes[2] = 0x0e;
    test1(bytes);
    printf("push rbp\n");

    bytes[0] = 0x48;
    bytes[1] = 0x89;
    bytes[2] = 0xe5;
    test1(bytes);
    printf("mov rbp, rsp\n");

    bytes[0] = 0x41;
    bytes[1] = 0x57;
    bytes[2] = 0xe5;
    test1(bytes);
    printf("push r15\n");

    return 0;
}

