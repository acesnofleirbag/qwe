#include "include/editor.h"
#include "include/ascii.h"
#include "include/buffer.h"
#include "include/cursor.h"
#include "include/debug.h"
#include "include/str.h"
#include "include/tui.h"
#include "include/viewport.h"
#include <curses.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *
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

static void
Editor__set_mode(Editor *editor, Mode mode) {
    editor->mode = mode;
}

static void
Editor__render_tui(Editor *editor) {
    Editor__render_statusbar(editor);

    for (int i = 0; i < editor->display_list.len; i++) {
        wrefresh(editor->display_list.data[i].window);
    }
}

Editor
Editor__new() {
    return (Editor) {
        .exit = false,
        .mode = MODE__NORMAL,
        .viewport = {.x = 0, .y = 0},
        .cursor = Cursor__new(),
        .finder = Finder__new(),
        .buffer = Buffer__new(),
        .display_list = DisplayList__new(),
        .display = NULL,
        .cmdline = String__new(1),
    };
}

void
Editor__run(Editor *editor) {
    TUI__start(editor);

    while (!editor->exit) {
        int ch = getch();

        Editor__compute(editor, ch);
        Editor__render_tui(editor);
    }

    Editor__release(editor);

    TUI__exit();
}

void
Editor__compute(Editor *editor, const int ch) {
    switch (editor->mode) {
        case MODE__GOTO:
            switch (ch) {
                case 'g':
                    Editor__goto_start(editor);
                    break;
                case 'G':
                    Editor__goto_end(editor);
                    break;
            }
            Editor__set_mode(editor, MODE__NORMAL);
            break;
        case MODE__NORMAL:
        case MODE__VISUAL:
            switch (ch) {
                case 'i':
                    Editor__set_mode(editor, MODE__INSERT);
                    break;
                case 'v':
                    Editor__set_mode(editor, MODE__VISUAL);
                    break;
                case ':':
                    Editor__set_mode(editor, MODE__COMMAND);
                    break;
                case 'j':
                case KEY_DOWN:
                    Editor__mv_cursor(editor, MOVEMENT__DOWN);
                    break;
                case 'k':
                case KEY_UP:
                    Editor__mv_cursor(editor, MOVEMENT__UP);
                    break;
                case 'h':
                case KEY_LEFT:
                    Editor__mv_cursor(editor, MOVEMENT__LEFT);
                    break;
                case 'l':
                case KEY_RIGHT:
                    Editor__mv_cursor(editor, MOVEMENT__RIGHT);
                    break;
                case 'g':
                    Editor__set_mode(editor, MODE__GOTO);
                    break;
                case '$':
                    Editor__goto_endline(editor);
                    break;
                case '0':
                    Cursor__from(&editor->cursor, 0, editor->cursor.y);
                    break;
                case ASCII_ESCAPE:
                    Editor__set_mode(editor, MODE__NORMAL);
                    break;
            }
            break;
        case MODE__INSERT:
            switch (ch) {
                case ASCII_ESCAPE:
                    Editor__set_mode(editor, MODE__NORMAL);
                    break;
                case KEY_BACKSPACE:
                    Editor__delete_char(editor);
                    break;
                case 10:  // KEY_ENTER
                    Editor__newline(editor);
                    break;
                case KEY_UP:
                    Editor__mv_cursor(editor, MOVEMENT__UP);
                    break;
                case KEY_RIGHT:
                    Editor__mv_cursor(editor, MOVEMENT__RIGHT);
                    break;
                case KEY_DOWN:
                    Editor__mv_cursor(editor, MOVEMENT__DOWN);
                    break;
                case KEY_LEFT:
                    Editor__mv_cursor(editor, MOVEMENT__LEFT);
                    break;
                default:
                    Editor__add_char(editor, ch);
                    break;
            }
            break;
        case MODE__COMMAND:
            switch (ch) {
                case 10:  // KEY_ENTER
                    Editor__exec_command(editor);
                    break;
                case KEY_BACKSPACE:
                    String__remove(&editor->cmdline, editor->cmdline.len);
                    break;
                case ASCII_ESCAPE:
                    Editor__set_mode(editor, MODE__NORMAL);
                    break;
                default:
                    String__append(&editor->cmdline, editor->cmdline.len, ch);
                    break;
            }
            break;
    }
}

Str *
Editor__get_current_line(Editor *editor) {
    if (editor->cursor.y < editor->buffer.lines) {
        return &editor->buffer.data[editor->cursor.y];
    }

    return NULL;
}

