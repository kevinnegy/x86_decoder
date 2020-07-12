#ifndef OPCODE_H
#define OPCODE_H

/** Opcodes **/
// Can be 1,2, or 3 bytes. A fourth can be in ModRM
enum one_byte_opcodes{
    // Add - done
    OP_ADD_04 = 0x4, // 8 AL imm8
    OP_ADD_05 = 0x5, // 16,32,64 ax,eax,rax imm16,32 
    OP_ADD_80 = 0x80, // 8 modrm imm8 - imm8->rm8
    OP_ADD_81 = 0x81, // 16,32,64 rm imm16,32 - imm8->rm
    OP_ADD_83 = 0x83, // 16,32,64 rm imm8 - imm8->rm
    OP_ADD_00 = 0x0, // 8 modrm - reg->rm
    OP_ADD_01 = 0x1, // 16,32,64 modrm - reg->rm
    OP_ADD_02 = 0x2, // 8 modrm - rm->reg
    OP_ADD_03 = 0x3, // 16,32,64 modrm - rm->reg

    // AND - done
    OP_AND_24 = 0x24, // 8 AL imm8
    OP_AND_25 = 0x25, // 16,32,64 ax,eax,rax imm16,32 
    OP_AND_80 = 0x80, // 8 modrm imm8 - imm8->rm8
    OP_AND_81 = 0x81, // 16,32,64 rm imm16,32 - imm8->rm
    OP_AND_83 = 0x83, // 16,32,64 rm imm8 - imm8->rm
    OP_AND_20 = 0x20, // 8 modrm - reg->rm
    OP_AND_21 = 0x21, // 16,32,64 modrm - reg->rm
    OP_AND_22 = 0x22, // 8 modrm - rm->reg
    OP_AND_23 = 0x23, // 16,32,64 modrm - rm->reg
    
    // Call -done 
    OP_CALL_E8 = 0xe8, // disp 32-> 64 bit sign extend
    OP_CALL_FF = 0xff, // rm16,32,64 

    // CMP - done
    OP_CMP_3C = 0x3c, // 8 AL imm8
    OP_CMP_3D = 0x3d, // 16,32,64 ax,eax,rax imm16,32 
    OP_CMP_80 = 0x80, // 8 modrm imm8 - imm8->rm8
    OP_CMP_81 = 0x81, // 16,32,64 rm imm16,32 - imm8->rm
    OP_CMP_83 = 0x83, // 16,32,64 rm imm8 - imm8->rm
    OP_CMP_38 = 0x38, // 8 modrm - reg->rm
    OP_CMP_39 = 0x39, // 16,32,64 modrm - reg->rm
    OP_CMP_3A = 0x3a, // 8 modrm - rm->reg
    OP_CMP_3B = 0x3b, // 16,32,64 modrm - rm->reg

    OP_CMPS_A6 = 0xa6, // compare RSI/ESI to RDI/EDI and set flags
    OP_CMPS_A7 = 0xa7, // compare RSI/ESI to RDI/EDI and set flags

    OP_CBW_98 = 0x98, // convert byte to word AL->AX, word to double AX->EAX, double to quad EAX->RAX 

    OP_CONVERT_99 = 0x99, // convert word double (CWD), CDQ, CQO

    // DEC -done
    OP_DEC_FE = 0xfe, // rm8
    OP_DEC_FF = 0xff, // rm16,32,64

    // DIV
    OP_DIV_F6 = 0xf6, // AX <- AL * rm8 
    OP_DIV_F7 = 0xf7, // rm 16,32,64 (same form as F6)

    // IDIV -signed multiply
    OP_IDIV_F6 = 0xf6, // AX <- AL * rm8 
    OP_IDIV_F7 = 0xf7, // rm 16,32,64 (same form as F6)

    // IMUL -signed multiply
    OP_IMUL_F6 = 0xf6, // AX <- AL * rm8 
    OP_IMUL_F7 = 0xf7, // rm 16,32,64 (same form as F6)
    OP_IMUL_6B = 0x6b, // imm8 * rm16,32,64 -> reg 16,32,64 
    OP_IMUL_69 = 0x69, // imm16,32 * rm16,32,64 -> reg 16,32,64 

    // INC -done
    OP_INC_FE = 0xfe, // rm8
    OP_INC_FF = 0xff, // rm16,32,64

