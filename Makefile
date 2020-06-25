src_files= $(wildcard *.c)
obj= $(src_files:.c=.o)

x86: $(src_files)
	gcc $(src_files) -o x86

clean:
	rm -f $(obj) x86
	

