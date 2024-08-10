#ifndef QUICKWARE_STR_H
#define QUICKWARE_STR_H

#include <sys/types.h>

#define STR_DEFAULT_SIZE 256

typedef struct {
    char *data;
    u_int64_t len;
    u_int64_t capacity;
} Str;

Str String__new(unsigned long size);
void String__append(Str *str, char ch);
void String__release(Str *str);

#endif
