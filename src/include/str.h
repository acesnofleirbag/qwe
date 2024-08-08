#ifndef QUICKWARE_STR_H
#define QUICKWARE_STR_H

#include <sys/types.h>

#define STR_DEFAULT_SIZE 512

typedef struct {
    char* data;
    u_int64_t size;
} Str;

Str* String__new(unsigned long size);
void String__append(Str* str, char* ch);

#endif
