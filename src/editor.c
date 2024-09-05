#include "include/editor.h"
#include "include/ascii.h"
#include "include/buffer.h"
#include "include/commander.h"
#include "include/cursor.h"
#include "include/debug.h"
#include "include/display.h"
#include "include/goto.h"
#include "include/str.h"
#include "include/timeline.h"
#include "include/tui.h"
#include <curses.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void
Editor__render_tui() {
    TUI__render_editor();
    TUI__render_statusbar();
    TUI__refresh_displays();
}

Editor
Editor__new(Buffer buffer) {
    return (Editor) {
        .exit = false,
        .mode = MODE__NORMAL,
        .display = EDITOR_DISPLAY,
        .finder = Finder__new(),
        .buffer = buffer,
        .display_list = DisplayList__new(),
        .commander = Commander__new(),
        .timeline = Timeline__new(),
    };
}

void
Editor__run() {
    TUI__start();

    while (!EDITOR.exit) {
        int ch = getch();

        Editor__compute(ch);
        Editor__render_tui();
    }

    Editor__release();

    TUI__exit();
}

void
Editor__compute(const int ch) {
    switch (EDITOR.mode) {
        case MODE__GOTO:
            switch (ch) {
                case 'g':
                    Goto__start_file();
                    break;
                case 'G':
                    Goto__end_file();
                    break;
            }
            Editor__set_mode(MODE__NORMAL);
            break;
        case MODE__NORMAL:
        case MODE__VISUAL:
            switch (ch) {
                case 'i':
                    Editor__set_mode(MODE__INSERT);
                    break;
                case 'v':
                    Editor__set_mode(MODE__VISUAL);
                    break;
                case ':':
                    Editor__set_mode(MODE__COMMAND);
                    break;
                case 'j':
                case KEY_DOWN:
                    Editor__mv_cursor(MOVEMENT__DOWN);
                    break;
                case 'k':
                case KEY_UP:
                    Editor__mv_cursor(MOVEMENT__UP);
                    break;
                case 'h':
                case KEY_LEFT:
                    Editor__mv_cursor(MOVEMENT__LEFT);
                    break;
                case 'l':
                case KEY_RIGHT:
                    Editor__mv_cursor(MOVEMENT__RIGHT);
                    break;
                case 'g':
                    Editor__set_mode(MODE__GOTO);
                    break;
                case '$':
                    Goto__end_line();
                    break;
                case '0':
                    Goto__start_line();
                    break;
                case ASCII_ESCAPE:
                    Editor__set_mode(MODE__NORMAL);
                    break;
            }
            break;
        case MODE__INSERT:
            switch (ch) {
                case ASCII_ESCAPE:
                    Editor__set_mode(MODE__NORMAL);
                    break;
                case KEY_BACKSPACE:
                    Editor__delete_char();
                    break;
                case ENTER: {
                    Editor__newline();
                    Cursor *cursor = Editor__get_cursor();
                    Cursor__from(0, cursor->y + 1);
                } break;
                case KEY_UP:
                    Editor__mv_cursor(MOVEMENT__UP);
                    break;
                case KEY_RIGHT:
                    Editor__mv_cursor(MOVEMENT__RIGHT);
                    break;
                case KEY_DOWN:
                    Editor__mv_cursor(MOVEMENT__DOWN);
                    break;
                case KEY_LEFT:
                    Editor__mv_cursor(MOVEMENT__LEFT);
                    break;
                default:
                    Editor__add_char(ch);
                    break;
            }
            break;
        case MODE__COMMAND:
            switch (ch) {
                case ENTER:
                    Commander__eval();
                    break;
                case KEY_BACKSPACE:
                    String__remove(&EDITOR.commander.cmd, EDITOR.commander.cmd.len);
                    break;
                case ASCII_ESCAPE:
                    Editor__set_mode(MODE__NORMAL);
                    break;
                default:
                    String__append(&EDITOR.commander.cmd, EDITOR.commander.cmd.len, ch);
                    break;
            }
            break;
    }
}

void
Editor__mv_cursor(Movement movement) {
    switch (movement) {
        case MOVEMENT__UP:
            Cursor__up();
            break;
        case MOVEMENT__DOWN:
            Cursor__down();
            break;
        case MOVEMENT__RIGHT:
            Cursor__right();
            break;
        case MOVEMENT__LEFT:
            Cursor__left();
            break;
    }
}

void
Editor__set_mode(Mode mode) {
    EDITOR.mode = mode;
}

Cursor *
Editor__get_cursor() {
    return &EDITOR.display_list.data[EDITOR.display].cursor;
}

void
Editor__newline() {
    EDITOR.buffer.data = EDITOR.buffer.data == NULL
                             ? calloc(1, sizeof(Str))
                             : realloc(EDITOR.buffer.data, sizeof(Str) * (EDITOR.buffer.lines + 1));

    Str line = String__new(1);
    EDITOR.buffer.data[EDITOR.buffer.lines] = line;
    EDITOR.buffer.lines += 1;
}

Str *
Editor__get_current_line() {
    Cursor *cursor = Editor__get_cursor();

    if (cursor->y < EDITOR.buffer.lines) {
        return &EDITOR.buffer.data[cursor->y];
    }

    return NULL;
}

void
Editor__add_char(int ch) {
    if (Editor__get_current_line() == NULL) {
        Editor__newline();
    }

    Cursor *cursor = Editor__get_cursor();
    Str *line = Editor__get_current_line();
    String__append(line, cursor->x, (char) ch);

    Editor__mv_cursor(MOVEMENT__RIGHT);
}

void
Editor__delete_char() {
    Cursor *cursor = Editor__get_cursor();
    Str *line = Editor__get_current_line();

    if (line != NULL) {
        String__remove(line, cursor->x);
    }

    Editor__mv_cursor(MOVEMENT__LEFT);
}

const char *
Editor__mode_as_str(Mode mode) {
    const char *modes[] = {
        [MODE__INSERT] = "INSERT",
        [MODE__VISUAL] = "VISUAL",
        [MODE__NORMAL] = "NORMAL",
        [MODE__COMMAND] = "COMMAND",
        [MODE__GOTO] = "GOTO",
    };

    return modes[mode];
}

void
Editor__release() {
    Finder__release();
    Commander__release();
    Timeline__release();
    DisplayList__release();
}
