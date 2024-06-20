#ifndef STDIO_H
#define STDIO_H

#include <stdarg.h>

typedef unsigned long size_t;

struct FILE {
    int _file;              
    unsigned char* _ptr;     
    unsigned char* _base;   
    int _bufsize;           
};

int printf(const char* format, ...);
int sprintf(char* buffer, const char* format, ...);
int snprintf(char* buffer, size_t size, const char* format, ...);
int fprintf(struct FILE* stream, const char* format, ...);

#endif