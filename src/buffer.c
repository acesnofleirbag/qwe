#include "include/buffer.h"
#include "include/str.h"
#include <stddef.h>

Buffer
Buffer__new() {
    return (Buffer) {
        .id = 1,
        .data = String__new(sizeof(char)),
        .prev = NULL,
        .next = NULL,
        .lines = 0,
    };
}
