#include "include/tui.h"
#include "include/cursor.h"
#include "include/display.h"
#include "include/editor.h"
#include <curses.h>
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

    WINDOW *editor_win = newwin(y_bound - 1, x_bound, 0, 0);
    WINDOW *statusbar_win = newwin(1, x_bound, y_bound - 1, 0);

    scrollok(editor_win, TRUE);

    display_t editor_display = {.type = DISPLAY_TYPE__ROOT, .win = editor_win, .cursor = cursor__new()};
    display_t statusbar_display = {.type = DISPLAY_TYPE__STATUS_BAR, .win = statusbar_win, .cursor = cursor__new()};

    display_list__add(&EDITOR.display_list, editor_display);
    display_list__add(&EDITOR.display_list, statusbar_display);
}

void
tui__render() {
    tui__render_editor();
    tui__render_statusbar();
}

void
tui__render_editor() {
    WINDOW *display = EDITOR.display_list.data[EDITOR.display].win;
    cursor_t *cursor = editor__get_cursor();

    wclear(display);

    uint64_t max_y = getmaxy(display);
    uint64_t start = (cursor->y > max_y ? cursor->y - max_y : 0) + cursor->offset.y;

    // rehydrate only visible area for a better performance
    for (uint64_t i = start, j = 0; i < start + max_y; i++, j++) {
        if (EDITOR.buffer.lines > i) {
            str_t line = EDITOR.buffer.data[i];

            mvwprintw(display, j, 0, "%s", line.data);
        }
    }
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
}

void
tui__refresh_displays() {
    for (int i = 0; i < EDITOR.display_list.len; i++) {
        wrefresh(EDITOR.display_list.data[i].win);
    }
}

void
tui__exit() {
    refresh();
    endwin();
}
