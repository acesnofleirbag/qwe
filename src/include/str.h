#ifndef QUICKWARE_STR_H
#define QUICKWARE_STR_H

#include <stdint.h>

#define STR_DEFAULT_SIZE 256

typedef struct {
    char *data;
    uint64_t len;
    uint64_t capacity;
} Str;

Str String__new(unsigned long size);
void String__append(Str *str, uint64_t x, char ch);
void String__release(Str *str);
void String__remove(Str *str, uint64_t x);

#endif
