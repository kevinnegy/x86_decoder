#ifndef PREFIX_H
#define PREFIX_H

#include "instruction.h"

/** Prefixes **/
void check_prefix(struct x86_instr * inst);

enum prefixes{
    prefix_lock = 0xf0,
    prefix_repn = 0xf2,
    prefix_rep = 0xf3,
    prefix_bnd = 0xf2, // TODO This is an iff prefix

    prefix_cs = 0x2e, 
    prefix_ss = 0x36, 
    prefix_ds = 0x3e, 
    prefix_es = 0x26, 
    prefix_fs = 0x64, 
    prefix_gs = 0x65, 
    prefix_branchn = 0x2e, // TODO another weird repeat
    prefix_branch = 0x3e, // TODO another weird repeat

    prefix_op_size_override = 0x66,
    prefix_addr_size_override = 0x67,
    
    REX = 0x40, // TODO fix
};

#endif
