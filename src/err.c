#include "include/err.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void
panic(const char *format, ...) {
    va_list args;

    va_start(args, format);

    fprintf(stderr, "ERROR: ");
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");

    va_end(args);

    exit(1);
}

void
err(const char *format, ...) {
    va_list args;

    va_start(args, format);

    fprintf(stderr, "ERROR: ");
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");

    va_end(args);
}
