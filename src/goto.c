#include "include/goto.h"
#include "include/cursor.h"
#include "include/editor.h"

void
goto__start_line() {
    cursor_t *cursor = editor__get_cursor();

    cursor__from(0, cursor->y);
}

void
goto__end_line() {
    cursor_t *cursor = editor__get_cursor();
    str_t *line = editor__get_current_line();

    if (line != NULL) {
        cursor__from(line->len, cursor->y);
    }
}

void
goto__start_file() {
    cursor__from(0, 0);
}

void
goto__end_file() {
    cursor__from(0, EDITOR.buffer.lines);
}