    // JCC -done (see opcode.c for which flags are in the condition)
    // TODO handle instructions that default to 64 bit operands in long mode like jcc
    OP_JCC_70 = 0x70, // JCC covers 70-7F
    OP_JCC_E3 = 0xe3, // JECXZ or JRCXZ (ECX/RCX = 0)

    // Jmp -done
    OP_JMP_EB = 0xeb,
    OP_JMP_E9 = 0xe9,
    OP_JMP_FF = 0xff,  

    // Lea done
    OP_LEA = 0x8d, // load effective address

    // Leave done
    OP_LEAVE_C9 = 0xc9, // stack pointer = base pointer, then pop base pointer 

    // Mov -done
    OP_MOV_88 = 0x88, // 8 bit regs/mm - reg->rm
    OP_MOV_89 = 0x89, // 16,32,64 regs/mm - reg->rm
    OP_MOV_8A = 0x8a, // 8 bit regs/mm - rm->reg
    OP_MOV_8B = 0x8b, // 16,32,64 regs/mm - rm->reg
    OP_MOV_8C = 0x8c, // mov sreg to 16/64 reg - reg->rm 
    OP_MOV_8E = 0x8e, // mov sreg to 16/64 reg - rm->reg 
    OP_MOV_A0 = 0xa0, // mov moffs8 to AL - moffs is seg:offset or just offset if REX.W 
    OP_MOV_A1 = 0xa1, // mov moffs16,32,64 to AX,EAX,RAX - moffs is seg:offset or just offset if REX.W 
    OP_MOV_A2 = 0xa2, // mov AL to moffs8 - moffs is seg:offset or just offset if REX.W 
    OP_MOV_A3 = 0xa3, // mov AX,EAX,RAX to moffs16,32,64 - moffs is seg:offset or just offset if REX.W 
    OP_MOV_B0 = 0xb0, // 8 bit imm to reg  (get reg from opcode B0-B7)
    OP_MOV_B8 = 0xb8, // imm16,32,64 -> reg16,32,64 (get reg from opcode B8-BF)
    OP_MOV_C6 = 0xc6, // 8 bit imm to reg - use only r/m in modrm
    OP_MOV_C7 = 0xc7, // 16,32,64 rm imm16,32 
    OP_MOVSX_63 = 0x63, // 16,32,64 regs/mm - rm->reg (sign extension move)

    // MUL
    OP_MUL_F6 = 0xf6, // AX <- AL * rm8 
    OP_MUL_F7 = 0xf7, // rm 16,32,64 (same form as F6)

    // Neg -done 
    OP_NEG_F6 = 0xf6, // 8 bit rm 
    OP_NEG_F7 = 0xf7, // 16,32,64 bit rm 
    
    // Nop -done
    OP_NOP_90 = 0x90, // nop one byte

    // Not -done 
    OP_NOT_F6 = 0xf6, // 8 bit rm 
    OP_NOT_F7 = 0xf7, // 16,32,64 bit rm 

    // Or - done
    OP_OR_0C = 0x0c, // 8 AL imm8
    OP_OR_0D = 0x0d, // 16,32,64 ax,eax,rax imm16,32 
    OP_OR_80 = 0x80, // 8 bit rm imm8
    OP_OR_81 = 0x81, // 16,32,64 rm imm16,32 
    OP_OR_83 = 0x83, // 16,32,64 rm imm8 - imm8->rm
    OP_OR_08 = 0x8, // 8bit regs/mm - reg->rm
    OP_OR_09 = 0x9, // normal reg->rm
    OP_OR_0A = 0xa, // 8 bit regs/mm - rm->reg 
    OP_OR_0B = 0xb, // normal rm->reg 

    // Pop-done
    OP_POP_8F = 0x8f, // pop is 58-5f, last 3 bits are for register
    OP_POP_58 = 0x58, // pop is 58-5f, last 3 bits are for register

    // Push -done
    OP_PUSH_FF = 0xff, // push 16,64 bit register 
    OP_PUSH_6A = 0x6a, // push imm8 
    OP_PUSH_68 = 0x68, // push imm16,32 
    OP_PUSH_50 = 0x50, // push is 50-57, last 3 bits are for register

    // Return
    OP_RET_C3 = 0xc3, // near return
    OP_RET_CB = 0xcb, // far return
    OP_RET_C2 = 0xc2, // near return, pop imm16 

