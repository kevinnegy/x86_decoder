#ifndef PREFIX_H
#define PREFIX_H

#include <stdlib.h>

struct prefixes{
    u_int8_t LOCK: 1;
    u_int8_t REPN: 1;
    u_int8_t REP: 1;
    u_int8_t CS: 1;
    u_int8_t SS: 1;
    u_int8_t DS: 1;
    u_int8_t ES: 1;
    u_int8_t FS: 1;
    u_int8_t GS: 1;
    u_int8_t OPERAND_OVERRIDE: 1;
    u_int8_t ADDRESS_OVERRIDE: 1;
    u_int8_t REX;
    u_int8_t VEX_C5;
    u_int16_t VEX_C4;
};

/** Prefixes **/
void check_prefix(unsigned char * inst, struct prefixes * prfx);
void check_rex(unsigned char * inst, struct prefixes * prfx);
void check_vex_C4(unsigned char * inst);

enum prefix{
    PREFIX_LOCK = 0xf0,
    PREFIX_REPN = 0xf2, // TODO REPN and REP repeat same instruction for each element in string
    PREFIX_REP = 0xf3,
    PREFIX_BND = 0xf2, // TODO Bound prefix shares same hex value as REPN 

    // TODO These use different memory segments than default segment
    PREFIX_CS = 0x2e, 
    PREFIX_SS = 0x36, 
    PREFIX_DS = 0x3e, 
    PREFIX_ES = 0x26, 
    PREFIX_FS = 0x64, 
    PREFIX_GS = 0x65, 

    // TODO Would this affect memory?
    PREFIX_BRANCHN = 0x2e, // TODO shares same hex value as CS 
    PREFIX_BRANCH = 0x3e, // TODO shares same hex as DS 

    PREFIX_OP_SIZE_OVERRIDE = 0x66,
    PREFIX_ADDR_SIZE_OVERRIDE = 0x67,

    PREFIX_VEX_C4 = 0xc4,
    PREFIX_VEX_C5 = 0xc5,
};
//REX-prefix is for 64 bit mode instructions. Only used if instruction operates on 64bit registers. "If rex used when no meaning, it is ignored" (is that a problem?)
// one rex prefix perinstruction, must go right before opcode or escapeopcode byte(0FH) (after any mandatory prefixes)
// one byte REX prefix: 0100WRXB:
#define REX_PREFIX  (1 << 6)    // 0100 required
#define REX_W       (1 << 3)    // 0 = operand size determined by cs.d, 1 = 64 bit operand size
#define REX_R       (1 << 2)    // extension of ModR/M reg field
#define REX_X       (1 << 1)    // extension of SIB index field
#define REX_B       (1 << 0)    // extension of ModR/M r/m field, SIB base field, or Opcode reg field ( how is this determined?, seems like based on if r or x are set.)

#define VEX_C4_R (0x1 << 7)
#define VEX_C4_vvvv (0xf << 3) // if 0xf, ignore, otherwise it is a register specifier
#define VEX_C4_L (0x1 << 2) // 0 = scalar or 128 bit, 1 = 256 bit
#define VEX_C4_pp 0x3       // acts like prefix, 00 = none, 01 = 66, 10 = F3, 11 = F2

#define VEX_C5_R (0x1 << 7)
#define VEX_C5_X (0x1 << 6)
#define VEX_C5_B (0x1 << 5)
#define VEX_C5_mmmm 0x1f    // 0 ignore, 1 implied 0F before opcode, 2 implied 0f 38, 3 0f 34

#define VEX_C5_W (0x1 << 7)
#define VEX_C5_vvvv (0xf << 3)
#define VEX_C5_L (0x1 << 2)
#define VEX_C5_pp 0x3



// Group 1:
    // FO - LOCK prefix - exclusive use of shared memory in multiprocessor environ
    // F2 - REPNE/REPNZ (repeat not equal/zero) - repeat for each element of string or sometimes mandatory for instructions
    // F3 - REP/REPE/REPZ - repeat (equal/zero) - mandatory for POPCNT, LZCNT, ADOX
    // F2 - BND(Bound) prefix iff:
            // CPUID.(EAX=07, ECX=0):EBX.MPX[bit 14] is set
            // BNDCFGU.EN and/or IA32_BNDCFGS.EN is set
            // if F2 precedes a near CALL, near RET, near JMP, short Jcc, or near Jcc instruction
        // BND checks if ret target is within bounds of the values stored at BND0 - BND3 regs 

// Group 2: // causes memory access to use segment outlined by prefix instead of default segment
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
    
#endif


