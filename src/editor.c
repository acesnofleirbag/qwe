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
editor__render_tui() {
    tui__render_editor();
    tui__render_statusbar();
    tui__refresh_displays();
}

editor_t
editor__new(buffer_t buffer) {
    return (editor_t) {
        .exit = false,
        .mode = MODE__NORMAL,
        .display = EDITOR_DISPLAY,
        .finder = finder__new(),
        .buffer = buffer,
        .display_list = display_list__new(),
        .commander = commander__new(),
        .timeline = timeline__new(),
    };
}

void
editor__run() {
    tui__start();

    while (!EDITOR.exit) {
        int ch = getch();

        editor__compute(ch);
        editor__render_tui();
    }

    editor__release();

    tui__exit();
}

void
editor__compute(const int ch) {
    switch (EDITOR.mode) {
        case MODE__GOTO:
            switch (ch) {
                case 'g':
                    goto__start_file();
                    break;
                case 'G':
                    goto__end_file();
                    break;
            }
            editor__set_mode(MODE__NORMAL);
            break;
        case MODE__NORMAL:
        case MODE__VISUAL:
            switch (ch) {
                case 'i':
                    editor__set_mode(MODE__INSERT);
                    break;
                case 'v':
                    editor__set_mode(MODE__VISUAL);
                    break;
                case ':':
                    editor__set_mode(MODE__COMMAND);
                    break;
                case 'j':
                case KEY_DOWN:
                    editor__mv_cursor(MOVEMENT__DOWN);
                    break;
                case 'k':
                case KEY_UP:
                    editor__mv_cursor(MOVEMENT__UP);
                    break;
                case 'h':
                case KEY_LEFT:
                    editor__mv_cursor(MOVEMENT__LEFT);
                    break;
                case 'l':
                case KEY_RIGHT:
                    editor__mv_cursor(MOVEMENT__RIGHT);
                    break;
                case 'g':
                    editor__set_mode(MODE__GOTO);
                    break;
                case '$':
                    goto__end_line();
                    break;
                case '0':
                    goto__start_line();
                    break;
                case ASCII_ESCAPE:
                    editor__set_mode(MODE__NORMAL);
                    break;
            }
            break;
        case MODE__INSERT:
            switch (ch) {
                case ASCII_ESCAPE:
                    editor__set_mode(MODE__NORMAL);
                    break;
                case KEY_BACKSPACE:
                    editor__delete_char();
                    break;
                case ENTER: {
                    editor__newline();
                    cursor_t *cursor = editor__get_cursor();
                    cursor__from(0, cursor->y + 1);
                } break;
                case KEY_UP:
                    editor__mv_cursor(MOVEMENT__UP);
                    break;
                case KEY_RIGHT:
                    editor__mv_cursor(MOVEMENT__RIGHT);
                    break;
                case KEY_DOWN:
                    editor__mv_cursor(MOVEMENT__DOWN);
                    break;
                case KEY_LEFT:
                    editor__mv_cursor(MOVEMENT__LEFT);
                    break;
                default:
                    editor__add_char(ch);
                    break;
            }
            break;
        case MODE__COMMAND:
            switch (ch) {
                case ENTER:
                    commander__eval();
                    break;
                case KEY_BACKSPACE:
                    string__remove(&EDITOR.commander.cmd, EDITOR.commander.cmd.len);
                    break;
                case ASCII_ESCAPE:
                    editor__set_mode(MODE__NORMAL);
                    break;
                default:
                    string__append(&EDITOR.commander.cmd, EDITOR.commander.cmd.len, ch);
                    break;
            }
            break;
    }
}

void
editor__mv_cursor(movement_t movement) {
    switch (movement) {
        case MOVEMENT__UP:
            cursor__up();
            break;
        case MOVEMENT__DOWN:
            cursor__down();
            break;
        case MOVEMENT__RIGHT:
            cursor__right();
            break;
        case MOVEMENT__LEFT:
            cursor__left();
            break;
    }
}

void
editor__set_mode(editor_mode_t mode) {
    EDITOR.mode = mode;
}

cursor_t *
editor__get_cursor() {
    return &EDITOR.display_list.data[EDITOR.display].cursor;
}

void
editor__newline() {
    EDITOR.buffer.data = EDITOR.buffer.data == NULL
                             ? calloc(1, sizeof(str_t))
                             : realloc(EDITOR.buffer.data, sizeof(str_t) * (EDITOR.buffer.lines + 1));

    str_t line = string__new(1);
    EDITOR.buffer.data[EDITOR.buffer.lines] = line;
    EDITOR.buffer.lines += 1;
}

str_t *
editor__get_current_line() {
    cursor_t *cursor = editor__get_cursor();

    if (cursor->y < EDITOR.buffer.lines) {
        return &EDITOR.buffer.data[cursor->y];
    }

    return NULL;
}

void
editor__add_char(int ch) {
    if (editor__get_current_line() == NULL) {
        editor__newline();
    }

    cursor_t *cursor = editor__get_cursor();
    str_t *line = editor__get_current_line();
    string__append(line, cursor->x, (char) ch);

    editor__mv_cursor(MOVEMENT__RIGHT);
}

void
editor__delete_char() {
    cursor_t *cursor = editor__get_cursor();
    str_t *line = editor__get_current_line();

    if (line != NULL) {
        string__remove(line, cursor->x);
    }

    editor__mv_cursor(MOVEMENT__LEFT);
}

const char *
editor__mode_as_str(editor_mode_t mode) {
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
editor__release() {
    finder__release();
    commander__release();
    timeline__release();
    display_list__release();
}
