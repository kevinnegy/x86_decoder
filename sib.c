#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "sib.h"
#include "prefix.h"
#include "instruction.h"
#include "registers.h"

void check_sib_32(unsigned char * inst, int opcode_index){
    struct one_op_sib * mold = (struct one_op_sib *) &inst[opcode_index];
    int scale = (mold->sib & SIB_SS) >> 6;
    int index = (mold->sib & SIB_INDEX) >> 3;
    int base = (mold->sib & SIB_BASE);

    // TODO handle base == 5
    char * base_reg = get_register(base, 32); 

    switch(scale){
        case 0:
            switch(index){
                case 0:
                case 1:
                case 2:
                case 3:
                    printf("memory access [%s] + ", get_register(index, 32));

                    if (base == 5)
                        break;

                    printf("%s\n", base_reg);
                    break;

                case 4: // Do nothing!
                    break;

                case 5:
                case 6:
                case 7:
                    printf("memory access [%s] + ", get_register(index, 32));

                    if (base == 5)
                        break;

                    printf("%s\n", base_reg);
                    break;

                default:
                    assert(0);
            }

            break;

        case 1:
            switch(index){
                case 0:
                case 1:
                case 2:
                case 3:
                    printf("memory access [%s * 2] + ", get_register(index, 32));

                    if (base == 5){
                        printf("[EBP] + ");
                        break;
                    }

                    printf("%s\n", base_reg);
                    break;

                case 4: // Do nothing!
                    break;

                case 5:
                case 6:
                case 7:
                    printf("memory access [%s * 2] + ", get_register(index, 32));

                    if (base == 5){
                        printf("[EBP] + ");
                        break;
                    }

                    printf("%s\n", base_reg);
                    break;

                default:
                    assert(0);
            }

            break;

        case 2:
            switch(index){
                case 0:
                case 1:
                case 2:
                case 3:
                    printf("memory access [%s * 4] + ", get_register(index, 32));

                    if (base == 5){
                        printf("[EBP] + ");
                        break;
                    }

                    printf("%s\n", base_reg);
                    break;

                case 4: // Do nothing!
                    break;

                case 5:
                case 6:
                case 7:
                    printf("memory access [%s * 4] + ", get_register(index, 32));

                    if (base == 5){
                        printf("[EBP] + ");
                        break;
                    }

                    printf("%s\n", base_reg);
                    break;

                default:
                    assert(0);
            }

            break;

        case 3:
            switch(index){
                case 0:
                case 1:
                case 2:
                case 3:
                    printf("memory access [%s * 8] + ", get_register(index, 32));

                    if (base == 5)
                        assert(0);

                    printf("%s\n", base_reg);
                    break;

                case 4: // Do nothing!
                    break;

                case 5:
                case 6:
                case 7:
                    printf("memory access [%s * 8] + ", get_register(index, 32));

                    if (base == 5)
                        assert(0);

                    printf("%s\n", base_reg);
                    break;

                default:
                    assert(0);
            }

            break;

        default:
            assert(0);
    }
}

void check_sib_64(unsigned char * inst, int rex_index){
    struct rex_one_op_sib * mold = (struct rex_one_op_sib *) &inst[rex_index];
    u_int8_t rex_x = (mold->rex & REX_X) << 2; // Need the REX_X (2nd) bit shifted left to 4th position 
    u_int8_t rex_b = (mold->rex & REX_B) << 3;
    int scale = (mold->sib & SIB_SS) >> 6;
    int index = (mold->sib & SIB_INDEX) >> 3;
    int base = (mold->sib & SIB_BASE);

    int base_ext = rex_x & base;
    int index_ext = rex_b & index;

    // TODO handle base == 5
    char * base_reg = get_register(base_ext, 32); 

    switch(scale){
        case 0:
            switch(index){
                case 0:
                case 1:
                case 2:
                case 3:
                    printf("memory access [%s] + ", get_register(index_ext, 64));

                    if (base == 5)
                        break;

                    printf("%s\n", base_reg);
                    break;

                case 4: // Do nothing!
                    break;

                case 5:
                case 6:
                case 7:
                    printf("memory access [%s] + ", get_register(index_ext, 64));

                    if (base == 5)
                        break;

                    printf("%s\n", base_reg);
                    break;

                default:
                    assert(0);
            }

            break;

        case 1:
            switch(index){
                case 0:
                case 1:
                case 2:
                case 3:
                    printf("memory access [%s * 2] + ", get_register(index_ext, 64));

                    if (base == 5){
                        printf("[EBP] + ");
                        break;
                    }

                    printf("%s\n", base_reg);
                    break;

                case 4: // Do nothing!
                    break;

                case 5:
                case 6:
                case 7:
                    printf("memory access [%s * 2] + ", get_register(index_ext, 64));

                    if (base == 5){
                        printf("[EBP] + ");
                        break;
                    }

                    printf("%s\n", base_reg);
                    break;

                default:
                    assert(0);
            }

            break;

        case 2:
            switch(index){
                case 0:
                case 1:
                case 2:
                case 3:
                    printf("memory access [%s * 4] + ", get_register(index_ext, 64));

                    if (base == 5){
                        printf("[EBP] + ");
                        break;
                    }

                    printf("%s\n", base_reg);
                    break;

                case 4: // Do nothing!
                    break;

                case 5:
                case 6:
                case 7:
                    printf("memory access [%s * 4] + ", get_register(index_ext, 64));

                    if (base == 5){
                        printf("[EBP] + ");
                        break;
                    }

                    printf("%s\n", base_reg);
                    break;

                default:
                    assert(0);
            }

            break;

        case 3:
            switch(index){
                case 0:
                case 1:
                case 2:
                case 3:
                    printf("memory access [%s * 8] + ", get_register(index_ext, 64));

                    if (base == 5)
                        assert(0);

                    printf("%s\n", base_reg);
                    break;

                case 4: // Do nothing!
                    break;

                case 5:
                case 6:
                case 7:
                    printf("memory access [%s * 8] + ", get_register(index_ext, 64));

                    if (base == 5)
                        assert(0);

                    printf("%s\n", base_reg);
                    break;

                default:
                    assert(0);
            }

            break;

        default:
            assert(0);
    }
}
