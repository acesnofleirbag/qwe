#include "include/str.h"
#include <curses.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

str_t
string__new(unsigned long size) {
    return (str_t) {
        .data = calloc(size, sizeof(char) + 1),
        .len = 0,
        .capacity = size,
    };
}

void
string__append(str_t *str, uint64_t x, const char ch) {
    if (str->len == str->capacity) {
        unsigned long new_capacity = str->capacity * 2;

        str->data = realloc(str->data, new_capacity);
        str->capacity = new_capacity;
    }

    memmove(&str->data[x + 1], &str->data[x], str->len - x);
    str->data[x] = ch;
    str->len += 1;
}

void
string__remove(str_t *str, uint64_t x) {
    memmove(&str->data[x - 1], &str->data[x], str->len - x + 1);
    str->len -= 1;
    str->data[str->len] = '\0';
}

void
string__release(str_t *str) {
    free(str->data);
    str->data = NULL;
}
