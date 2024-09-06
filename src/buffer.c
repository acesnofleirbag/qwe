#include "include/buffer.h"
#include "include/debug.h"
#include "include/err.h"
#include "include/str.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

buffer_t
buffer__new(uint64_t id) {
    return (buffer_t) {
        .id = id,
        .data = NULL,
        .lines = 0,
    };
}

buffer_t
buffer__from_file(char *fname, uint64_t id) {
    FILE *file = fopen(fname, "r");

    if (file == NULL) {
        err("Opening file: %s", fname);
        fclose(file);
    }

    uint64_t lines = 0;
    str_t str = string__new(1);
    str_t *data = calloc(1, sizeof(str_t));
    data[lines] = str;

    char ch;

    while ((ch = fgetc(file)) != EOF) {
        str_t *line = &data[lines];

        if (ch == '\r') {
            continue;
        }

        if (ch == '\n') {
            lines += 1;
            data = realloc(data, sizeof(str_t) * (lines + 1));
            str_t new_line = string__new(1);
            data[lines] = new_line;
            continue;
        }

        string__append(line, line->len, ch);
    }

    fclose(file);

    return (buffer_t) {
        .id = id,
        .data = data,
        .lines = lines,
    };
}

void
buffer__release(buffer_t *buffer) {
    for (uint64_t i = 0; i < buffer->lines; i++) {
        string__release(&buffer->data[i]);
    }
}
