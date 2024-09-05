#include "include/cursor.h"
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

    WINDOW *editor_win = newwin(y_bound - 1, x_bound, 0, 0);
    WINDOW *statusbar_win = newwin(1, x_bound, y_bound - 1, 0);

    scrollok(editor_win, TRUE);

    Display editor_display = {.type = DISPLAY_TYPE__ROOT, .win = editor_win, .cursor = Cursor__new()};
    Display statusbar_display = {.type = DISPLAY_TYPE__STATUS_BAR, .win = statusbar_win, .cursor = Cursor__new()};

    DisplayList__add(&editor->display_list, editor_display);
    DisplayList__add(&editor->display_list, statusbar_display);
}

void
TUI__exit() {
    refresh();
    endwin();
}
