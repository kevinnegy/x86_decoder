#ifndef REGISTERS_H
#define REGISTERS_H

// Registers are stored in 3 bits

// word registers w = 1 (or default, no w bit needed)
enum regs_16{
    regs_16_ax = 0x0,
    regs_16_cx = 0x1,
    regs_16_dx = 0x2,
    regs_16_bx = 0x3,
    regs_16_sp = 0x4,
    regs_16_bp = 0x5,
    regs_16_si = 0x6,
    regs_16_di = 0x7,
}

enum regs_32{
    regs_32_eax = 0x0,
    regs_32_ecx = 0x1,
    regs_32_edx = 0x2,
    regs_32_ebx = 0x3,
    regs_32_esp = 0x4,
    regs_32_ebp = 0x5,
    regs_32_esi = 0x6,
    regs_32_edi = 0x7,
}

enum regs_64{
    regs_64_rax = 0x0,
    regs_64_rcx = 0x1,
    regs_64_rdx = 0x2,
    regs_64_rbx = 0x3,
    regs_64_rsp = 0x4,
    regs_64_rbp = 0x5,
    regs_64_rsi = 0x6,
    regs_64_rdi = 0x7,
}

// byte registers, w = 0 (required)
enum byte_regs_16{
    byte_regs_16_al = 0x0,
    byte_regs_16_cl = 0x1,
    byte_regs_16_dl = 0x2,
    byte_regs_16_bl = 0x3,
    byte_regs_16_ah = 0x4,
    byte_regs_16_ch = 0x5,
    byte_regs_16_dh = 0x6,
    byte_regs_16_bh = 0x7,
}

enum byte_regs_32{
    byte_regs_32_eal = 0x0,
    byte_regs_32_ecl = 0x1,
    byte_regs_32_edl = 0x2,
    byte_regs_32_ebl = 0x3,
    byte_regs_32_eah = 0x4,
    byte_regs_32_ech = 0x5,
    byte_regs_32_edh = 0x6,
    byte_regs_32_ebh = 0x7,
}

enum byte_regs_64{
    byte_regs_64_ral = 0x0,
    byte_regs_64_rcl = 0x1,
    byte_regs_64_rdl = 0x2,
    byte_regs_64_rbl = 0x3,
    byte_regs_64_rah = 0x4,
    byte_regs_64_rch = 0x5,
    byte_regs_64_rdh = 0x6,
    byte_regs_64_rbh = 0x7,
}

#endif 

