bin= x86

all: x86

x86: instruction.h instruction.c prefix.h prefix.c registers.h registers.c opcode.h opcode.c modrm.h modrm.c main.c
	gcc instruction.h instruction.c prefix.h prefix.c registers.h registers.c opcode.h opcode.c modrm.h modrm.c main.c -o x86

clean:
	rm $(bin) 
	

