#ifndef QUICKWARE_BUFFER_H
#define QUICKWARE_BUFFER_H

#include "str.h"
#include <stdint.h>

typedef struct buffer {
    uint64_t id;
    uint64_t lines;
    str_t *data;
} buffer_t;

buffer_t buffer__new(uint64_t id);
buffer_t buffer__from_file(char *fname, uint64_t id);
void buffer__release(buffer_t *buffer);

#endif
