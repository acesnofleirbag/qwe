#include "include/cursor.h"
#include <curses.h>

Cursor
Cursor__new() {
    return (Cursor) {
        .x = 0,
        .y = 0,
    };
}

void
Cursor__up(Cursor *cursor) {
    cursor->y -= 1;

    move(cursor->y, cursor->x);
}

void
Cursor__down(Cursor *cursor) {
    cursor->y += 1;

    move(cursor->y, cursor->x);
}

void
Cursor__left(Cursor *cursor) {
    cursor->x -= 1;

    move(cursor->y, cursor->x);
}

void
Cursor__right(Cursor *cursor) {
    cursor->x += 1;

    move(cursor->y, cursor->x);
}
