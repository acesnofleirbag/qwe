#include "include/editor.h"
#include "include/ascii.h"
#include "include/buffer.h"
#include "include/cursor.h"
#include "include/debug.h"
#include "include/display.h"
#include "include/str.h"
#include "include/tui.h"
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
    Editor__render_editor(editor);
    DisplayList__refresh_displays(&editor->display_list);
}

static Cursor *
Editor__get_cursor(Editor *editor) {
    return &editor->display_list.data[editor->display].cursor;
}

Editor
Editor__new() {
    return (Editor) {
        .exit = false,
        .mode = MODE__NORMAL,
        .finder = Finder__new(),
        .buffer = Buffer__new(),
        .display_list = DisplayList__new(),
        .display = EDITOR_DISPLAY,
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
                case '0': {
                    Cursor *cursor = Editor__get_cursor(editor);
                    Cursor__from(cursor, 0, cursor->y);
                } break;
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
                case ENTER: {
                    Editor__newline(editor);
                    Cursor *cursor = Editor__get_cursor(editor);
                    Cursor__from(cursor, 0, cursor->y + 1);
                } break;
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
                case ENTER:
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

void
Editor__mv_cursor(Editor *editor, Movement movement) {
    WINDOW *display = editor->display_list.data[editor->display].win;
    Cursor *cursor = Editor__get_cursor(editor);

    switch (movement) {
        case MOVEMENT__UP: {
            if (cursor->offset.y > 0 && cursor->y == cursor->offset.y) {
                wscrl(display, -1);
                cursor->offset.y -= 1;
            }

            Cursor__up(cursor);
            break;
            case MOVEMENT__DOWN: {
                uint64_t max_y = getmaxy(display);
                uint64_t new_y = cursor->y + 1;

                if (new_y < max_y) {
                    Cursor__down(cursor);
                } else {
                    cursor->y += 1;

                    wscrl(display, 1);
                    cursor->offset.y += 1;
                }
            } break;
            case MOVEMENT__RIGHT: {
                Str *line = Editor__get_current_line(editor);

                if (line != NULL) {
                    if (cursor->x < line->len) {
                        Cursor__right(cursor);
                    }
                }
            } break;
            case MOVEMENT__LEFT:
                Cursor__left(cursor);
                break;
        }
    }
}

void
Editor__newline(Editor *editor) {
    editor->buffer.data = editor->buffer.data == NULL
                              ? calloc(1, sizeof(Str))
                              : realloc(editor->buffer.data, sizeof(Str) * (editor->buffer.lines + 1));

    Str line = String__new(1);
    editor->buffer.data[editor->buffer.lines] = line;
    editor->buffer.lines += 1;
}

Str *
Editor__get_current_line(Editor *editor) {
    Cursor *cursor = Editor__get_cursor(editor);

    if (cursor->y < editor->buffer.lines) {
        return &editor->buffer.data[cursor->y];
    }

    return NULL;
}

void
Editor__add_char(Editor *editor, int ch) {
    if (Editor__get_current_line(editor) == NULL) {
        Editor__newline(editor);
    }

    Cursor *cursor = Editor__get_cursor(editor);
    Str *line = Editor__get_current_line(editor);
    String__append(line, cursor->x, (char) ch);

    Editor__mv_cursor(editor, MOVEMENT__RIGHT);
}

void
Editor__delete_char(Editor *editor) {
    Cursor *cursor = Editor__get_cursor(editor);
    Str *line = Editor__get_current_line(editor);

    if (line != NULL) {
        String__remove(line, cursor->x);
    }

    Editor__mv_cursor(editor, MOVEMENT__LEFT);
}

void
Editor__goto_endline(Editor *editor) {
    Cursor *cursor = Editor__get_cursor(editor);
    Str *line = Editor__get_current_line(editor);

    if (line != NULL) {
        Cursor__from(cursor, line->len, cursor->y);
    }
}

void
Editor__goto_start(Editor *editor) {
    Cursor *cursor = Editor__get_cursor(editor);

    Cursor__from(cursor, 0, 0);
}

void
Editor__goto_end(Editor *editor) {
    Cursor *cursor = Editor__get_cursor(editor);

    Cursor__from(cursor, 0, editor->buffer.lines);
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
Editor__render_editor(Editor *editor) {
    WINDOW *display = editor->display_list.data[editor->display].win;
    Cursor *cursor = Editor__get_cursor(editor);

    wclear(display);

    uint64_t max_y = getmaxy(display);
    uint64_t start = (cursor->y > max_y ? cursor->y - max_y : 0) + cursor->offset.y;

    // rehydrate only visible area for a better performance
    for (uint64_t i = start, j = 0; i < start + max_y; i++, j++) {
        if (editor->buffer.lines > i) {
            Str line = editor->buffer.data[i];

            mvwprintw(display, j, 0, "%s", line.data);
        }
    }
}

void
Editor__render_statusbar(Editor *editor) {
    Display display = editor->display_list.data[STATUS_BAR_DISPLAY];
    Cursor *cursor = Editor__get_cursor(editor);

    wclear(display.win);

    if (editor->mode != MODE__COMMAND) {
        mvwprintw(display.win, 0, 0, "[%s]", Editor__mode_as_str(editor->mode));
    } else {
        mvwprintw(display.win, 0, 0, ":%s", editor->cmdline.data);
    }

    char cursor_info[20];
    sprintf(cursor_info, "%zu:%zu", cursor->y + 1, cursor->x + 1);

    mvwprintw(display.win, 0, getmaxx(display.win) - strlen(cursor_info), "%s", cursor_info);
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
        delwin(editor->display_list.data[i].win);
    }
}
