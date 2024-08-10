#include "include/str.h"
#include "include/mem.h"
#include <stdio.h>
#include <string.h>

Str
String__new(unsigned long size) {
    return (Str) {
        .data = MEM__request(size * sizeof(char) + 1),
        .len = 0,
        .capacity = size,
    };
}

void
String__append(Str *str, const char ch) {
    if (str->len == str->capacity) {
        unsigned long new_capacity = str->capacity * 2;

        str->data = MEM__expand(str->data, new_capacity, str->capacity, CASTMODE__CHAR);
        str->capacity = new_capacity;
    }

    memmove(&str->data[str->len], &ch, 1);
    str->len += 1;
}

void
String__release(Str *str) {
    MEM__release(str->data);
}
