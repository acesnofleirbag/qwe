#ifndef QUICKWARE_BUFFER_H
#define QUICKWARE_BUFFER_H

#include "str.h"
#include <sys/types.h>

struct Buffer {
    int id;
    struct Buffer *prev, *next;
    Str *data;
    u_int64_t lines;
};

typedef struct Buffer Buffer;

Buffer Buffer__new();
Buffer Buffer__from_file(char *fname);

#endif
