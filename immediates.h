#ifndef IMMEDIATES_H
#define IMMEDIATES_H

#include "instruction.h"

long long get_displacement(struct x86_instr * inst, int mode);

#endif
