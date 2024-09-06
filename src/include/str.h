#ifndef QUICKWARE_STR_H
#define QUICKWARE_STR_H

#include <stdint.h>

#define STR_DEFAULT_SIZE 256

typedef struct str {
    char *data;
    uint64_t len;
    uint64_t capacity;
} str_t;

str_t string__new(unsigned long size);
void string__append(str_t *str, uint64_t x, char ch);
void string__release(str_t *str);
void string__remove(str_t *str, uint64_t x);

#endif
