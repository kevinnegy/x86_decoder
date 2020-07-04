#ifndef OPCODE_H
#define OPCODE_H

/** Opcodes **/
// Can be 1,2, or 3 bytes. A fourth can be in ModRM
enum one_byte_opcodes{
    // Add - done
    OP_ADD_00 = 0x0, // 8 modrm - reg->rm
    OP_ADD_01 = 0x1, // 16,32,64 modrm - reg->rm
    OP_ADD_02 = 0x2, // 8 modrm - rm->reg
    OP_ADD_03 = 0x3, // 16,32,64 modrm - rm->reg
    OP_ADD_04 = 0x4, // 8 AL imm8
    OP_ADD_05 = 0x5, // 16,32,64 ax,eax,rax imm16,32 
    OP_ADD_80 = 0x80, // 8 modrm imm8 - imm8->rm8
    OP_ADD_81 = 0x81, // 16,32,64 rm imm16,32 - imm8->rm
    OP_ADD_83 = 0x83, // 16,32,64 rm imm8 - imm8->rm

    // Call -done 
    OP_CALL_E8 = 0xe8, // disp 32-> 64 bit sign extend
    OP_CALL_FF = 0xff, // rm64 or jump to different location

    // Jmp -done
    OP_JMP_EB = 0xeb,
    OP_JMP_E9 = 0xe9,
    OP_JMP_FF = 0xff,  

    // Lea done
    OP_LEA = 0x8d, // load effective address

    // Mov done
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
    OP_MOV_B0 = 0xb0, // 8 bit imm to reg 
    OP_MOV_B8 = 0xb8, // mov uses last 3 bits for register, b8-bf, moves imm to reg
    OP_MOV_C6 = 0xc6, // 8 bit imm to reg - use only r/m in modrm
    OP_MOV_C7 = 0xc7, // 16,32,64 rm imm16,32 

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

    // TODO handle
    OP_SHL = 0xc1, // shift logical left always imm8
    OP_TEST_85 = 0x85, // test = reg_1 & reg_2 -> SF, ZF, PF flags
};

enum two_byte_opcodes{
    OP_JZJE_84 = 0x84, // jump if equal or zero (meaning if zf flag = 1)
    OP_POP_A1 = 0xa1, // pop FS register
    OP_POP_A9 = 0xa9, // pop GS register
    OP_PUSH_A0 = 0xa0, // push FS register
    OP_PUSH_A8 = 0xa8, // push GS register
    OP_RDTSC = 0x31, // read time stamp counter
};

void check_opcode(unsigned char * inst, int operand_override, int address_override, int rex);

// In 2 byte mode, it must have one of the following (3 byte is the same except a third opcode):
    // escape opcode 0F primary opcode and then a second opcode
    // mandatory prefix (66, F2, or F3), then escape, then second opcode

#endif
