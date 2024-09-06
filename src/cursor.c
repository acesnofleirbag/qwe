#include "include/cursor.h"
#include "include/editor.h"
#include <curses.h>
#include <stdint.h>

cursor_t
cursor__new() {
    return (cursor_t) {.x = 0, .y = 0, .highest_x = 0, .offset = {.x = 0, .y = 0}};
}

void
cursor__up() {
    cursor_t *cursor = editor__get_cursor();

    if (cursor->offset.y > 0 && cursor->y == cursor->offset.y) {
#ifndef TEST
        WINDOW *display = EDITOR.display_list.data[EDITOR.display].win;
        wscrl(display, -1);
#endif  // TEST
        cursor->offset.y -= 1;
    }

    if (cursor->y == 0) {
        return;
    }

    cursor->y -= 1;
#ifndef TEST
    move(cursor->y - cursor->offset.y, cursor->x);
#endif  // TEST
}

void
cursor__down() {
    viewport_t *viewport = &EDITOR.display_list.data[EDITOR.display].viewport;
    cursor_t *cursor = editor__get_cursor();

    uint64_t new_y = cursor->y + 1;

    if (new_y < viewport->y) {
        cursor->y += 1;
#ifndef TEST
        move(cursor->y, cursor->x);
#endif  // TEST
    } else {
        cursor->y += 1;

#ifndef TEST
        WINDOW *display = EDITOR.display_list.data[EDITOR.display].win;
        wscrl(display, 1);
#endif  // TEST
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
#ifndef TEST
    move(cursor->y, cursor->x);
#endif  // TEST
}

void
cursor__right() {
    cursor_t *cursor = editor__get_cursor();
    str_t *line = editor__get_current_line();

    if (line == NULL) {
        return;
    }

    if (cursor->x < line->len) {
        cursor->x += 1;
#ifndef TEST
        move(cursor->y, cursor->x);
#endif  // TEST

        if (cursor->highest_x < cursor->x) {
            cursor->highest_x = cursor->x;
        }
    }
}

void
cursor__from(uint64_t x, uint64_t y) {
    cursor_t *cursor = editor__get_cursor();

    cursor->x = x;
    cursor->y = y;

#ifndef TEST
    move(cursor->y, cursor->x);
#endif  // TEST
}
