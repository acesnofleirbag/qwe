#include "include/goto.h"
#include "include/cursor.h"
#include "include/editor.h"

void
Goto__start_line() {
    Cursor *cursor = Editor__get_cursor();

    Cursor__from(0, cursor->y);
}

void
Goto__end_line() {
    Cursor *cursor = Editor__get_cursor();
    Str *line = Editor__get_current_line();

    if (line != NULL) {
        Cursor__from(line->len, cursor->y);
    }
}

void
Goto__start_file() {
    Cursor__from(0, 0);
}

void
Goto__end_file() {
    Cursor__from(0, EDITOR.buffer.lines);
}
