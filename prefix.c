#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "prefix.h"
#include "opcode.h"

void check_prefix(unsigned char * inst, int operand_override, int address_override){
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
            check_prefix(&inst[1], operand_override, address_override);
            return;
        case PREFIX_OP_SIZE_OVERRIDE:
            check_prefix(&inst[1], 1, address_override);
            return;
        case PREFIX_ADDR_SIZE_OVERRIDE:
            check_prefix(&inst[1], operand_override, 1);
            return;
        default:
            check_rex(inst, operand_override, address_override);
            return;
    }
    return;
}

void check_rex(unsigned char * inst, int operand_override, int address_override){
    assert(inst != NULL);
    if((inst[0] & 0xf0) == REX_PREFIX) // Clear last 4 bits 
        check_opcode(&inst[1], operand_override, address_override, inst[0]);
    else
        check_opcode(inst, operand_override, address_override, 0);
    return;
}
