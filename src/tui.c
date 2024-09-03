#include "include/display.h"
#include "include/editor.h"
#include <curses.h>

void
TUI__start(Editor *editor) {
    int x_bound;
    int y_bound;

    initscr();
    raw();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    getmaxyx(stdscr, y_bound, x_bound);

    WINDOW *root = newwin(y_bound - 1, x_bound, 0, 0);
    WINDOW *statusbar = newwin(1, x_bound, y_bound - 1, 0);

    Display root_display = {.type = DISPLAY_TYPE__ROOT, .window = root};
    Display statusbar_display = {.type = DISPLAY_TYPE__STATUS_BAR, .window = statusbar};

    DisplayList__add(&editor->display_list, root_display);
    DisplayList__add(&editor->display_list, statusbar_display);

    editor->display = &root_display;
    editor->viewport = (Viewport) {.x = x_bound, .y = y_bound};
    Cursor__set_viewport(&editor->cursor, editor->viewport);
}

void
TUI__exit() {
    refresh();
    endwin();
}
