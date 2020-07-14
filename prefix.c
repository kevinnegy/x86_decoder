#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "prefix.h"
#include "opcode.h"

void check_prefix(unsigned char * inst, struct prefixes * prfx){
    assert(inst != NULL);
    switch(inst[0]){
        case PREFIX_LOCK:
            assert(prfx->LOCK == 0);
            prfx->LOCK = 1;
            break;
        case PREFIX_REPN: // BND shares this same prefix number  
            assert(prfx->REPN == 0);
            prfx->REPN = 1;
            break;
        case PREFIX_REP:
            assert(prfx->REP == 0);
            prfx->REP = 1;
            break;
        case PREFIX_CS: // BRANCHN shares prefix
            assert(prfx->CS == 0);
            prfx->CS = 1;
            break;
        case PREFIX_SS:
            assert(prfx->SS == 0);
            prfx->SS = 1;
            break;
        case PREFIX_DS: // BRANCH shares prefix
            assert(prfx->DS == 0);
            prfx->DS = 1;
            break;
        case PREFIX_ES:
            assert(prfx->ES == 0);
            prfx->ES = 1;
            break;
        case PREFIX_FS:
            assert(prfx->FS == 0);
            prfx->FS = 1;
            break;
        case PREFIX_GS:
            assert(prfx->GS == 0);
            prfx->GS = 1;
            break;
        case PREFIX_OP_SIZE_OVERRIDE:
            assert(prfx->OPERAND_OVERRIDE == 0);
            prfx->OPERAND_OVERRIDE = 1;
            break;
        case PREFIX_ADDR_SIZE_OVERRIDE:
            assert(prfx->ADDRESS_OVERRIDE == 0);
            prfx->ADDRESS_OVERRIDE = 1;
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
        assert(prfx->REX == 0);
        prfx->REX = inst[0];
        check_opcode(&inst[1], prfx->OPERAND_OVERRIDE, prfx->ADDRESS_OVERRIDE, prfx->REX);
    }
    else{
        check_opcode(inst, prfx->OPERAND_OVERRIDE, prfx->ADDRESS_OVERRIDE, prfx->REX);
    }
    return;
}

void check_vex_C4(unsigned char * inst){
    printf("WE HAVE A VEX");
    assert(0);
    return;
}

