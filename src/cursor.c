#include "include/cursor.h"
#include "include/debug.h"
#include "include/editor.h"
#include <curses.h>
#include <stdint.h>

Cursor
Cursor__new() {
    return (Cursor) {.x = 0, .y = 0, .highest_x = 0, .offset = {.x = 0, .y = 0}};
}

void
Cursor__up() {
    WINDOW *display = EDITOR.display_list.data[EDITOR.display].win;
    Cursor *cursor = Editor__get_cursor();

    if (cursor->offset.y > 0 && cursor->y == cursor->offset.y) {
        wscrl(display, -1);
        cursor->offset.y -= 1;
    }

    if (cursor->y == 0) {
        return;
    }

    cursor->y -= 1;
    move(cursor->y - cursor->offset.y, cursor->x);
}

void
Cursor__down() {
    WINDOW *display = EDITOR.display_list.data[EDITOR.display].win;
    Cursor *cursor = Editor__get_cursor();

    uint64_t max_y = getmaxy(display);
    uint64_t new_y = cursor->y + 1;

    if (new_y < max_y) {
        cursor->y += 1;
        move(cursor->y, cursor->x);
    } else {
        cursor->y += 1;

        wscrl(display, 1);
        cursor->offset.y += 1;
    }
}

void
Cursor__left() {
    Cursor *cursor = Editor__get_cursor();

    if (cursor->x == 0) {
        return;
    }

    cursor->x -= 1;
    move(cursor->y, cursor->x);
}

void
Cursor__right() {
    Cursor *cursor = Editor__get_cursor();

    Str *line = Editor__get_current_line();

    if (line != NULL) {
        if (cursor->x < line->len) {
            cursor->x += 1;
            move(cursor->y, cursor->x);

            if (cursor->highest_x < cursor->x) {
                cursor->highest_x = cursor->x;
            }
        }
    }
}

void
Cursor__from(uint64_t x, uint64_t y) {
    Cursor *cursor = Editor__get_cursor();

    cursor->x = x;
    cursor->y = y;

    move(cursor->y, cursor->x);
}
