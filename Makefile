bin=x86

all: x86

x86: x86_ins.c
	gcc x86_ins.c -o x86

clean:
	rm $(bin) 
	

