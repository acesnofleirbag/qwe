#ifndef QUICKWARE_CURSOR_H
#define QUICKWARE_CURSOR_H

#include <stdint.h>

typedef struct {
    uint64_t x;
    uint64_t y;
} Cursor;

Cursor Cursor__new();
void Cursor__up(Cursor *cursor);
void Cursor__down(Cursor *cursor);
void Cursor__left(Cursor *cursor);
void Cursor__right(Cursor *cursor);

#endif
