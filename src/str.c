#include "include/str.h"
#include "include/mem.h"

Str *
String__new(unsigned long size) {
    return (Str *) MEM__request(size);
}

void
String__append(Str *str, char *ch) {}