void
Editor__delete_char(Editor *editor) {
    Str *line = Editor__get_current_line(editor);

    if (line != NULL) {
        String__remove(line, editor->cursor.x);
    }

    Editor__mv_cursor(editor, MOVEMENT__LEFT);
    Editor__redraw_line(editor);
}

void
Editor__newline(Editor *editor) {
    editor->buffer.data = editor->buffer.data == NULL
                              ? calloc(1, sizeof(Str))
                              : realloc(editor->buffer.data, sizeof(Str) * (editor->buffer.lines + 1));

    Str line = String__new(1);
    editor->buffer.data[editor->buffer.lines] = line;
    editor->buffer.lines += 1;

    Editor__mv_cursor(editor, MOVEMENT__DOWN);
}

void
Editor__add_char(Editor *editor, int ch) {
    if (editor->buffer.lines == 0) {
        Editor__newline(editor);
    }

    Str *line = Editor__get_current_line(editor);
    String__append(line, editor->cursor.x, (char) ch);

    Editor__mv_cursor(editor, MOVEMENT__RIGHT);
    Editor__redraw_line(editor);
}

void
Editor__redraw_line(Editor *editor) {
    WINDOW *root = editor->display_list.data[0].window;
    Str *line = Editor__get_current_line(editor);

    wclrtoeol(root);
    mvwprintw(root, editor->cursor.y, 0, "%s", line->data);

    Cursor__from(&editor->cursor, editor->cursor.x, editor->cursor.y);
}

void
Editor__mv_cursor(Editor *editor, Movement movement) {
    switch (movement) {
        case MOVEMENT__UP: {
            uint64_t new_y = editor->cursor.y - 1;

            if (new_y < editor->buffer.lines) {
                Str prev_line = editor->buffer.data[new_y];

                if (editor->cursor.x <= prev_line.len) {
                    Cursor__up(&editor->cursor);
                } else {
                    Cursor__from(&editor->cursor, prev_line.len, new_y);
                }
            }
            break;
            case MOVEMENT__RIGHT: {
                if (editor->cursor.y < editor->buffer.lines) {
                    Str *line = Editor__get_current_line(editor);

                    if ((editor->cursor.x + 1) <= line->len) {
                        Cursor__right(&editor->cursor);
                    }
                }
            } break;
            case MOVEMENT__DOWN: {
                uint64_t new_y = editor->cursor.y + 1;

                if (new_y < editor->buffer.lines) {
                    Str next_line = editor->buffer.data[new_y];

                    if (editor->cursor.x <= next_line.len) {
                        Cursor__down(&editor->cursor);
                    } else {
                        Cursor__from(&editor->cursor, next_line.len, new_y);
                    }
                }
            } break;
            case MOVEMENT__LEFT:
                Cursor__left(&editor->cursor);
                break;
        }
    }
}

void
Editor__render_statusbar(Editor *editor) {
    WINDOW *statusbar = editor->display_list.data[1].window;

    if (!(editor->mode == MODE__COMMAND)) {
        wclrtoeol(statusbar);
        mvwprintw(statusbar, 0, 0, "-- %s --", Editor__mode_as_str(editor->mode));
    } else {
        wclrtoeol(statusbar);
        mvwprintw(statusbar, 0, 0, ":%s", editor->cmdline.data);
    }

    char lineinfo[20];
    sprintf(lineinfo, "%zu:%zu", editor->cursor.y + 1, editor->cursor.x);

    wclrtoeol(statusbar);
    mvwprintw(statusbar, 0, editor->viewport.x - strlen(lineinfo), "%s", lineinfo);
}

void
Editor__goto_endline(Editor *editor) {
    Str *line = Editor__get_current_line(editor);

    if (line != NULL) {
        Cursor__from(&editor->cursor, line->len, editor->cursor.y);
    }
}

void
Editor__goto_start(Editor *editor) {
    Cursor__from(&editor->cursor, 0, 0);
}

void
Editor__goto_end(Editor *editor) {
    Cursor__from(&editor->cursor, 0, editor->buffer.lines);
}

void
Editor__exec_command(Editor *editor) {
    Str cmd = editor->cmdline;

    if (strcmp("q", cmd.data) == 0) {
        editor->exit = true;
    }

    Editor__set_mode(editor, MODE__NORMAL);
}

void
Editor__release(Editor *editor) {
    Buffer *item = &editor->buffer;

    while (item) {
        Buffer *next = item->next;

        Buffer__release(item);

        item = next;
    }

    for (int i = 0; i < editor->display_list.len; i++) {
        delwin(editor->display_list.data[i].window);
    }
}
