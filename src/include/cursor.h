#ifndef QUICKWARE_CURSOR_H
#define QUICKWARE_CURSOR_H

#include <stdint.h>

typedef struct cursor {
    uint64_t x;
    uint64_t y;
    uint64_t highest_x;
    struct {
        uint64_t x;
        uint64_t y;
    } offset;
} cursor_t;

cursor_t cursor__new();
void cursor__up();
void cursor__down();
void cursor__left();
void cursor__right();
void cursor__from(uint64_t x, uint64_t y);

#endif
