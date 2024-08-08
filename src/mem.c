#include "include/mem.h"
#include <stddef.h>
#include <stdlib.h>

void*
MEM__request(unsigned long size) {
    return malloc(size);
}

void*
MEM__refresh(void* target, unsigned long size) {
    return realloc(target, size);
}

void
MEM__release(void* target) {
    free(target);
}
