#include "include/buffer.h"
#include "include/debug.h"
#include "include/err.h"
#include "include/str.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

Buffer
Buffer__new(uint64_t id) {
    return (Buffer) {
        .id = id,
        .data = NULL,
        .lines = 0,
    };
}

Buffer
Buffer__from_file(char *fname, uint64_t id) {
    FILE *file = fopen(fname, "r");

    if (file == NULL) {
        err("Opening file: %s", fname);
        fclose(file);
    }

    uint64_t lines = 0;
    Str str = String__new(1);
    Str *data = calloc(1, sizeof(Str));
    data[lines] = str;

    char ch;

    while ((ch = fgetc(file)) != EOF) {
        Str *line = &data[lines];

        if (ch == '\r') {
            continue;
        }

        if (ch == '\n') {
            lines += 1;
            data = realloc(data, sizeof(Str) * (lines + 1));
            Str new_line = String__new(1);
            data[lines] = new_line;
            continue;
        }

        String__append(line, line->len, ch);
    }

    fclose(file);

    return (Buffer) {
        .id = id,
        .data = data,
        .lines = lines,
    };
}

void
Buffer__release(Buffer *buffer) {
    for (uint64_t i = 0; i < buffer->lines; i++) {
        String__release(&buffer->data[i]);
    }
}
