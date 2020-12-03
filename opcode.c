#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "opcode.h"
#include "prefix.h"
#include "x86_decoder.h"
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

void check_third_opcode(unsigned char * inst, struct prefixes * prfx){
    assert(inst != NULL);
    int opcode = inst[0]; 
    int operand_override = prfx->OPERAND_OVERRIDE;
    int address_override = prfx->ADDRESS_OVERRIDE;
    int rex = prfx->REX;

    int operand_size = find_operand_size(operand_override, rex);
    int address_size = find_address_size(operand_override);
    switch(opcode){
        case OP_MOVBE_F0:
            assert((inst[1] & 0xc0) != 0xc0); // Rm must be a memory location
            check_modrm_rm(&inst[1], operand_size, address_size, prfx);
            check_modrm_reg(&inst[1], operand_size, address_size, prfx);
            return;

        case OP_MOVBE_F1:
            assert((inst[1] & 0xc0) != 0xc0); // Rm must be a memory location
            check_modrm_reg(&inst[1], operand_size, address_size, prfx);
            check_modrm_rm(&inst[1], operand_size, address_size, prfx);
            return; 

        case OP_PALIGNR_0F:
            if(operand_override == 1){ // 66 prefix
                check_modrm_reg(&inst[1], 4, address_size, prfx); // operand_size should be 4 for the xmm regs
                check_modrm_rm(&inst[1], 4, address_size, prfx);
            }
            else{
                check_modrm_reg(&inst[1], 3, address_size, prfx); // operand_size should be 3 for the mm regs
                check_modrm_rm(&inst[1], 3, address_size, prfx);
            }
            get_immediate(&inst[2], 8);
            return;

        case OP_PMAXUB_3E:
        case OP_PMINUB_3A:
            assert(operand_override == 1);
            check_modrm_reg(&inst[1], 4, address_size, prfx); // operand_size should be 4 for the xmm regs
            check_modrm_rm(&inst[1], 4, address_size, prfx);
            return;

        case OP_PCMPISTRI_63:
            assert(operand_override == 1);
            check_modrm_reg(&inst[1], 4, address_size, prfx); // operand_size should be 4 for the xmm regs
            check_modrm_rm(&inst[1], 4, address_size, prfx);
            get_immediate(&inst[2], 8);
            return;
            
    }

    assert(0); 
    return;
}

