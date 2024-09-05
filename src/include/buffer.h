#ifndef QUICKWARE_BUFFER_H
#define QUICKWARE_BUFFER_H

#include "str.h"
#include <stdint.h>

struct Buffer {
    uint64_t id;
    uint64_t lines;
    Str *data;
};

typedef struct Buffer Buffer;

Buffer Buffer__new(uint64_t id);
Buffer Buffer__from_file(char *fname, uint64_t id);
void Buffer__release(Buffer *buffer);

#endif
