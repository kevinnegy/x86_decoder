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
        case OP_NOP_1F: 
            return; 

        case OP_POP_A1:
        case OP_POP_A9:
        case OP_PUSH_A0:
        case OP_PUSH_A8:
            //TODO handle FS/GS regster
            printf("memory access [rsp]\n"); 
            return;

        case OP_CMOV_44:
            check_modrm_rm(&inst[1], operand_size, address_size, rex);
            check_modrm_reg(&inst[1], operand_size, address_size, rex);
            return;

        case OP_MOVSX_BE:
            check_modrm_reg(&inst[1], operand_size, address_size, rex);
            check_modrm_rm(&inst[1], 8, address_size, rex);
            return;

        case OP_MOVSX_BF:
            assert(operand_size != 16);
            check_modrm_reg(&inst[1], operand_size, address_size, rex);
            check_modrm_rm(&inst[1], 16, address_size, rex);
            return;

        case OP_RDTSC:
            return;

    }
    if((opcode & 0x80) == 0x80){
        switch(opcode & 0xf){
            case 0: // JO (0F = 1)
            case 1: // JNO (0F = 0)
            case 2: // JB/JC/JNAE (CF = 1)
            case 3: // JAE/JNB/JNC (CF = 0)
            case 4: // JE/JZ (ZF = 1)
            case 5: // JNZ (ZF = 0) 
            case 6: // JBE/JNA (CF = 1 or ZF = 1)
            case 7: // JA/JNBE (if CF = 0 and ZF = 0)
            case 8: // JS (SF = 1) 
            case 9: // JNS (SF = 0) 
            case 0xa: // JP/JPE (PF = 1) 
            case 0xb: // JNP/JPO (PF = 0)
            case 0xc: // JL (SF != OF)
            case 0xd: // JGE/JNL (SF = OF)
            case 0xf: // JG/JNLE (ZF = 0 and SF = OF)
            case 0xe: // JGE/JNG (ZF = 1 or SF != OF)
                printf("Conditional displacement! 0x%lx\n", get_displacement(&inst[1], 32, 6)); // 6bytes/inst
                return;
        }
    }

    assert(0);
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
        case OP_NOP_90:
            return;

        case OP_MOV_A0: case OP_MOV_A1: case OP_MOV_A2: case OP_MOV_A3:
        case OP_MOV_8C:
        case OP_MOV_8E:
            // TODO handle sregister (see opcode.h)
            assert(0);
            return;

        case OP_TEST_84:
            // Test does no memory writing, skip calls 
            //check_modrm_rm(&inst[1], 8, address_size, rex);
            //check_modrm_reg(&inst[1], 8, address_size, rex);
            return;

        case OP_TEST_85:
            // Test does no memory writing, skip calls 
            //check_modrm_reg(&inst[1], 8, address_size, rex);
            //check_modrm_rm(&inst[1], 8, address_size, rex);
            return;

        // 8 bit operand case - rm (dest), reg
        case OP_ADD_00:
        case OP_AND_20:
        case OP_CMP_38:
        case OP_MOV_88:
        case OP_OR_08:
        case OP_SUB_28:
        case OP_XOR_30:
            check_modrm_rm(&inst[1], 8, address_size, rex);
            check_modrm_reg(&inst[1], 8, address_size, rex);
            return;

        // 8 bit operand case - reg (dest), rm
        case OP_ADD_02:
        case OP_AND_22:
        case OP_CMP_3A:
        case OP_MOV_8A:
        case OP_OR_0A:
        case OP_SUB_2A:
        case OP_XOR_32:
            check_modrm_reg(&inst[1], 8, address_size, rex);
            check_modrm_rm(&inst[1], 8, address_size, rex);
            return;

        // Normal - rm (dest), reg 
        case OP_ADD_01:
        case OP_AND_21:
        case OP_CMP_39:
        case OP_MOV_89:
        case OP_OR_09:
        case OP_SUB_29:
        case OP_XOR_31:
            check_modrm_rm(&inst[1], operand_size, address_size, rex);
            check_modrm_reg(&inst[1], operand_size, address_size, rex);
            return;

        // Normal - reg (dest), rm 
        case OP_ADD_03:
        case OP_AND_23:
        case OP_CMP_3B:
        case OP_LEA:
        case OP_MOV_8B:
        case OP_MOVSX_63:
        case OP_OR_0B:
        case OP_SUB_2B:
        case OP_XOR_33:
            check_modrm_reg(&inst[1], operand_size, address_size, rex);
            check_modrm_rm(&inst[1], operand_size, address_size, rex);
            return;


        // ax,eax,rax imm16,32
        case OP_ADD_05:
        case OP_AND_25:
        case OP_CMP_3D:
        case OP_OR_0D:
        case OP_SUB_2D:
        case OP_TEST_A9:
        case OP_XOR_35:
            get_register(0, operand_size, rex);
            if(operand_size == 64)
                get_immediate(&inst[1], 32);
            else
                get_immediate(&inst[1], operand_size);
            return;
        
        // AL imm8
        case OP_ADD_04:
        case OP_AND_24:
        case OP_CMP_3C:
        case OP_OR_0C:
        case OP_SUB_2C:
        case OP_TEST_A8:
        case OP_XOR_34:
            get_register(0, 8, 0); // AL
            get_immediate(&inst[1], 8);
            return;

        case OP_CALL_E8:
            printf("disp 0x%lx\n", get_displacement(&inst[1], 64, 5)); // 32 bit disp but sign extended
            return;

        // rm8 (dest), imm8
        case OP_MOV_C6:
            check_modrm_rm(&inst[1], 8, address_size, rex);
            get_immediate(&inst[2], 8);
            return;

        // rm8 (dest), imm8
        case 0x80:
            switch((inst[1] & 0x38) >> 3){
                case 0: // Add
                case 1: // Or
                case 4: // AND
                case 5: // Sub
                case 6: // XOR 
                case 7: // CMP
                    check_modrm_rm(&inst[1], 8, address_size, rex);
                    get_immediate(&inst[2], 8);
                    return;
                default:
                    assert(0);
            }
            return;

        // 16,32,64 rm and 16,32 immediate
        case 0x81:
            switch((inst[1] & 0x38) >> 3){
                case 0: // Add
                case 1: // Or
                case 4: // AND
                case 5: // Sub
                case 6: // XOR 
                case 7: // CMP
                    check_modrm_rm(&inst[1], operand_size, address_size, rex);
                    if(operand_size == 64)
                        get_immediate(&inst[2], 32);
                    else
                        get_immediate(&inst[2], operand_size);
                default:
                    assert(0);
            }
            return;

        // 16,32,64 rm and 16,32 immediate
        case OP_MOV_C7:
            check_modrm_rm(&inst[1], operand_size, address_size, rex);
            if(operand_size == 64)
                get_immediate(&inst[2], 32);
            else
                get_immediate(&inst[2], operand_size);
            return;
            
        case 0x83:
            switch((inst[1] & 0x38) >> 3){
                case 0: // Add
                case 1: // Or
                case 4: // AND
                case 5: // Sub
                case 6: // XOR 
                case 7: // CMP
                    check_modrm_rm(&inst[1], operand_size, address_size, rex);
                    get_immediate(&inst[2], 8);
                    return;
                default:
                    assert(0);
            }
            return;

        case 0x8F:
            switch((inst[1] & 0x38) >> 3){
                case 0: // Push
                    if(operand_size == 32)
                        operand_size = 64;
                    check_modrm_rm(&inst[1], operand_size, address_size, rex);
                    printf("memory access [rsp]\n"); 
                    return;
                default:
                    assert(0);
            }
            return;

        case 0xC0:
            switch((inst[1] & 0x38) >> 3){
                case 4: // SAL/SHL rm8 imm8 
                case 5: // SHR
                case 7: // SAR
                    check_modrm_rm(&inst[1], 8, address_size, rex);
                    get_immediate(&inst[2], 8);
                    return;
                default:
                    assert(0);
            }
            return;

        case 0xC1:
            switch((inst[1] & 0x38) >> 3){
                case 4: // SAL/SHL imm8 
                case 5: // SHR
                case 7: // SAR
                    check_modrm_rm(&inst[1], operand_size, address_size, rex);
                    get_immediate(&inst[2], 8);
                    return;
                default:
                    assert(0);
            }
            return;

        case 0xD0:
            switch((inst[1] & 0x38) >> 3){
                case 4: // SAL/SHL 1 time
                case 5: // SHR
                case 7: // SAR 1 time
                    check_modrm_rm(&inst[1], 8, address_size, rex);
                    return;
                default:
                    assert(0);
            }
            return;

        case 0xD1:
            switch((inst[1] & 0x38) >> 3){
                case 4: // SAL/SHL 1 time
                case 5: // SHR
                case 7: // SAR
                    check_modrm_rm(&inst[1], operand_size, address_size, rex);
                    return;
                default:
                    assert(0);
            }
            return;


        case 0xD2:
            switch((inst[1] & 0x38) >> 3){
                case 4: // SAL/SHL CL number of times
                case 5: // SHR
                case 7: // SAR
                    check_modrm_rm(&inst[1], 8, address_size, rex);
                    return;
                default:
                    assert(0);
            }
            return;

         case 0xD3:
            switch((inst[1] & 0x38) >> 3){
                case 4: // SAL/SHL CL number of times
                case 5: // SHR
                case 7: // SAR
                    check_modrm_rm(&inst[1], operand_size, address_size, rex);
                    return;
                default:
                    assert(0);
            }
            return;
            

        case 0xF6:
            switch((inst[1] & 0x38) >> 3){
                case 0: // Test 
                    // Test does no memory writing, skip modrm_rm call 
                    //check_modrm_rm(&inst[1], 8, address_size, rex);
                    get_immediate(&inst[2], 8);
                    return;
                case 3: // Neg rm
                    check_modrm_rm(&inst[1], 8, address_size, rex);
                    return;
                default:
                    assert(0);
            }
            return;

        case 0xF7:
            switch((inst[1] & 0x38) >> 3){
                case 0: // Test
                    // Test does no memory writing, skip modrm_rm call 
                    //check_modrm_rm(&inst[1], operand_size, address_size, rex);
                    if(operand_size == 64)
                        get_immediate(&inst[2], 32);
                    else
                        get_immediate(&inst[2], operand_size);
                    return;
                case 3:
                    check_modrm_rm(&inst[1], operand_size, address_size, rex);
                    return;
                default:
                    assert(0);
            }
            return;


        // rm (dest), imm16,32
        case 0xFF: // Could be call(f) jmp(f) inc dec or push
            switch((inst[1] & 0x38) >> 3){
                case 2: // Call
                    check_modrm_rm(&inst[1], 64, address_size, rex);
                    return;
                case 3: // Call
                    assert(0);//TODO handle this weird sreg case
                    return;
                case 4: // Jmp
                    check_modrm_rm(&inst[1], 64, address_size, rex);
                    return; 
                case 5: // Jmp
                    ;//TODO handle this weird sreg case
                case 6: // Pop
                    if(operand_size == 32)
                        operand_size = 64;
                    check_modrm_rm(&inst[1], operand_size, address_size, rex);
                    printf("memory access [rsp]\n"); 
                    return;
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

        case OP_PUSH_6A:
            get_immediate(&inst[1], 8);
            printf("memory access [rsp]\n"); 
            return;
            
        case OP_PUSH_68:
            get_immediate(&inst[1], operand_size);
            printf("memory access [rsp]\n"); 
            return;

        case OP_RET_C3:
        case OP_RET_CB:
            printf("RETURN STATEMENT");
            return;

        case OP_RET_C2:
        case OP_RET_CA:
            get_immediate(&inst[1], 16);
            printf("RETURN STATEMENT");
            return;
    }

    // Opcodes whose last 3 bits are for one register
    switch(opcode & 0xf8){
        case OP_MOV_B0:
            get_register(opcode & 0x7, 8, rex); 
            get_immediate(&inst[1], 8);
            return;

        case OP_MOV_B8:
            get_register(opcode & 0x7, operand_size, rex); 
            get_immediate(&inst[1], 8);
            return;

        case OP_POP_58:
        case OP_PUSH_50:
            if(operand_size == 32)
                operand_size = 64;
            get_register(opcode & 0x7, operand_size, 0);
            printf("memory access [rsp]\n"); 
            return;
    }

    // JCC instructions
    if((opcode & 0x70) == 0x70){
        switch(opcode & 0xf){
            case 0: // JO (0F = 1)
            case 1: // JNO (0F = 0)
            case 2: // JB/JC/JNAE (CF = 1)
            case 3: // JAE/JNB/JNC (CF = 0)
            case 4: // JE/JZ (ZF = 1)
            case 5: // JNZ (ZF = 0) 
            case 6: // JBE/JNA (CF = 1 or ZF = 1)
            case 7: // JA/JNBE (if CF = 0 and ZF = 0)
            case 8: // JS (SF = 1) 
            case 9: // JNS (SF = 0) 
            case 0xa: // JP/JPE (PF = 1) 
            case 0xb: // JNP/JPO (PF = 0)
            case 0xc: // JL (SF != OF)
            case 0xd: // JGE/JNL (SF = OF)
            case 0xf: // JG/JNLE (ZF = 0 and SF = OF)
            case 0xe: // JGE/JNG (ZF = 1 or SF != OF)
                printf("Conditional displacement! 0x%lx\n", get_displacement(&inst[1], 8, 2)); // TODO check if instruction length matters here
            return;
        }
    }

    else if(opcode == OP_JCC_E3){
        // TODO handle operand size defaulting to 64 bit without rex 
        printf("Conditional displacement! 0x%lx\n", get_displacement(&inst[1], operand_size, 0)); // TODO check if instruction length matters here
        return;
    }

    assert(0);
    return;
}