void check_second_opcode(unsigned char * inst, struct prefixes * prfx){
    assert(inst != NULL);

    int opcode = inst[0];
    
    if(opcode == 0x38 || opcode == 0x3a){
        check_third_opcode(&inst[1], prfx);
        return;
    }

    int operand_override = prfx->OPERAND_OVERRIDE;
    int address_override = prfx->ADDRESS_OVERRIDE;
    int rex = prfx->REX;

    int operand_size = find_operand_size(prfx->OPERAND_OVERRIDE, rex);
    int address_size = find_address_size(prfx->OPERAND_OVERRIDE);
    switch(opcode){
        case OP_CPUID_A2:
        case OP_NOP_1F: 
        case OP_SYSCALL_05:
        case OP_XGETBV_01:
            return; 

        case OP_POP_A1:
        case OP_POP_A9:
        case OP_PUSH_A0:
        case OP_PUSH_A8:
            //TODO handle FS/GS regster
            printf("memory access [rsp]\n"); 
            return;
        
        case OP_PADD_FC:
        case OP_PADD_FD:
        case OP_PADD_FE:
        case OP_PADD_D4:
        case OP_PCMPGT_64:
        case OP_PCMPGT_65:
        case OP_PCMPGT_66:
        case OP_PCMPEQ_74:
        case OP_PCMPEQ_75:
        case OP_PCMPEQ_76:
        case OP_PMAXUB_DE:
        case OP_PMINUB_DA:
        case OP_POR_EB:
        case OP_PSLL_F1:
        case OP_PSLL_F2:
        case OP_PSLL_F3:
        case OP_PSUBB_F8:
        case OP_PSUBW_F9:
        case OP_PSUBD_FA:
        case OP_PUNPCKH_68:
        case OP_PUNPCKH_69:
        case OP_PUNPCKH_6A:
        case OP_PXOR_EF:
            if(operand_override == 1){ // 66 prefix
                check_modrm_reg(&inst[1], 4, address_size, prfx); // operand_size should be 4 for the xmm regs
                check_modrm_rm(&inst[1], 4, address_size, prfx);
            }
            else{
                check_modrm_reg(&inst[1], 3, address_size, prfx); // operand_size should be 3 for the mm regs
                check_modrm_rm(&inst[1], 3, address_size, prfx);
            }
            return;

        case OP_PUNPCKH_6D:
            assert(operand_override == 1);
            check_modrm_reg(&inst[1], 4, address_size, prfx); // operand_size should be 4 for the xmm regs
            check_modrm_rm(&inst[1], 4, address_size, prfx);
            return;

        case OP_CMPXCHG_B0:
        case OP_XADD_C0:
            check_modrm_rm(&inst[1], 8, address_size, prfx);
            check_modrm_reg(&inst[1], 8, address_size, prfx);
            return;
            
        case OP_CMPXCHG_B1:
        case OP_XADD_C1:
            check_modrm_rm(&inst[1], operand_size, address_size, prfx);
            check_modrm_reg(&inst[1], operand_size, address_size, prfx);
            return;
    
        case OP_MOVD_6E:
            if(operand_override == 1) // 66H
                check_modrm_reg(&inst[1], 4, address_size, prfx); // operand_size should be 4 for the xmm regs
            else
                check_modrm_reg(&inst[1], 3, address_size, prfx); // operand_size should be 3 for the mm regs
            check_modrm_rm(&inst[1], operand_size, address_size, prfx);
            return;

        case OP_MOVD_7E:
            check_modrm_rm(&inst[1], operand_size, address_size, prfx);
            if(operand_override == 1) // 66H
                check_modrm_reg(&inst[1], 4, address_size, prfx); // operand_size should be 4 for the xmm regs
            else
                check_modrm_reg(&inst[1], 3, address_size, prfx); // operand_size should be 3 for the mm regs
            return;

        case OP_MOVAPS_28:
        case OP_MOVDQU_6F:
        case OP_MOVUPS_10:
            check_modrm_reg(&inst[1], 4, address_size, prfx); // operand_size should be 4 for the xmm regs
            check_modrm_rm(&inst[1], 4, address_size, prfx);
            return;

        case OP_MOVAPS_29:
        case OP_MOVQ_D6:
        case OP_MOVDQU_7F:
        case OP_MOVUPS_11:
            check_modrm_rm(&inst[1], 4, address_size, prfx);
            check_modrm_reg(&inst[1], 4, address_size, prfx); // operand_size should be 4 for the xmm regs
            return;

        case OP_MOVHPD_16:
        case OP_MOVLPD_12:
            check_modrm_reg(&inst[1], 4, address_size, prfx); // operand_size should be 4 for the xmm regs
            check_modrm_rm(&inst[1], 64, address_size, prfx);
            return;
            
        case OP_PMOVMSK_D7:
            check_modrm_reg(&inst[1], 64, address_size, prfx); // operand_size should be 4 for the xmm regs
            if(operand_override == 1){ // 66 prefix
                check_modrm_rm(&inst[1], 4, address_size, prfx);
            }
            else{
                check_modrm_rm(&inst[1], 3, address_size, prfx);
            }
            return;

        case OP_PSHUFD_70: 
            assert(operand_override == 1);
            check_modrm_reg(&inst[1], 4, address_size, prfx); // operand_size should be 4 for the xmm regs
            check_modrm_rm(&inst[1], 4, address_size, prfx);
            get_immediate(&inst[2], 8);
            return;
        
        case OP_PUNPCK_60:
        case OP_PUNPCK_62:
            if(operand_override == 1){ // 66 prefix
                check_modrm_reg(&inst[1], 4, address_size, prfx); // operand_size should be 4 for the xmm regs
                check_modrm_rm(&inst[1], 4, address_size, prfx);
            }
            else{
                check_modrm_reg(&inst[1], 3, address_size, prfx); // operand_size should be 3 for the mm regs
                check_modrm_rm(&inst[1], 3, address_size, prfx);
            }
            return;

        case OP_PUNPCK_61:
            if(operand_override == 1){ // 66 prefix
                check_modrm_rm(&inst[1], 4, address_size, prfx);
                check_modrm_reg(&inst[1], 4, address_size, prfx); // operand_size should be 4 for the xmm regs
            }
            else{
                check_modrm_rm(&inst[1], 3, address_size, prfx);
                check_modrm_reg(&inst[1], 3, address_size, prfx); // operand_size should be 3 for the mm regs
            }
            return;
            
        case OP_PUNPCK_6C:
            check_modrm_reg(&inst[1], 4, address_size, prfx); // operand_size should be 4 for the xmm regs
            check_modrm_rm(&inst[1], 4, address_size, prfx);
            return;

        case 0x71:
            switch((inst[1] & 0x38) >> 3){
                case 6: // PSLL
                    if(operand_override == 1){ // 66 prefix
                        check_modrm_rm(&inst[1], 4, address_size, prfx);
                        check_modrm_reg(&inst[1], 4, address_size, prfx); // operand_size should be 4 for the xmm regs
                    }
                    else{
                        check_modrm_rm(&inst[1], 3, address_size, prfx);
                        check_modrm_reg(&inst[1], 3, address_size, prfx); // operand_size should be 3 for the mm regs
                    }
                    return;
                default:
                    assert(0);
            }
            return;

        case 0x72:
            switch((inst[1] & 0x38) >> 3){
                case 6: // PSLL
                    if(operand_override == 1){ // 66 prefix
                        check_modrm_rm(&inst[1], 4, address_size, prfx);
                        check_modrm_reg(&inst[1], 4, address_size, prfx); // operand_size should be 4 for the xmm regs
                    }
                    else{
                        check_modrm_rm(&inst[1], 3, address_size, prfx);
                        check_modrm_reg(&inst[1], 3, address_size, prfx); // operand_size should be 3 for the mm regs
                    }
                    return;
                default:
                    assert(0);
            }
            return;

        case 0x73:
            switch((inst[1] & 0x38) >> 3){
                case 6: //PSLL
                    if(operand_override == 1)
                        check_modrm_reg(&inst[1], 4, address_size, prfx); // operand_size should be 4 for the xmm regs
                    else
                        check_modrm_reg(&inst[1], 3, address_size, prfx); // operand_size should be 3 for the mm regs
                    get_immediate(&inst[2], 8);

                case 7: // PSLLDQ
                    check_modrm_reg(&inst[1], 4, address_size, prfx); // operand_size should be 4 for the xmm regs
                    get_immediate(&inst[2], 8);
                    return;
            }
            return;

        case OP_BT_BA:
            check_modrm_rm(&inst[1], operand_size, address_size, prfx);
            get_immediate(&inst[2], 8);
            return;

        case OP_BT_A3:
            check_modrm_rm(&inst[1], operand_size, address_size, prfx);
            check_modrm_reg(&inst[1], operand_size, address_size, prfx);
            return;

        // Normal rm->reg
        case OP_BSF_BC:
        case OP_IMUL_AF:
            check_modrm_reg(&inst[1], operand_size, address_size, prfx);
            check_modrm_rm(&inst[1], operand_size, address_size, prfx);
            return;

        case OP_MOVSX_BE:
        case OP_MOVZX_B6:
            check_modrm_reg(&inst[1], operand_size, address_size, prfx);
            check_modrm_rm(&inst[1], 8, address_size, prfx);
            return;

        case OP_MOVSX_BF:
        case OP_MOVZX_B7:
            assert(operand_size != 16);
            check_modrm_reg(&inst[1], operand_size, address_size, prfx);
            check_modrm_rm(&inst[1], 16, address_size, prfx);
            return;

        case 0xAE:
            switch((inst[1] & 0x38) >> 3){
                case 5: // xrstor
                    assert(operand_size != 16);
                    check_modrm_rm(&inst[1], operand_size, address_size, prfx);
                    return;
                default:
                    assert(0);
            }
            return;

        case 0xC7:
            switch((inst[1] & 0x38) >> 3){
                case 4: // xsavec 
                    assert(operand_size != 16);
                    check_modrm_rm(&inst[1], operand_size, address_size, prfx);
                    return;
                default:
                    assert(0);
            }
            return;
                    

        case OP_RDTSC:
            return;

    }

    if((opcode & 0xF0) == 0x40){
        switch(opcode & 0xf){
            case 0: // CMOVO (0F = 1)
            case 1: // CMOVNO (0F = 0)
            case 2: // CMOVB/CMOVC/CMOVNAE (CF = 1)
            case 3: // CMOVAE/CMOVNB/CMOVNC (CF = 0)
            case 4: // CMOVE/CMOVZ (ZF = 1)
            case 5: // CMOVNE/CMOVNZ (ZF = 0) 
            case 6: // CMOVBE/CMOVNA (CF = 1 or ZF = 1)
            case 7: // CMOVA/CMOVNBE (if CF = 0 and ZF = 0)
            case 8: // CMOVS (SF = 1) 
            case 9: // CMOVNS (SF = 0) 
            case 0xa: // CMOVP/CMOVPE (PF = 1) 
            case 0xb: // CMOVNP/CMOVPO (PF = 0)
            case 0xc: // CMOVL (SF != OF)
            case 0xd: // CMOVGE/CMOVNL (SF = OF)
            case 0xf: // CMOVG/CMOVNLE (ZF = 0 and SF = OF)
            case 0xe: // CMOVGE/CMOVLE/CMOVNG (ZF = 1 or SF != OF)
                check_modrm_reg(&inst[1], operand_size, address_size, prfx);
                check_modrm_rm(&inst[1], operand_size, address_size, prfx);
                return;
        }
    }

    if((opcode & 0xF0) == 0x80){
        switch(opcode & 0xf){
            case 0: // JO (0F = 1)
            case 1: // JNO (0F = 0)
            case 2: // JB/JC/JNAE (CF = 1)
            case 3: // JAE/JNB/JNC (CF = 0)
            case 4: // JE/JZ (ZF = 1)
            case 5: // JNE/JNZ (ZF = 0) 
            case 6: // JBE/JNA (CF = 1 or ZF = 1)
            case 7: // JA/JNBE (if CF = 0 and ZF = 0)
            case 8: // JS (SF = 1) 
            case 9: // JNS (SF = 0) 
            case 0xa: // JP/JPE (PF = 1) 
            case 0xb: // JNP/JPO (PF = 0)
            case 0xc: // JL (SF != OF)
            case 0xd: // JGE/JNL (SF = OF)
            case 0xf: // JG/JNLE (ZF = 0 and SF = OF)
            case 0xe: // JGE/JLE/JNG (ZF = 1 or SF != OF)
                printf("Conditional displacement! 0x%lx\n", get_displacement(&inst[1], 32, 6)); // 6bytes/inst
                return;
        }
    }

    if((opcode & 0xF0) == 0x90){
        switch(opcode & 0xf){
            case 0: // SETO (0F = 1)
            case 1: // SETNO (0F = 0)
            case 2: // SETB/SETC/SETNAE (CF = 1)
            case 3: // SETAE/SETNB/SETNC (CF = 0)
            case 4: // SETE/SETZ (ZF = 1)
            case 5: // SETNZ/SETNE (ZF = 0) 
            case 6: // SETBE/SETNA (CF = 1 or ZF = 1)
            case 7: // SETA/SETNBE (if CF = 0 and ZF = 0)
            case 8: // SETS (SF = 1) 
            case 9: // SETNS (SF = 0) 
            case 0xa: // SETP/SETPE (PF = 1) 
            case 0xb: // SETNP/SETPO (PF = 0)
            case 0xc: // SETL/SETNGE (SF != OF)
            case 0xd: // SETGE/SETNL (SF = OF)
            case 0xf: // SETG/SETNLE (ZF = 0 and SF = OF)
            case 0xe: // SETGE/SETLE/SETNG (ZF = 1 or SF != OF)
                check_modrm_rm(&inst[1], 8, 8, prfx);
                return;
        }
    }

    assert(0);
    return;
}

