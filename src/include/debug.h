#ifndef QUICKWARE_DEBUG_H
#define QUICKWARE_DEBUG_H

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static inline void
debug(const char *fmt, ...) {
    va_list args;

    va_start(args, fmt);

    FILE *file = fopen("debug.log", "a");

    if (file != NULL) {
        time_t now = time(NULL);
        char *timestamp = ctime(&now);
        size_t len = strlen(timestamp);
        timestamp[len - 1] = '\0';

        fprintf(file, "[%s] ", timestamp);
        vfprintf(file, fmt, args);
        fprintf(file, "\n");

        fclose(file);
    }

    va_end(args);
}

#endif
