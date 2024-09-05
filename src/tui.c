#include "include/tui.h"
#include "include/cursor.h"
#include "include/display.h"
#include "include/editor.h"
#include <curses.h>
#include <string.h>

void
TUI__start() {
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

    DisplayList__add(&EDITOR.display_list, editor_display);
    DisplayList__add(&EDITOR.display_list, statusbar_display);
}

void
TUI__render() {
    TUI__render_editor();
    TUI__render_statusbar();
}

void
TUI__render_editor() {
    WINDOW *display = EDITOR.display_list.data[EDITOR.display].win;
    Cursor *cursor = Editor__get_cursor();

    wclear(display);

    uint64_t max_y = getmaxy(display);
    uint64_t start = (cursor->y > max_y ? cursor->y - max_y : 0) + cursor->offset.y;

    // rehydrate only visible area for a better performance
    for (uint64_t i = start, j = 0; i < start + max_y; i++, j++) {
        if (EDITOR.buffer.lines > i) {
            Str line = EDITOR.buffer.data[i];

            mvwprintw(display, j, 0, "%s", line.data);
        }
    }
}

void
TUI__render_statusbar() {
    Display display = EDITOR.display_list.data[STATUS_BAR_DISPLAY];
    Cursor *cursor = Editor__get_cursor();

    wclear(display.win);

    if (EDITOR.mode != MODE__COMMAND) {
        mvwprintw(display.win, 0, 0, "[%s]", Editor__mode_as_str(EDITOR.mode));
    } else {
        mvwprintw(display.win, 0, 0, ":%s", EDITOR.commander.cmd.data);
    }

    char cursor_info[20];
    sprintf(cursor_info, "%zu:%zu", cursor->y + 1, cursor->x + 1);

    mvwprintw(display.win, 0, getmaxx(display.win) - strlen(cursor_info), "%s", cursor_info);
}

void
TUI__refresh_displays() {
    for (int i = 0; i < EDITOR.display_list.len; i++) {
        wrefresh(EDITOR.display_list.data[i].win);
    }

    wrefresh(stdscr);
}

void
TUI__exit() {
    refresh();
    endwin();
}