// TODO handle 16 bit cases
void check_opcode(unsigned char * inst, struct prefixes * prfx){
    assert(inst != NULL);
    u_int8_t opcode = inst[0]; 

    if(opcode == 0xf){
        check_second_opcode(&inst[1], prfx);
        return;
    }
    else if (opcode == 0xc4 || opcode == 0xc5){
        return;
    }

    int operand_override = prfx->OPERAND_OVERRIDE;
    int address_override = prfx->ADDRESS_OVERRIDE;
    int rex = prfx->REX;

    int operand_size = find_operand_size(operand_override, rex);
    int address_size = find_address_size(operand_override);

    switch(opcode){
        case OP_NOP_90:
        case OP_CONVERT_99:
        case OP_CMPS_A6:
        case OP_CMPS_A7:
            return;

        case OP_MOV_A0: case OP_MOV_A1: case OP_MOV_A2: case OP_MOV_A3:
        case OP_MOV_8C:
        case OP_MOV_8E:
            // TODO handle sregister (see opcode.h)
            assert(0);
            return;

        case OP_TEST_84:
            // Test does no memory writing, skip calls 
            //check_modrm_rm(&inst[1], 8, address_size, prfx);
            //check_modrm_reg(&inst[1], 8, address_size, prfx);
            return;

        case OP_TEST_85:
            // Test does no memory writing, skip calls 
            //check_modrm_reg(&inst[1], 8, address_size, prfx);
            //check_modrm_rm(&inst[1], 8, address_size, prfx);
            return;

        // 8 bit operand case - rm (dest), reg
        case OP_ADD_00:
        case OP_AND_20:
        case OP_CMP_38:
        case OP_MOV_88:
        case OP_OR_08:
        case OP_SBB_18:
        case OP_SUB_28:
        case OP_XOR_30:
            check_modrm_rm(&inst[1], 8, address_size, prfx);
            check_modrm_reg(&inst[1], 8, address_size, prfx);
            return;

        // 8 bit operand case - reg (dest), rm
        case OP_ADD_02:
        case OP_AND_22:
        case OP_CMP_3A:
        case OP_MOV_8A:
        case OP_OR_0A:
        case OP_SBB_1A:
        case OP_SUB_2A:
        case OP_XCHG_86: // Could go in either normal 8 bit
        case OP_XOR_32:
            check_modrm_reg(&inst[1], 8, address_size, prfx);
            check_modrm_rm(&inst[1], 8, address_size, prfx);
            return;

        // Normal - rm (dest), reg 
        case OP_ADD_01:
        case OP_AND_21:
        case OP_CMP_39:
        case OP_MOV_89:
        case OP_OR_09:
        case OP_SBB_19:
        case OP_SUB_29:
        case OP_XCHG_87: // Could go in either normal reg/rm slot
        case OP_XOR_31:
            check_modrm_rm(&inst[1], operand_size, address_size, prfx);
            check_modrm_reg(&inst[1], operand_size, address_size, prfx);
            return;

        // Normal - reg (dest), rm 
        case OP_ADD_03:
        case OP_AND_23:
        case OP_CMP_3B:
        case OP_LEA:
        case OP_MOV_8B:
        case OP_MOVSX_63:
        case OP_OR_0B:
        case OP_SBB_1B:
        case OP_SUB_2B:
        case OP_XOR_33:
            check_modrm_reg(&inst[1], operand_size, address_size, prfx);
            check_modrm_rm(&inst[1], operand_size, address_size, prfx);
            return;

        // ax,eax,rax imm16,32
        case OP_ADD_05:
        case OP_AND_25:
        case OP_CMP_3D:
        case OP_OR_0D:
        case OP_SBB_1D:
        case OP_SUB_2D:
        case OP_TEST_A9:
        case OP_XOR_35:
            get_register(0, operand_size, prfx);
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
        case OP_SBB_1C:
        case OP_SUB_2C:
        case OP_TEST_A8:
        case OP_XOR_34:
            get_register(0, 8, prfx); // AL
            get_immediate(&inst[1], 8);
            return;

        case OP_CALL_E8:
            printf("disp 0x%lx\n", get_displacement(&inst[1], 64, 5)); // 32 bit disp but sign extended
            return;

        case OP_LEAVE_C9:
            if(prfx->OPERAND_OVERRIDE == 1)
                printf("pop [bp]");
            else
                printf("pop [rbp]");
            return;


        // rm8 (dest), imm8
        case OP_MOV_C6:
            check_modrm_rm(&inst[1], 8, address_size, prfx);
            get_immediate(&inst[2], 8);
            return;

        // rm8 (dest), imm8
        case 0x80:
            switch((inst[1] & 0x38) >> 3){
                case 0: // Add
                case 1: // Or
                case 3: // Sbb
                case 4: // AND
                case 5: // Sub
                case 6: // XOR 
                case 7: // CMP
                    check_modrm_rm(&inst[1], 8, address_size, prfx);
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
                case 3: // Sbb
                case 4: // AND
                case 5: // Sub
                case 6: // XOR 
                case 7: // CMP
                    check_modrm_rm(&inst[1], operand_size, address_size, prfx);
                    if(operand_size == 64)
                        get_immediate(&inst[2], 32);
                    else
                        get_immediate(&inst[2], operand_size);
                    return;
                default:
                    assert(0);
            }
            return;

        // 16,32,64 rm and 16,32 immediate
        case OP_MOV_C7:
            check_modrm_rm(&inst[1], operand_size, address_size, prfx);
            if(operand_size == 64)
                get_immediate(&inst[2], 32);
            else
                get_immediate(&inst[2], operand_size);
            return;
            
        case 0x83:
            switch((inst[1] & 0x38) >> 3){
                case 0: // Add
                case 1: // Or
                case 3: // Sbb
                case 4: // AND
                case 5: // Sub
                case 6: // XOR 
                case 7: // CMP
                    check_modrm_rm(&inst[1], operand_size, address_size, prfx);
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
                    check_modrm_rm(&inst[1], operand_size, address_size, prfx);
                    printf("memory access [rsp]\n"); 
                    return;
                default:
                    assert(0);
            }
            return;

        case 0xC0:
            switch((inst[1] & 0x38) >> 3){
                case 0: // ROL 
                case 1: // ROR 
                case 2: // RCL 
                case 3: // RCR 
                case 4: // SAL/SHL rm8 imm8 
                case 5: // SHR
                case 7: // SAR
                    check_modrm_rm(&inst[1], 8, address_size, prfx);
                    get_immediate(&inst[2], 8);
                    return;
                default:
                    assert(0);
            }
            return;

        case 0xC1:
            switch((inst[1] & 0x38) >> 3){
                case 0: // ROL 
                case 1: // ROR 
                case 2: // RCL 
                case 3: // RCR 
                case 4: // SAL/SHL imm8 
                case 5: // SHR
                case 7: // SAR
                    check_modrm_rm(&inst[1], operand_size, address_size, prfx);
                    get_immediate(&inst[2], 8);
                    return;
                default:
                    assert(0);
            }
            return;

        case 0xD0:
            switch((inst[1] & 0x38) >> 3){
                case 0: // ROL 
                case 1: // ROR 
                case 2: // RCL 
                case 3: // RCR 
                case 4: // SAL/SHL 1 time
                case 5: // SHR
                case 7: // SAR 1 time
                    check_modrm_rm(&inst[1], 8, address_size, prfx);
                    return;
                default:
                    assert(0);
            }
            return;

        case 0xD1:
            switch((inst[1] & 0x38) >> 3){
                case 0: // ROL 
                case 1: // ROR 
                case 2: // RCL 
                case 3: // RCR 
                case 4: // SAL/SHL 1 time
                case 5: // SHR
                case 7: // SAR
                    check_modrm_rm(&inst[1], operand_size, address_size, prfx);
                    return;
                default:
                    assert(0);
            }
            return;


        case 0xD2:
            switch((inst[1] & 0x38) >> 3){
                case 0: // ROL 
                case 1: // ROR 
                case 2: // RCL 
                case 3: // RCR 
                case 4: // SAL/SHL CL number of times
                case 5: // SHR
                case 7: // SAR
                    check_modrm_rm(&inst[1], 8, address_size, prfx);
                    return;
                default:
                    assert(0);
            }
            return;

         case 0xD3:
            switch((inst[1] & 0x38) >> 3){
                case 0: // ROL 
                case 1: // ROR 
                case 2: // RCL 
                case 3: // RCR 
                case 4: // SAL/SHL CL number of times
                case 5: // SHR
                case 7: // SAR
                    check_modrm_rm(&inst[1], operand_size, address_size, prfx);
                    return;
                default:
                    assert(0);
            }
            return;
            

        case 0xF6:
            switch((inst[1] & 0x38) >> 3){
                case 0: // Test 
                    // Test does no memory writing, skip modrm_rm call 
                    //check_modrm_rm(&inst[1], 8, address_size, prfx);
                    get_immediate(&inst[2], 8);
                    return;
                case 2: // Not
                case 3: // Neg rm
                case 4: // MUL rm
                case 5: // IMUL rm
                case 6: // DIV rm
                case 7: // IDIV rm
                    check_modrm_rm(&inst[1], 8, address_size, prfx);
                    return;
                default:
                    assert(0);
            }
            return;

        case 0xF7:
            switch((inst[1] & 0x38) >> 3){
                case 0: // Test
                    // Test does no memory writing, skip modrm_rm call 
                    //check_modrm_rm(&inst[1], operand_size, address_size, prfx);
                    if(operand_size == 64)
                        get_immediate(&inst[2], 32);
                    else
                        get_immediate(&inst[2], operand_size);
                    return;
                case 2: // Not
                case 3: // Neg
                case 4: // MUL
                case 5: // IMUL
                case 6: // DIV rm
                case 7: // IDIV rm
                    check_modrm_rm(&inst[1], operand_size, address_size, prfx);
                    return;
                default:
                    assert(0);
            }
            return;

        case 0xFE:
            switch((inst[1] & 0x38) >> 3){
                case 0: // INC 
                case 1: // DEC 
                    check_modrm_rm(&inst[1], 8, address_size, prfx);
                    return;
                default:
                    assert(0);
            }
            return;
            

        // rm (dest), imm16,32
        case 0xFF: // Could be call(f) jmp(f) inc dec or push
            switch((inst[1] & 0x38) >> 3){
                case 0: // INC 
                case 1: // DEC 
                    check_modrm_rm(&inst[1], operand_size, address_size, prfx);
                    return;
                case 2: // Call
                    check_modrm_rm(&inst[1], 64, address_size, prfx);
                    return;
                case 3: // Call
                    assert(0);//TODO handle this weird sreg case
                    return;
                case 4: // Jmp
                    check_modrm_rm(&inst[1], 64, address_size, prfx);
                    return; 
                case 5: // Jmp
                    ;//TODO handle this weird sreg case
                case 6: // Push
                    if(operand_size == 32)
                        operand_size = 64;
                    check_modrm_rm(&inst[1], operand_size, address_size, prfx);
                    printf("memory access [rsp]\n"); 
                    return;
                default:
                    return;
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
            printf("RETURN [rsp]");
            return;

        case OP_RET_CB:
            printf("FAR RETURN [rsp]");
            return;

        case OP_RET_C2:
            printf("RETURN [rsp] (read %lld bytes)\n", get_immediate(&inst[1], 16));
            return;

        case OP_RET_CA:
            printf("FAR RETURN [rsp] (read %lld bytes)\n", get_immediate(&inst[1], 16));
            return;

        case OP_STOS_AA:
        case OP_STOS_AB:
            if(operand_size == 64)
                printf("memory access [rdi]\n");
            else
                printf("memory access [edi]\n");
            return;

        case OP_CBW_98:
            return;

        case OP_IMUL_6B:
            check_modrm_reg(&inst[1], operand_size, address_size, prfx);
            check_modrm_rm(&inst[1], operand_size, address_size, prfx);
            get_immediate(&inst[2], 8); 
            return;

        case OP_IMUL_69:
            check_modrm_reg(&inst[1], operand_size, address_size, prfx);
            check_modrm_rm(&inst[1], operand_size, address_size, prfx);
            if(operand_size == 64)
                get_immediate(&inst[2], 32); 
            else
                get_immediate(&inst[2], operand_size);
            return;
    }

    // Opcodes whose last 3 bits are for one register
    switch(opcode & 0xf8){
        case OP_MOV_B0:
            get_register(opcode & 0x7, 8, prfx); 
            get_immediate(&inst[1], 8);
            return;

        case OP_MOV_B8:
            get_register(opcode & 0x7, operand_size, prfx); 
            get_immediate(&inst[1], 8);
            return;

        case OP_POP_58:
        case OP_PUSH_50:
            if(operand_size == 32)
                operand_size = 64;
            get_register(opcode & 0x7, operand_size, 0);
            printf("memory access [rsp]\n"); 
            return;
        
        case OP_XCHG_90:
            check_modrm_rm(&inst[1], operand_size, address_size, prfx);
            return;
    }

    // JCC instructions
    if((opcode & 0xF0) == 0x70){
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
