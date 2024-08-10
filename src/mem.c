#include "include/mem.h"
#include "include/err.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void*
MEM__request(unsigned long size) {
    void* ptr = malloc(size);

    // Empty memory initialization
    memset(ptr, 0, size);

    return ptr;
}

void*
MEM__expand(void* target, unsigned long size, /* previous allocated size */ unsigned long psize, CastMode cast) {
    void* ptr = realloc(target, size);

    if (size > psize) {
        // Empty memory initialization for new allocated area
        switch (cast) {
            case CASTMODE__INT:
                memset((int*) ptr + psize, 0, size - psize);
                break;
            case CASTMODE__CHAR:
                memset((char*) ptr + psize, 0, size - psize);
                break;
            default:
                panic("ERROR: MEM__expand cast mode not defined");
        }
    }

    return ptr;
}

void
MEM__release(void* target) {
    free(target);
    target = NULL;
}
