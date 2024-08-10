#include "include/buffer.h"
#include "include/err.h"
#include "include/str.h"
#include <stddef.h>
#include <stdio.h>

Buffer
Buffer__new() {
    return (Buffer) {
        .id = 1,
        .data = NULL,
        .prev = NULL,
        .next = NULL,
        .lines = 0,
    };
}

Buffer
Buffer__from_file(char *fname) {
    FILE *file = fopen(fname, "r+");

    if (file == NULL) {
        err("Opening file: %s", fname);
        fclose(file);
    }

    return (Buffer) {
        .id = 1,  // FIXME
        .data = NULL,
        .prev = NULL,
        .next = NULL,
        .lines = 0,
    };
}
