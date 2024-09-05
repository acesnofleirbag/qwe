#include "include/cursor.h"
#include "include/debug.h"
#include <stdint.h>

Cursor
Cursor__new() {
    return (Cursor) {.x = 0, .y = 0, .highest_x = 0, .offset = {.x = 0, .y = 0}};
}

void
Cursor__up(Cursor *cursor) {
    if (cursor->y == 0) {
        return;
    }

    cursor->y -= 1;
    move(cursor->y - cursor->offset.y, cursor->x);
}

void
Cursor__down(Cursor *cursor) {
    cursor->y += 1;
    move(cursor->y, cursor->x);
}

void
Cursor__left(Cursor *cursor) {
    if (cursor->x == 0) {
        return;
    }

    cursor->x -= 1;
    move(cursor->y, cursor->x);
}

void
Cursor__right(Cursor *cursor) {
    cursor->x += 1;
    move(cursor->y, cursor->x);

    if (cursor->highest_x < cursor->x) {
        cursor->highest_x = cursor->x;
    }
}

void
Cursor__from(Cursor *cursor, uint64_t x, uint64_t y) {
    cursor->x = x;
    cursor->y = y;

    move(cursor->y, cursor->x);
}