    // Rotate (RCL, RCR, ROL, ROR)
    OP_ROT_C0 = 0xc0, // 8 bit rm rotate imm8 number of times 
    OP_ROT_C1 = 0xc1, // 16,32,64 bit rm rotate imm8 # of times
    OP_ROT_D0 = 0xd0, // 8 bit rm rotate once left 
    OP_ROT_D1 = 0xd1, // 16,32,64 rm rotate once left 
    OP_ROT_D2 = 0xd2, // 8 bit rm rotate CL register number of times 
    OP_ROT_D3 = 0xd3, // 16,32,64 bit rm rotate CL register number of times 
    
    // SAR - done (sign bit)
    OP_SAR_C0 = 0xc0, // 8 bit rm shift imm8 number of times 
    OP_SAR_C1 = 0xc1, // 16,32,64 bit rm shift imm8 # of times
    OP_SAR_D0 = 0xd0, // 8 bit rm shift right once 
    OP_SAR_D1 = 0xd1, // 16,32,64 rm shift once 
    OP_SAR_D2 = 0xd2, // 8 bit rm shift CL register number of times 
    OP_SAR_D3 = 0xd3, // 16,32,64 bit rm shift CL register number of times 
    OP_RET_CA = 0xca, // far return, pop imm16 

    // SBB -done
    OP_SBB_1C = 0x1c, // 8 AL imm8
    OP_SBB_1D = 0x1d, // 16,32,64 ax,eax,rax imm16,32 
    OP_SBB_80 = 0x80, // 8 modrm imm8 - imm8->rm8
    OP_SBB_81 = 0x81, // 16,32,64 rm imm16,32 - imm8->rm
    OP_SBB_83 = 0x83, // 16,32,64 rm imm8 - imm8->rm
    OP_SBB_18 = 0x18, // 8 modrm - reg->rm
    OP_SBB_19 = 0x19, // 16,32,64 modrm - reg->rm
    OP_SBB_1A = 0x1a, // 8 modrm - rm->reg
    OP_SBB_1B = 0x1b, // 16,32,64 modrm - rm->reg

    // SHL - done (SHL and SAL are the same)
    OP_SHL_C0 = 0xc0, // 8 bit rm shift imm8 number of times 
    OP_SHL_C1 = 0xc1, // 16,32,64 bit rm shift imm8 # of times
    OP_SHL_D0 = 0xd0, // 8 bit rm shift once left 
    OP_SHL_D1 = 0xd1, // 16,32,64 rm shift once left 
    OP_SHL_D2 = 0xd2, // 8 bit rm shift CL register number of times 
    OP_SHL_D3 = 0xd3, // 16,32,64 bit rm shift CL register number of times 
    
    // SHR -done (unsigned) 
    OP_SHR_C0 = 0xc0, // 8 bit rm shift imm8 number of times 
    OP_SHR_C1 = 0xc1, // 16,32,64 bit rm shift imm8 # of times
    OP_SHR_D0 = 0xd0, // 8 bit rm shift right once 
    OP_SHR_D1 = 0xd1, // 16,32,64 rm shift once 
    OP_SHR_D2 = 0xd2, // 8 bit rm shift CL register number of times 
    OP_SHR_D3 = 0xd3, // 16,32,64 bit rm shift CL register number of times 

    // Sub -done
    OP_SUB_2C = 0x2c,  // 8 AL imm8
    OP_SUB_2D = 0x2d,  // 16,32,64 ax,eax,rax imm16,32 
    OP_SUB_80 = 0x80, // 8 bit rm imm8
    OP_SUB_81 = 0x81, // 16,32,64 rm imm16,32 
    OP_SUB_83 = 0x83, // 16,32,64 rm imm8 - imm8->rm
    OP_SUB_28 = 0x28, // 8bit regs/mm - reg->rm  
    OP_SUB_29 = 0x29, // normal reg->rm
    OP_SUB_2A = 0x2a, // 8bit regs/mm - rm->reg  
    OP_SUB_2B = 0x2b, // normal rm->reg 

    // STOS - store string
    OP_STOS_AA = 0xaa, // store AL in [RDI/EDI]
    OP_STOS_AB = 0xab, // store AX,EAX,RAX in [RDI/EDI]

