#ifndef DECODER_H
#define DECODER_H
#include "instruction.h"

#define BYTE 8 

struct byte_1{
    u_int8_t byte:BYTE;
    unsigned long long bytes_after:(BYTE * 8);
    unsigned long long bytes_after2:(BYTE * 6);
};

struct byte_2{
    unsigned long long bytes_before:BYTE;
    u_int8_t byte:BYTE;
    unsigned long long bytes_after:(BYTE * 8);
    unsigned long long bytes_after2:(BYTE * 5);
};

struct byte_3{
    unsigned long long bytes_before:(BYTE * 2);
    u_int8_t byte:BYTE;
    unsigned long long bytes_after:(BYTE * 8);
    unsigned long long bytes_after2:(BYTE * 4);
};

struct byte_4{
    unsigned long long bytes_before:(BYTE * 3);
    u_int8_t byte:BYTE;
    unsigned long long bytes_after:(BYTE * 8);
    unsigned long long bytes_after2:(BYTE * 3);
};

struct byte_5{
    unsigned long long bytes_before:(BYTE * 4);
    u_int8_t byte:BYTE;
    unsigned long long bytes_after:(BYTE * 8);
    unsigned long long bytes_after2:(BYTE * 2);
};

struct byte_6{
    unsigned long long bytes_before:(BYTE * 5);
    u_int8_t byte:BYTE;
    unsigned long long bytes_after:(BYTE * 8);
    unsigned long long bytes_after2:(BYTE * 1);
};

struct byte_7{
    unsigned long long bytes_before:(BYTE * 6);
    u_int8_t byte:BYTE;
    unsigned long long bytes_after:(BYTE * 8);
};

struct byte_8{
    unsigned long long bytes_before:(BYTE * 7);
    u_int8_t byte:BYTE;
    unsigned long long bytes_after:(BYTE * 7);
};

struct byte_9{
    unsigned long long bytes_before:(BYTE * 8);
    u_int8_t byte:BYTE;
    unsigned long long bytes_after:(BYTE * 6);
};

struct byte_10{
    unsigned long long bytes_before2:BYTE;
    unsigned long long bytes_before:(BYTE * 8);
    u_int8_t byte:BYTE;
    unsigned long long bytes_after:(BYTE * 5);
};

struct byte_11{
    unsigned long long bytes_before2:(BYTE * 2);
    unsigned long long bytes_before:(BYTE * 8);
    u_int8_t byte:BYTE;
    unsigned long long bytes_after:(BYTE * 4);
};

struct byte_12{
    unsigned long long bytes_before2:(BYTE * 3);
    unsigned long long bytes_before:(BYTE * 8);
    u_int8_t byte:BYTE;
    unsigned long long bytes_after:(BYTE * 3);
};

struct byte_13{
    unsigned long long bytes_before2:(BYTE * 4);
    unsigned long long bytes_before:(BYTE * 8);
    u_int8_t byte:BYTE;
    unsigned long long bytes_after:(BYTE * 2);
};

struct byte_14{
    unsigned long long bytes_before2:(BYTE * 5);
    unsigned long long bytes_before:(BYTE * 8);
    u_int8_t byte:BYTE;
    unsigned long long bytes_after:BYTE;
};

struct byte_15{
    unsigned long long bytes_before2:(BYTE * 6);
    unsigned long long bytes_before:(BYTE * 8);
    u_int8_t byte:BYTE;
};

void decoder(unsigned char * inst);
#endif
