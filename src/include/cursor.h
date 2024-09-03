#ifndef QUICKWARE_CURSOR_H
#define QUICKWARE_CURSOR_H

#include "viewport.h"
#include <stdint.h>

typedef struct {
    uint64_t x;
    uint64_t y;
    uint64_t x_tracking;
    Viewport viewport;
} Cursor;

Cursor Cursor__new();
void Cursor__up(Cursor *cursor);
void Cursor__down(Cursor *cursor);
void Cursor__left(Cursor *cursor);
void Cursor__right(Cursor *cursor);
void Cursor__set_x_tracking(Cursor *cursor, uint64_t x);
void Cursor__from(Cursor *cursor, uint64_t x, uint64_t y);
void Cursor__set_viewport(Cursor *cursor, Viewport viewport);

#endif