    // Test - done, no memory writing, just ANDs operands and sets SF PF ZF flags
    OP_TEST_A8 = 0xa8, // 8 AL imm8 
    OP_TEST_A9 = 0xa9, // 16,32,64 ax,eax,rax imm16,32 
    OP_TEST_F6 = 0xf6, // 8 bit rm imm8
    OP_TEST_F7 = 0xf7, // 16,32,64 rm imm16,32 
    OP_TEST_84 = 0x84, // 8bit regs/mm - reg->rm 
    OP_TEST_85 = 0x85, // normal reg->rm 
    
    // XCHG - done
    OP_XCHG_90 = 0x90, // swap AX,EAX,RAX with rm 16,32,64
    OP_XCHG_86 = 0x86, // swap rm8 and reg8 
    OP_XCHG_87 = 0x87, // swap rm16,32,64 and reg16,32,64 
    
    // XOR -done
    OP_XOR_34 = 0x34, // 8 AL imm8 
    OP_XOR_35 = 0x35, // 16,32,64 ax,eax,rax imm16,32 
    OP_XOR_80 = 0x80, // 8 bit rm imm8
    OP_XOR_81 = 0x81, // 16,32,64 rm imm16,32 
    OP_XOR_83 = 0x83, // 16,32,64 rm imm8 - imm8->rm
    OP_XOR_30 = 0x30, // 8bit regs/mm - reg->rm  
    OP_XOR_31 = 0x31, // normal reg->rm
    OP_XOR_32 = 0x32, // 8bit regs/mm - rm->reg  
    OP_XOR_33 = 0x33, // normal rm->reg 
};

enum two_byte_opcodes{
    // CMOV 
    OP_CMOV_40 = 0x40, // CMOV covers 40-4f

    // Bit test (stores answer in CF flag)
    OP_BT_A3 = 0xa3, // normal reg -> rm
    OP_BT_BA = 0xba, // normal imm8 -> rm

    // BSF - bit scan forward
    OP_BSF_BC = 0xbc, // normal rm -> reg

    // Done
    OP_CMPXCHG_B0 = 0xb0, // cmp rm8 with AL, if equal, ZF set and r8 loaded into rm8
    OP_CMPXCHG_B1 = 0xb1, // cmp rm16,32,64 with AX,EAX,RAX, if equal, ZF set and r8 loaded into rm8
    OP_CPUID_A2 = 0xa2, // Gets CPU info into registers based on what EAX stores
    OP_JCC_80 = 0x80, // 64 bit mode JCC instructions are 80-8F (see one byte JCC instructions)
    OP_IMUL_AF = 0xaf, // normal rm -> reg

    // TODO handle vex
    // TODO handle prefixes (override prefixes)
    OP_MOVAPS_28 = 0x28, // xmm/m128 -> xmm
    OP_MOVAPS_29 = 0x29, // xmm -> xmm/m128 
    OP_MOVD_6E = 0x6e, // rm32,64 -> mm registers (Also MOVQ)
    OP_MOVD_7E = 0x7e, // mm registers -> rm32,64
    OP_MOVQ_D6 = 0xd6, //xmm -> xmm/m 
    OP_MOVDQU_6F = 0x6f, // xmm/m -> xmm 
    OP_MOVDQU_7F = 0x7f, // xmm -> xmm/m 
    OP_MOVHPD_16 = 0x16, // m64 -> xmm 
    OP_MOVLPD_12 = 0x12, // m64 -> xmm 
    OP_MOVUPS_10 = 0x10, // xmm/m -> xmm
    OP_MOVUPS_11 = 0x11, // xmm -> xmm/m

    // MOVS/ZX -done
    OP_MOVSX_BE = 0xbe, // rm8 -> reg16,32,64 (sign extend)
    OP_MOVSX_BF = 0xbf, // rm16 -> reg32,64
    OP_MOVZX_B6 = 0xb6, // rm8 -> reg16,32,64 (zero extend)
    OP_MOVZX_B7 = 0xb7, // rm16 -> reg32,64

    // NOP - done 
    OP_NOP_1F = 0x1f, // NOP multi byte (rm32,64)

