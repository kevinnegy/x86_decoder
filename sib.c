#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "sib.h"
#include "prefix.h"
#include "registers.h"

void check_sib(unsigned char * inst, int operand_size, int rex){
    int sib = inst[0];
    int scale = (sib & SIB_SS) >> 6;
    int index = (sib & SIB_INDEX) >> 3;
    int base = (sib & SIB_BASE);

    u_int8_t rex_x = (rex & REX_X) << 2; 
    u_int8_t rex_b = (rex & REX_B) << 3;
    base = rex_b | base;
    index = rex_x | index;

    operand_size = 64; 

    char * base_reg = get_register(base, operand_size, rex); 
    if (base == 5)
        printf("0");
    else
        printf("%s", base_reg);

    int index_no_rex = index & 0x7; // Ignore REX ext bit for switch
    switch(scale){
        case 0:
            switch(index_no_rex){ 

                case 0:
                case 1:
                case 2:
                case 3:
                    printf("+%s", get_register(index, operand_size, rex));
                case 4: // Do nothing!
                    break;
                case 5:
                case 6:
                case 7:
                    printf("+%s", get_register(index, operand_size, rex));
                    break;

                default:
                    assert(0);
            }

            break;

        case 1:
            switch(index_no_rex){
                case 0:
                case 1:
                case 2:
                case 3:
                    printf("+%s*2", get_register(index, operand_size, rex));

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
                    printf("+%s*2", get_register(index, operand_size, rex));

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
            switch(index_no_rex){
                case 0:
                case 1:
                case 2:
                case 3:
                    printf("+%s*4", get_register(index, operand_size, rex));

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
                    printf("+%s*4", get_register(index, operand_size, rex));

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
            switch(index_no_rex){
                case 0:
                case 1:
                case 2:
                case 3:
                    printf("+%s*8", get_register(index, operand_size, rex));

                    if (base == 5)
                        return;

                    break;

                case 4: // Do nothing!
                    break;

                case 5:
                case 6:
                case 7:
                    printf("+%s*8", get_register(index, operand_size, rex));

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
