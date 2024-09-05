#ifndef QUICKWARE_CURSOR_H
#define QUICKWARE_CURSOR_H

#include <stdint.h>

typedef struct {
    uint64_t x;
    uint64_t y;
    uint64_t highest_x;
    struct {
        uint64_t x;
        uint64_t y;
    } offset;
} Cursor;

Cursor Cursor__new();
void Cursor__up();
void Cursor__down();
void Cursor__left();
void Cursor__right();
void Cursor__from(uint64_t x, uint64_t y);

#endif
