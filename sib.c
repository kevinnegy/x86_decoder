#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "sib.h"
#include "registers.h"
#include "prefix.h"

void check_sib(unsigned char * inst, int operand_size, struct prefixes * prfx){
    int sib = inst[0];
    int scale = (sib & SIB_SS) >> 6;
    int index = (sib & SIB_INDEX) >> 3;
    int base = (sib & SIB_BASE);

    if(prfx->REX != 0){
        u_int8_t rex_x = (prfx->REX & REX_X) << 2; 
        u_int8_t rex_b = (prfx->REX & REX_B) << 3;
        base = rex_b | base;
        index = rex_x | index;
    }

    operand_size = 64; 

    char * base_reg = get_register(base, operand_size, prfx); 
    if (base == 5)
        printf("0");
    else
        printf("%s", base_reg);

    switch(scale){
        case 0:
            switch(index & 0x7){ 

                case 0:
                case 1:
                case 2:
                case 3:
                    printf("+%s", get_register(index, operand_size, prfx));
                case 4: // Do nothing!
                    break;
                case 5:
                case 6:
                case 7:
                    printf("+%s", get_register(index, operand_size, prfx));
                    break;

                default:
                    assert(0);
            }

            break;

        case 1:
            switch(index & 0x7){
                case 0:
                case 1:
                case 2:
                case 3:
                    printf("+%s*2", get_register(index, operand_size, prfx));

                    if (base == 5){
                        printf("+EBP]");
                        break;
                    }
                    break;

                case 4: // Do nothing!
                    break;

                case 5:
                case 6:
                case 7:
                    printf("+%s*2", get_register(index, operand_size, prfx));

                    if (base == 5){
                        printf("+EBP]");
                        break;
                    }
                    break;

                default:
                    assert(0);
            }

            break;

        case 2:
            switch(index & 0x7){
                case 0:
                case 1:
                case 2:
                case 3:
                    printf("+%s*4", get_register(index, operand_size, prfx));

                    if (base == 5){
                        printf("+EBP]");
                        break;
                    }
                    break;
                case 4: // Do nothing!
                    break;

                case 5:
                case 6:
                case 7:
                    printf("+%s*4", get_register(index, operand_size, prfx));

                    if (base == 5){
                        printf("+EBP]");
                        break;
                    }
                    break;

                default:
                    assert(0);
            }

            break;

        case 3:
            switch(index & 0x7){
                case 0:
                case 1:
                case 2:
                case 3:
                    printf("+%s*8", get_register(index, operand_size, prfx));

                    if (base == 5)
                        return;

                    break;

                case 4: // Do nothing!
                    break;

                case 5:
                case 6:
                case 7:
                    printf("+%s*8", get_register(index, operand_size, prfx));

                    if (base == 5)
                        return;

                    break;

                default:
                    assert(0);
            }

            break;

        default:
            assert(0);
    }
}
