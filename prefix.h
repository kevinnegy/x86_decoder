#ifndef PREFIX_H
#define PREFIX_H

/** Prefixes **/
int check_prefix(u_int8_t byte);
int check_rex(u_int8_t byte);

enum prefixes{
    PREFIX_LOCK = 0xf0,
    PREFIX_REPN = 0xf2,
    PREFIX_REP = 0xf3,
    PREFIX_BND = 0xf2, // TODO This is an iff prefix

    PREFIX_CS = 0x2e, 
    PREFIX_SS = 0x36, 
    PREFIX_DS = 0x3e, 
    PREFIX_ES = 0x26, 
    PREFIX_FS = 0x64, 
    PREFIX_GS = 0x65, 
    PREFIX_BRANCHN = 0x2e, // TODO another weird repeat
    PREFIX_BRANCH = 0x3e, // TODO another weird repeat

    PREFIX_OP_SIZE_OVERRIDE = 0x66,
    PREFIX_ADDR_SIZE_OVERRIDE = 0x67,
};

//REX-prefix is for 64 bit mode instructions. Only used if instruction operates on 64bit registers. "If rex used when no meaning, it is ignored" (is that a problem?)
// one rex prefix perinstruction, must go right before opcode or escapeopcode byte(0FH) (after any mandatory prefixes)
// one byte REX prefix: 0100WRXB:
#define REX_PREFIX  (1 << 6)    // 0100 required
#define REX_W       (1 << 3)    // 0 = operand size determined by cs.d, 1 = 64 bit operand size
#define REX_R       (1 << 2)    // extension of ModR/M reg field
#define REX_X       (1 << 1)    // extension of SIB index field
#define REX_B       (1 << 0)    // extension of ModR/M r/m field, SIB base field, or Opcode reg field ( how is this determined?, seems like based on if r or x are set.)

#endif
