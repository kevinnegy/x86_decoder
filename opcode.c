#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "opcode.h"
#include "x86_decoder.h"
#include "prefix.h"
#include "immediates.h"
#include "modrm.h"
#include "registers.h"

// Destination first, source second

// Assume 64 bit programs only 
int find_operand_size(int operand_override, int rex){
    if(rex & REX_W) // 66 byte prefix is ignored if REX_W exists
        return 64;
    if(operand_override)
        return 16;
    return 32;     
}

// Assume 64 bit programs only - can never be 16 bit mode. See wiki.osdev.org table on "operand-size and address-size override prefix"
int find_address_size(int address_override){
    if(address_override)
        return 32;
    return 64;     
}

void check_third_opcode(unsigned char * inst, int operand_override, int address_override, int rex){
    assert(inst != NULL);
    int operand_size = find_operand_size(operand_override, rex);
    int address_size = find_address_size(operand_override);
    return;
}


void check_second_opcode(unsigned char * inst, int operand_override, int address_override, int rex){
    assert(inst != NULL);

    int opcode = inst[0];
    
    if(opcode == 0x38 || opcode == 0x3a){
        check_third_opcode(&inst[1], operand_override, address_override, rex);
        return;
    }

    int operand_size = find_operand_size(operand_override, rex);
    int address_size = find_address_size(operand_override);
    switch(opcode){
        case OP_JZJE_84: //TODO are tttn encodings necessary?
        {
            // TODO handle
            int64_t disp = get_displacement(&inst[1], DEFAULT_BIT_MODE, 0);

            if(DEFAULT_BIT_MODE == 16)
                disp = disp + 4; 
            else
                disp = disp + 6; 

            printf("disp 0x%lx\n", disp);

            return;
        }
        case OP_RDTSC:
            return;

        default:
            assert(0);
    }
    return;
}

// TODO handle 16 bit cases
void check_opcode(unsigned char * inst, int operand_override, int address_override, int rex){
    assert(inst != NULL);
    u_int8_t opcode = inst[0]; 

    if(opcode == 0xf){
        check_second_opcode(&inst[1], operand_override, address_override, rex);
        return;
    }

    int operand_size = find_operand_size(operand_override, rex);
    int address_size = find_address_size(operand_override);

    switch(opcode){
        // TODO verify that all these instructions handle rex byte correctly
        case OP_MOV_8C:
        case OP_MOV_8E:
            // TODO handle sregister (see opcode.h)
            return;

        // 8 bit operand case - rm (dest), reg
        case OP_ADD_00:
        case OP_MOV_88:
            check_modrm_rm(&inst[1], 8, address_size, rex);
            check_modrm_reg(&inst[1], 8, address_size, rex);
            return;

        // 8 bit operand case - reg (dest), rm
        case OP_ADD_02:
        case OP_MOV_8A:
            check_modrm_reg(&inst[1], 8, address_size, rex);
            check_modrm_rm(&inst[1], 8, address_size, rex);
            return;

        // Normal - rm (dest), reg 
        case OP_ADD_01:
        case OP_MOV_89:
            check_modrm_rm(&inst[1], operand_size, address_size, rex);
            check_modrm_reg(&inst[1], operand_size, address_size, rex);
            return;

        // Normal - reg (dest), rm 
        case OP_ADD_03:
        case OP_LEA:
        case OP_MOV_8B:
            check_modrm_reg(&inst[1], operand_size, address_size, rex);
            check_modrm_rm(&inst[1], operand_size, address_size, rex);
            return;

//        case OP_SUB_83:   // 83 \5 (use r/m not reg of modrm for register) and ib (immediate_8 byte)
            // TODO combine with add_83 
           return;
        // rm (dest), imm8 
        case OP_ADD_83:
            check_modrm_rm(&inst[1], operand_size, address_size, rex);
            get_immediate(&inst[2], 8);
            return;

        // rm (dest), imm16,32
        case OP_ADD_81:
            check_modrm_rm(&inst[1], operand_size, address_size, rex);
            if(operand_size == 64)
                get_immediate(&inst[2], 32);
            else
                get_immediate(&inst[2], operand_size);
            return;
            
        // rm8 (dest), imm8
        case OP_ADD_80:
            check_modrm_rm(&inst[1], 8, address_size, rex);
            get_immediate(&inst[2], 8);
            return;

        // ax,eax,rax imm16,32
        case OP_ADD_05:
            get_register(0, operand_size, rex);
            if(operand_size == 64)
                get_immediate(&inst[1], 32);
            else
                get_immediate(&inst[1], operand_size);
            return;
        
        // AL imm8
        case OP_ADD_04:
            get_register(0, 8, 0); // AL
            get_immediate(&inst[1], 8);
            return;

        case OP_OR:
        case OP_SUB_2B:
        case OP_TEST_85:
            // TODO handle checkmodrm and override prefixes
            return;
        case OP_CALL_E8:
            printf("disp 0x%lx\n", get_displacement(&inst[1], 64, 5)); // 32 bit disp but sign extended
            return;
        case 0xFF: // Could be call(f) jmp(f) inc dec or push
            switch(inst[1] & 0x38 >> 3){
                case 2: // Call
                    check_modrm_rm(&inst[1], 64, address_size, rex);
                    return;
                case 3: // Call
                    ;//TODO handle this weird sreg case
                    return;
                case 4: // Jmp
                    check_modrm_rm(&inst[1], 64, address_size, rex);
                    return; 
                case 5: // Jmp
                    ;//TODO handle this weird sreg case
                default:
                    assert(0);
            }
            return;

        case OP_JMP_EB:
            printf("memory access [rip+0x%lx]\n", get_displacement(&inst[1], 8, 2));
            return;
        case OP_JMP_E9:
            printf("memory access [rip+0x%lx]\n", get_displacement(&inst[1], 32, 2));
            return;


        case OP_SHL:
            return;
    }

    // Opcodes whose last 3 bits are for one register
    opcode = opcode & 0xf8;
    switch(opcode){
        case OP_POP:
        case OP_PUSH:
            if(DEFAULT_BIT_MODE == 64)
                printf("memory access [rsp]\n"); 
            else if(DEFAULT_BIT_MODE == 32)
                printf("memory access [esp]\n"); 
            else if(DEFAULT_BIT_MODE == 16)
                printf("memory access [sp]\n"); 
                
            return;
        default:
            assert(0);
            return;
    }
    return;
}
