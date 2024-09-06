#include "include/explorer.h"
#include <stdbool.h>
#include <sys/stat.h>

bool
explorer__is_file(char *str) {
    struct stat buffer;

    if (stat(str, &buffer) != 0) {
        return false;
    }

    return S_ISREG(buffer.st_mode);
}

bool
explorer__is_dir(char *str) {
    struct stat buffer;

    if (stat(str, &buffer) != 0) {
        return false;
    }

    return S_ISDIR(buffer.st_mode);
}
