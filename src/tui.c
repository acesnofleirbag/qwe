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
    getmaxyx(stdscr, x_bound, y_bound);

    editor->viewport = (Viewport) {.x = x_bound, .y = y_bound};
}

void
TUI__exit() {
    refresh();
    endwin();
}
