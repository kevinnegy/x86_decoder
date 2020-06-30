#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "prefix.h"
#include "opcode.h"

void check_prefix(unsigned char * inst){
    assert(inst != NULL);
    switch(inst[0]){
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
            check_prefix(&inst[1]);
        default:
            check_rex(inst);
    }
    return;
}

void check_rex(unsigned char * inst){
    assert(inst != NULL);
    if((inst[0] & 0xf0) == REX_PREFIX) // Clear last 4 bits 
        check_opcode_rex(&inst[1], inst[0]);
    else
        check_opcode(inst);
    return;
}
