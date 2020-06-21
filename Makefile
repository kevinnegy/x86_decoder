bin= x86

all: x86

x86: instruction.h instruction.c prefix.h prefix.c registers.h opcode.h opcode.c main.c
	gcc instruction.h instruction.c prefix.h prefix.c registers.h opcode.h opcode.c main.c -o x86

clean:
	rm $(bin) 
	

