#include <stdio.h>
#include <stdlib.h>
#include "prefix.h"

int check_prefix(u_int8_t byte){
        switch(byte){
        case PREFIX_LOCK:
        case PREFIX_REPN: // BND shares this same prefix number  
        case PREFIX_REP:
        case PREFIX_CS: // BRANCHN shares prefix
        case PREFIX_SS:
        case PREFIX_DS: // BRANCH shares prefix
        case PREFIX_ES:
        case PREFIX_FS:
        case PREFIX_GS:
        case PREFIX_OP_SIZE_OVERRIDE:
        case PREFIX_ADDR_SIZE_OVERRIDE:
            return 1;
        }
    return 0;
}

int check_rex(u_int8_t byte){
    return (byte & 0xf0) == REX_PREFIX; // Clear last 4 bits 
}

// Returns 16, 32, 64
// TODO should this go in instruction.c?
//int check_bit_mode(struct x86_instr * inst){
//    if (inst->rex != 0x0 && inst->rex & REX_W){
//        return 64;   
//    }
//    return DEFAULT_BIT_MODE;
//}


// Group 1:
    // FO - LOCK prefix - exclusive use of shared memory in multiprocessor environ
    // F2 - REPNE/REPNZ (repeat not equal/zero) - repeat for each element of string or sometimes mandatory for instructions
    // F3 - REP/REPE/REPZ - repeat (equal/zero) - mandatory for POPCNT, LZCNT, ADOX
    // F2 - BND prefix iff:
            // CPUID.(EAX=07, ECX=0):EBX.MPX[bit 14] is set
            // BNDCFGU.EN and/or IA32_BNDCFGS.EN is set
            // When F2 precedes a near CALL, near RET, near JMP, short Jcc, or near Jcc instruction

// Group 2:
    // 2E - CS segment override prefix
    // 36 - SS segment override prefix
    // 3E - DS segment override prefix
    // 26 - ES segment override prefix
    // 64 - FS segment override prefix
    // 65 - GS segment override prefix

    // 2E - Branch not taken (only Jcc instructions)
    // 3E - Branch taken (only Jcc instructions)

// Group 3:
    // 66 - operand size override prefix (switch between 16 and 32 bit operands; use to select non-default) or mandatory

// Group 4:
    // 67 - address size override prefix (switch between 16 and 32 bit addressing)
