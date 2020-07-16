#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "prefix.h"
#include "opcode.h"

void check_prefix(unsigned char * inst, struct prefixes * prfx){
    assert(inst != NULL);
    switch(inst[0]){
        case PREFIX_LOCK:
            prfx->LOCK = 1;
            break;
        case PREFIX_REPN: // BND shares this same prefix number  
            prfx->REPN = 1;
            break;
        case PREFIX_REP:
            prfx->REP = 1;
            break;
        case PREFIX_CS: // BRANCHN shares prefix
            prfx->CS = 1;
            break;
        case PREFIX_SS:
            prfx->SS = 1;
            break;
        case PREFIX_DS: // BRANCH shares prefix
            prfx->DS = 1;
            break;
        case PREFIX_ES:
            prfx->ES = 1;
            break;
        case PREFIX_FS:
            prfx->FS = 1;
            break;
        case PREFIX_GS:
            prfx->GS = 1;
            break;
        case PREFIX_OP_SIZE_OVERRIDE:
            prfx->OPERAND_OVERRIDE = 1;
            break;
        case PREFIX_ADDR_SIZE_OVERRIDE:
            prfx->ADDRESS_OVERRIDE = 1;
            break;
        case PREFIX_VEX_C4: // three byte
            prfx->VEX_C4 = (u_int16_t) inst[1] << 4;
            prfx->VEX_C4 = prfx->VEX_C5 + inst[2];
            inst++;
            inst++;
            assert(0);
            break;
        case PREFIX_VEX_C5: // two byte
            prfx->VEX_C5 = inst[1];
            inst++;
            break;
        case PREFIX_EVEX_62:
            assert(0);
            break;
        default:
            check_rex(inst, prfx);
            return;
    }

    check_prefix(&inst[1], prfx);
    return;
}

void check_rex(unsigned char * inst, struct prefixes * prfx){
    assert(inst != NULL);
    if((inst[0] & 0xf0) == REX_PREFIX){ // Clear last 4 bits 
        prfx->REX = inst[0];
        check_opcode(&inst[1], prfx);
    }
    else{
        check_opcode(inst, prfx);
    }
    return;
}

void check_vex_C4(unsigned char * inst){
    printf("WE HAVE A VEX");
    assert(0);
    return;
}