    // TODO handle vex
    OP_PADD_FC = 0xfc, // mm/m64 -> mm or xmm/m128 -> xmm
    OP_PADD_FD = 0xfd, // mm/m64 -> mm or xmm/m128 -> xmm
    OP_PADD_FE = 0xfe, // mm/m64 -> mm or xmm/m128 -> xmm
    OP_PADD_D4 = 0xd4, // mm/m64 -> mm or xmm/m128 -> xmm
    OP_PCMPGT_64 = 0x64, // either compare all (1)bytes in 64/128 bit data or
    OP_PCMPGT_65 = 0x65, 
    OP_PCMPGT_66 = 0x66, 
    OP_PCMPEQ_74 = 0x74,
    OP_PCMPEQ_75 = 0x75, // (2) words
    OP_PCMPEQ_76 = 0x76, // (3) doublewords 
    OP_PMAXUB_DE = 0xde, // mm/m64 -> mm or xmm/m128 -> xmm
    OP_PMINUB_DA = 0xda, // mm (dest), mm2/m64(src) or xmm
    OP_PMOVMSK_D7 = 0xd7, // mm(64) -> reg(32 or 64) or xmm(128) -> reg(32 or 64) // 64 bit operand size is default in 64 bit mode
    OP_POR_EB = 0xeb, // mm/m64 -> mm or xmm/m128 -> xmm 
    OP_PSHUFD_70 = 0x70,
    OP_PSLL_F1 = 0xf1, // mm/m64 -> mm or xmm/m128 -> xmm
    OP_PSLL_F2 = 0xf2,
    OP_PSLL_F3 = 0xf3,
    OP_PSLL_71 = 0x71, // shift mm by imm8  
    OP_PSLL_72 = 0x72, // shift mm by imm8  
    OP_PSLL_73 = 0x73, // shift mm by imm8  
    OP_PSLLDQ_73 = 0x73, // imm8 ->xmm
    OP_PSUBB_F8 = 0xF8,
    OP_PSUBW_F9 = 0xF9,
    OP_PSUBD_FA = 0xFA,
    OP_PUNPCK_60 = 0x60, // mm/m32 -> mm or xmm2/m128 -> xmm
    OP_PUNPCK_61 = 0x61, // mm -> mm/m32 or xmm-> xmm2/m128
    OP_PUNPCK_62 = 0x62, // mm/m32 -> mm or xmm2/m128 -> xmm
    OP_PUNPCK_6C = 0x6C, // xmm2/m128 -> xmm
    OP_PUNPCKH_68 = 0x68, // mm/m32 -> mm or xmm2/m128 -> xmm
    OP_PUNPCKH_69 = 0x69, // mm/m32 -> mm or xmm2/m128 -> xmm
    OP_PUNPCKH_6A = 0x6a, // mm/m32 -> mm or xmm2/m128 -> xmm
    OP_PUNPCKH_6D = 0x6d, // xmm2/m128 -> xmm
    OP_PXOR_EF = 0xef, // logical xor

    //Done
    OP_POP_A1 = 0xa1, // pop FS register
    OP_POP_A9 = 0xa9, // pop GS register
    OP_PUSH_A0 = 0xa0, // push FS register
    OP_PUSH_A8 = 0xa8, // push GS register
    OP_RDTSC = 0x31, // read time stamp counter
    OP_SETCC_90 = 0x90, // SETCC covers 90-9F
    OP_SYSCALL_05 = 0x05, // Fast syscall, modifies a lot of registers but not stack pointer
    OP_XADD_C0 = 0xc0, // swaps operands(rm8, r8) and then puts sum in rm8
    OP_XADD_C1 = 0xc1, // swaps operands(rm16,32,64, r16,32,64) and then puts sum in rm
    OP_XGETBV_01 = 0x01, // get value of control register EDX:EAX <- XCR[ECX]
    OP_XSAVEC_C7 = 0xc7, // save state components to rm
    OP_XRSTOR_AE = 0xae, // restore state components from rm
};

enum three_byte_opcodes{
    // Move data after swapping bytes
    OP_MOVBE_F0 = 0xf0, // rm (m only) -> reg
    OP_MOVBE_F1 = 0xf1, // reg -> rm (m only) 

    OP_PALIGNR_0F = 0x0f, 
    OP_PCMPISTRI_63 = 0x63, // xmm, xmm/m128, imm8
    OP_PMAXUB_3E = 0x3e, // mm/m64 -> mm or xmm/m128 -> xmm
    OP_PMINUB_3A = 0x3a, // xmm (dest), xmm/m128(src)

};

void check_opcode(unsigned char * inst, int operand_override, int address_override, int rex);

// In 2 byte mode, it must have one of the following (3 byte is the same except a third opcode):
    // escape opcode 0F primary opcode and then a second opcode
    // mandatory prefix (66, F2, or F3), then escape, then second opcode

#endif
