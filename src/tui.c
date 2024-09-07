#include "include/tui.h"
#include "include/cursor.h"
#include "include/debug.h"
#include "include/display.h"
#include "include/editor.h"
#include <curses.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void
tui__start() {
    int x_bound;
    int y_bound;

    initscr();
    raw();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    getmaxyx(stdscr, y_bound, x_bound);

    // editor display
    WINDOW *editor_win = newwin(y_bound - 1, x_bound, 0, 0);

    scrollok(editor_win, TRUE);

    int editor_x = 0;
    int editor_y = 0;
    getmaxyx(editor_win, editor_y, editor_x);

    viewport_t editor_viewport = {.x = editor_x, .y = editor_y};
    display_t editor_display = display__new(DISPLAY_TYPE__EDITOR, editor_win, editor_viewport);

    display_list__add(&EDITOR.display_list, editor_display);

    // status bar display
    WINDOW *status_bar_win = newwin(1, x_bound, y_bound - 1, 0);

    int status_bar_x = 0;
    int status_bar_y = 0;
    getmaxyx(status_bar_win, status_bar_y, status_bar_x);

    viewport_t status_bar_viewport = {.x = status_bar_x, .y = status_bar_y};
    display_t status_bar_display = display__new(DISPLAY_TYPE__STATUS_BAR, status_bar_win, status_bar_viewport);

    display_list__add(&EDITOR.display_list, status_bar_display);
}

void
tui__render() {
    tui__render_editor();
    tui__render_statusbar();
}

void
tui__render_editor() {
    display_t display = EDITOR.display_list.data[EDITOR.display];
    cursor_t *cursor = editor__get_cursor();

    wclear(display.win);

    uint64_t max_y = getmaxy(display.win);
    uint64_t start = (cursor->y > max_y ? cursor->y - max_y : 0) + cursor->offset.y;

    // rehydrate only visible area for a better performance
    for (uint64_t i = start, j = 0; i < start + max_y; i++, j++) {
        if (EDITOR.buffer.lines > i) {
            str_t line = EDITOR.buffer.data[i];

            uint64_t nline = editor__line_number_by_mode(start, j);

            mvwprintw(display.win, j, 0, "%zu %s", nline, line.data);
        }
    }

    wrefresh(display.win);
}

void
tui__render_statusbar() {
    display_t display = EDITOR.display_list.data[STATUS_BAR_DISPLAY];
    cursor_t *cursor = editor__get_cursor();

    wclear(display.win);

    if (EDITOR.mode != MODE__COMMAND) {
        mvwprintw(display.win, 0, 0, "[%s]", editor__mode_as_str(EDITOR.mode));
    } else {
        mvwprintw(display.win, 0, 0, ":%s", EDITOR.commander.cmd.data);
    }

    char cursor_info[20];
    sprintf(cursor_info, "%zu:%zu", cursor->y + 1, cursor->x + 1);

    mvwprintw(display.win, 0, getmaxx(display.win) - strlen(cursor_info), "%s", cursor_info);

    wrefresh(display.win);
}

void
tui__exit() {
    refresh();
    endwin();
}
