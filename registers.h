#ifndef REGISTERS_H
#define REGISTERS_H

#include "prefix.h"

// Registers are stored in 3 bits

// word registers w = 1 (or default, no w bit needed)
enum regs_16{
    REGS_16_AX = 0x0,
    REGS_16_CX = 0x1,
    REGS_16_DX = 0x2,
    REGS_16_BX = 0x3,
    REGS_16_SP = 0x4,
    REGS_16_BP = 0x5,
    REGS_16_SI = 0x6,
    REGS_16_DI = 0x7,
    REGS_16_R8W = 0x8,
    REGS_16_R9W = 0x9,
    REGS_16_R10W = 0xa,
    REGS_16_R11W = 0xb,
    REGS_16_R12W = 0xc,
    REGS_16_R13W = 0xd,
    REGS_16_R14W = 0xe,
    REGS_16_R15W = 0xf,
};

enum regs_32{
    REGS_32_EAX = 0x0,
    REGS_32_ECX = 0x1,
    REGS_32_EDX = 0x2,
    REGS_32_EBX = 0x3,
    REGS_32_ESP = 0x4,
    REGS_32_EBP = 0x5,
    REGS_32_ESI = 0x6,
    REGS_32_EDI = 0x7,
    REGS_32_R8D = 0x8,
    REGS_32_R9D = 0x9,
    REGS_32_R10D = 0xa,
    REGS_32_R11D = 0xb,
    REGS_32_R12D = 0xc,
    REGS_32_R13D = 0xd,
    REGS_32_R14D = 0xe,
    REGS_32_R15D = 0xf,
};

enum regs_64{
    REGS_64_RAX = 0x0,
    REGS_64_RCX = 0x1,
    REGS_64_RDX = 0x2,
    REGS_64_RBX = 0x3,
    REGS_64_RSP = 0x4,
    REGS_64_RBP = 0x5,
    REGS_64_RSI = 0x6,
    REGS_64_RDI = 0x7,
    REGS_64_R8  = 0x8,
    REGS_64_R9  = 0x9,
    REGS_64_R10 = 0xa,
    REGS_64_R11 = 0xb,
    REGS_64_R12 = 0xc,
    REGS_64_R13 = 0xd,
    REGS_64_R14 = 0xe,
    REGS_64_R15 = 0xf,
};

// byte registers, w = 0 (required)
enum regs_8{
    REGS_8_AL = 0x0,
    REGS_8_CL = 0x1,
    REGS_8_DL = 0x2,
    REGS_8_BL = 0x3,
    REGS_8_AH = 0x4,
        REGS_8_SPL = 0x4, // REX
    REGS_8_CH = 0x5,
        REGS_8_BPL = 0x5,
    REGS_8_DH = 0x6,
        REGS_8_SIL = 0x6,
    REGS_8_BH = 0x7,
        REGS_8_DIL = 0x7,
    REGS_8_R8L = 0x8,
    REGS_8_R9L = 0x9,
    REGS_8_R10L = 0xa,
    REGS_8_R11L = 0xb,
    REGS_8_R12L = 0xc,
    REGS_8_R13L = 0xd,
    REGS_8_R14L = 0xe,
    REGS_8_R15L = 0xf,
};

enum sregs{
    ES = 0x0,
    CS = 0x1,
    SS = 0x2,
    DS = 0x3,
    FS = 0x4,
    GS = 0x5,
};

enum cntrl_regs{
    CR0,
    CR1, 
    CR2, 
    CR3, 
    CR4, 
    CR5, 
    CR6, 
    CR7, 
    CR8, 
};

// 64 bit regs
enum mm_regs{
    MM0,
    MM1, 
    MM2, 
    MM3, 
    MM4, 
    MM5, 
    MM6, 
    MM7, 
    MM8, 
    MM9, 
    MM10, 
    MM11, 
    MM12, 
    MM13, 
    MM14, 
    MM15, 
};

// 128 bit regs
enum xmm_regs{
    XMM0,
    XMM1, 
    XMM2, 
    XMM3, 
    XMM4, 
    XMM5, 
    XMM6, 
    XMM7, 
    XMM8, 
    XMM9, 
    XMM10, 
    XMM11, 
    XMM12, 
    XMM13, 
    XMM14, 
    XMM15, 
};

char * get_register(int index, int mode, struct prefixes * prfx);

#endif 

