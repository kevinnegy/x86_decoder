#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// intel syntax has destination first like mips
// bytes should be stored as unsigned char, 2 digit hexadecimals

#define MAX_INSTR_LEN 15 // TODO confirm

// Stores the byte instruction, the index of each segment, and the string assembly instruction
struct x86_instr{
    unsigned char * byte_code;
    int prefix_flag; // If it exists, must be index 0. 
    int rex_ptr;
    int opcode_ptr;
    int modrm_ptr;
    int sib_ptr;
    int displacement_ptr;
    int immediate_ptr; 
    char * x86_string;
};

struct x86_instr * create_x86_instr(char * bytes); 
void disassemble_instr(struct x86_instr * inst);
void build_x86_string(struct x86_instr * inst);

void check_prefix(struct x86_instr * inst);
void check_opcode(struct x86_instr * inst);
void check_modrm(struct x86_instr * inst);
void check_sib(struct x86_instr * inst);
void check_displacement(struct x86_instr * inst);
void check_immediate(struct x86_instr * inst);

// Copies user-provided bytecode into new x86 struct
// TODO maybe change this to something else. Is a function necessary?
struct x86_instr * create_x86_instr(char * bytes){ 

    struct x86_instr * inst = (struct x86_instr *) malloc(sizeof(struct x86_instr));
    
    inst->prefix_flag = 0; // If checkprefix finds a prefix, this will be set to 1
    inst->rex_ptr = 0; 
    inst->opcode_ptr = 0; 
    inst->modrm_ptr = 0; 
    inst->sib_ptr = 0; 
    inst->immediate_ptr = 0; 
    inst->displacement_ptr = 0; 

    inst->byte_code = (unsigned char *) malloc(sizeof(unsigned char) * MAX_INSTR_LEN); 
    memcpy(inst->byte_code, bytes, MAX_INSTR_LEN); 

    return inst;

}

/** Prefixes **/

enum prefixes{
    prefix_lock = 0xf0,
    prefix_repn = 0xf2,
    prefix_rep = 0xf3,
    prefix_bnd = 0xf2, // TODO This is an iff prefix

    prefix_cs = 0x2e, 
    prefix_ss = 0x36, 
    prefix_ds = 0x3e, 
    prefix_es = 0x26, 
    prefix_fs = 0x64, 
    prefix_gs = 0x65, 
    prefix_branchn = 0x2e, // TODO another weird repeat
    prefix_branch = 0x3e, // TODO another weird repeat

    prefix_op_size_override = 0x66,
    prefix_addr_size_override = 0x67,
    
    REX = 0x48, // TODO fix
};

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

/** Opcodes **/
// Can be 1,2, or 3 bytes. A fourth can be in ModRM
enum opcodes{
    op_call = 0xe8,
    op_mov = 0x89,
};

// Assumes check_prefix has determined the correct opcode location
void check_opcode(struct x86_instr * inst){
    // TODO handle more than 1 byte of opcode
    unsigned char opcode = inst->byte_code[inst->opcode_ptr]; 
    switch(opcode){
    case(op_call):
        printf("CALL\n");
        break;
    case(op_mov):
        printf("MOV\n");
        break;
    default:
        printf("check_opcode: invalid opcode\n");
        break;
    }
}

// In 2 byte mode, it must have one of the following (3 byte is the same except a third opcode):
    // escape opcode 0F primary opcode and then a second opcode
    // mandatory prefix (66, F2, or F3), then escape, then second opcode

// Registers (Prefix E for 32bit, prefix R for 64bit) 
    // 000 - AX 
    // 001 - CX 
    // 010 - DX 
    // 011 - BX 
    // 100 - SP 
    // 101 - BP 
    // 110 - SI 
    // 111 - DI 

// If w bit exists, use above encoding when w = 1, if 0:
    // 000 - AL 
    // 001 - CL 
    // 010 - DL 
    // 011 - BL 
    // 100 - AH 
    // 101 - CH 
    // 110 - DH 
    // 111 - BH 

/** ModR/M **/
// ModR/M byte - addressing mode byte:
    // mod first two bits - something
    // reg/opcode 3bits - a register or further opcode
    // r/m 3 bits - another register or further opcode

// some ModR/M bytes require a second byte SIB 
// Scaled index byte:
    // scale
    // index - index register
    // base - base register


/** Displacement/Immediate **/
// Address displacement - 1, 2, or 4 bytes or none
// Immediate - 1,2,4, or none



//MOV
//                        100 - rsp 111 - rdi (mov rdi to rsp)
//    0100 1000 1000 1001 1110 0111
//         1R0B 1000 1001 11          - qwordreg 1 to qwordreg2
//         

int main(){
    unsigned char * bytes = (unsigned char *) malloc(sizeof(unsigned char) * MAX_INSTR_LEN);
    bytes[0] = 0xe8;
    bytes[1] = 0x89;
    bytes[2] = 0xe7;
    struct x86_instr * inst = create_x86_instr(bytes);
    check_prefix(inst);
    check_opcode(inst);
    
    int i = 0;
    for(i = 0; i<3; i++){
        printf("%2hhx ", inst->byte_code[i]);
    }
    
    printf("%x", (REX_prefix | REX_W));
    return 0;
}
