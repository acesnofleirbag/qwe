#include "include/cursor.h"
#include "include/debug.h"
#include "include/editor.h"
#include <curses.h>
#include <stdint.h>

cursor_t
cursor__new() {
    return (cursor_t) {.x = 0, .y = 0, .highest_x = 0, .offset = {.x = 0, .y = 0}};
}

void
cursor__up() {
    WINDOW *display = EDITOR.display_list.data[EDITOR.display].win;
    cursor_t *cursor = editor__get_cursor();

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
cursor__down() {
    WINDOW *display = EDITOR.display_list.data[EDITOR.display].win;
    cursor_t *cursor = editor__get_cursor();

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
cursor__left() {
    cursor_t *cursor = editor__get_cursor();

    if (cursor->x == 0) {
        return;
    }

    cursor->x -= 1;
    move(cursor->y, cursor->x);
}

void
cursor__right() {
    cursor_t *cursor = editor__get_cursor();

    str_t *line = editor__get_current_line();

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
cursor__from(uint64_t x, uint64_t y) {
    cursor_t *cursor = editor__get_cursor();

    cursor->x = x;
    cursor->y = y;

    move(cursor->y, cursor->x);
}
