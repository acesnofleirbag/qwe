#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void
debug(const char *msg) {
    FILE *file = fopen("qwe.log", "a");

    if (file != NULL) {
        time_t now = time(NULL);
        char *timestamp = ctime(&now);
        size_t len = strlen(timestamp);
        timestamp[len - 1] = '\0';

        fprintf(file, "[%s] %s\n", timestamp, msg);
        fclose(file);
    }
}
