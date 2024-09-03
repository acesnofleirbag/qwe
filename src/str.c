#include "include/str.h"
#include <curses.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Str
String__new(unsigned long size) {
    return (Str) {
        .data = calloc(size, sizeof(char) + 1),
        .len = 0,
        .capacity = size,
    };
}

void
String__append(Str *str, uint64_t x, const char ch) {
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
String__remove(Str *str, uint64_t x) {
    memmove(&str->data[x - 1], &str->data[x], str->len - x + 1);
    str->len -= 1;
    str->data[str->len] = '\0';
}

void
String__release(Str *str) {
    free(str->data);
    str->data = NULL;
}
