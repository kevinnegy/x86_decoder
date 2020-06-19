#include "prefix.h"
#include <stdio.h>



void check_prefix(struct x86_instr * inst){
    unsigned char * bytes = inst-> byte_code;
    if(bytes == NULL){
        printf("check_prefix: byte_code is NULL\n");
        return; 
    }
    
    int opcode_flag = 0; // turn on when prefix not found
    int i = 0;
    while(opcode_flag == 0 && i < 15){
        switch(bytes[i]){
        case(prefix_lock):
            break;
        case(prefix_repn):
            // Must also include bnd
            break; 
        case(prefix_rep):
            break; 
        case(prefix_cs):
            // Must include branchn
            break;
        case(prefix_ss):
            break;
        case(prefix_ds):
            // Must inlcude branch
            break;
        case(prefix_es):
            break;
        case(prefix_fs):
            break;
        case(prefix_gs):
            break;
        case(prefix_op_size_override):
            break;
        case(prefix_addr_size_override):
            break;
        case(REX):  
            printf("Reached REX\n");
            inst->rex_ptr = i;
            i++; // REX must come before opcode, check_opcode will generate error if this is not true
        default:
            printf("Reached opcode %2hhx\n", bytes[i]);

            // Set indices for prefixes and opcode 
            if(i != 0) // We found prefixes
                inst->prefix_flag = 1; 
            inst->opcode_ptr = i;

            opcode_flag = 1;
            break;
        }
        if(opcode_flag == 1)
            break;
        i++;
    }
}


// Group 1:
    // FO - LOCK prefix - exclusive use of shared memory in multiprocessor environ
    // F2 - REPNE/REPNZ (repeat not equal/zero) - repeat for each element of string or sometimes mandatory for instructions
    // F3 - REP/REPE/REPZ - repeat (equal/zero) - mandatory for POPCNT, LZCNT, ADOX
    // F2 - BND prefix iff:
            // CPUID.(EAX=07, ECX=0):EBX.MPX[bit 14] is set
            // BNDCFGU.EN and/or IA32_BNDCFGS.EN is set
            // When F2 precedes a near CALL, near RET, near JMP, short Jcc, or near Jcc instruction

// Group 2:
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


//REX-prefix is for 64 bit mode instructions. Only used if instruction operates on 64bit registers. "If rex used when no meaning, it is ignored" (is that a problem?)
// one rex prefix perinstruction, must go right before opcode or escapeopcode byte(0FH) (after any mandatory prefixes)
// one byte REX prefix: 0100WRXB:
#define REX_prefix  (1 << 6)    // 0100 required
#define REX_W       (1 << 3)    // 0 = operand size determined by cs.d, 1 = 64 bit operand size
#define REX_R       (1 << 2)    // extension of ModR/M reg field
#define REX_X       (1 << 1)    // extension of SIB index field
#define REX_B       (1 << 0)    // extension of ModR/M r/m field, SIB base field, or Opcode reg field ( how is this determined?, seems like based on if r or x are set.)


