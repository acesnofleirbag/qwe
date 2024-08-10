#ifndef QUICKWARE_MEMORY_H
#define QUICKWARE_MEMORY_H

#include <stddef.h>

typedef enum {
    CASTMODE__INT,
    CASTMODE__CHAR,
} CastMode;

void* MEM__request(unsigned long size);
void* MEM__expand(void* target, unsigned long size, unsigned long psize, CastMode cast);
void MEM__release(void* target);

#endif
