# x86_decoder 
Identifies instructions that access memory and prints associated memory addresses. 

Note: vector instructions not fully implemented

Instructions are parsed in order:

1) prefix
2) REX prefix
3) opcode

Then opcode determines which to parse between modrm byte, SIB, or immediate bytes.

