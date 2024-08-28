#include "include/editor.h"
#include "include/ascii.h"
#include "include/buffer.h"
#include "include/cursor.h"
#include "include/debug.h"
#include "include/mem.h"
#include "include/str.h"
#include "include/tui.h"
#include "include/viewport.h"
#include <curses.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

Editor
Editor__new() {
    return (Editor) {
        .exit = false,
        .mode = MODE__NORMAL,
        .viewport = (Viewport) {.x = 0, .y = 0},
        .cursor = Cursor__new(),
        .finder = Finder__new(),
        .buffer = Buffer__new(),
    };
}

void
Editor__run(Editor *editor) {
    TUI__start(editor);

    while (!editor->exit) {
        int ch = getch();

        Editor__compute(editor, ch);
    }

    Editor__release(editor);

    TUI__exit();
}

void
Editor__compute(Editor *editor, const int ch) {
    Mode mode = editor->mode;

    switch (mode) {
        case MODE__NORMAL:
        case MODE__VISUAL:
            switch (ch) {
                case 'i':
                    editor->mode = MODE__INSERT;
                    break;
                case 'v':
                    editor->mode = MODE__VISUAL;
                    break;
                case ':':
                    editor->mode = MODE__COMMAND;
                    break;
                case 'j':
                case KEY_DOWN:
                    Cursor__down(&editor->cursor);
                    break;
                case 'k':
                case KEY_UP:
                    Cursor__up(&editor->cursor);
                    break;
                case 'h':
                case KEY_LEFT:
                    Cursor__left(&editor->cursor);
                    break;
                case 'l':
                case KEY_RIGHT:
                    Cursor__right(&editor->cursor);
                    break;
                case CTRL('q'):
                    editor->exit = true;
                    break;
                case ASCII_ESCAPE:
                    editor->mode = MODE__NORMAL;
                    break;
            }
            break;
        case MODE__INSERT:
            switch (ch) {
                case ASCII_ESCAPE:
                    editor->mode = MODE__NORMAL;
                    break;
                case KEY_BACKSPACE:
                    Cursor__left(&editor->cursor);
                    delch();
                    break;
                case KEY_ENTER:
                    Editor__newline(editor);
                    Cursor__down(&editor->cursor);
                    break;
                case KEY_UP:
                    Cursor__up(&editor->cursor);
                    break;
                case KEY_RIGHT:
                    Cursor__right(&editor->cursor);
                    break;
                case KEY_DOWN:
                    Cursor__down(&editor->cursor);
                    break;
                case KEY_LEFT:
                    Cursor__left(&editor->cursor);
                    break;
                default:
                    addch(ch);
                    Editor__addch(editor, ch);
                    Cursor__right(&editor->cursor);
                    break;
            }
            break;
        case MODE__COMMAND:
            switch (ch) {
                case ASCII_ESCAPE:
                    editor->mode = MODE__NORMAL;
                    break;
            }
            break;
    }
}

void
Editor__newline(Editor *editor) {
    editor->buffer.data = editor->buffer.data == NULL ? MEM__request(sizeof(Str))
                                                      : MEM__expand(editor->buffer.data,
                                                                    sizeof(Str) * (editor->buffer.lines + 1),
                                                                    sizeof(Str) * editor->buffer.lines,
                                                                    CASTMODE__CHAR);

    Str line = String__new(1);
    editor->buffer.data[editor->buffer.lines] = line;

    editor->buffer.lines += 1;
}

void
Editor__addch(Editor *editor, int ch) {
    if (editor->buffer.lines == 0) {
        Editor__newline(editor);
    }

    String__append(&editor->buffer.data[editor->cursor.y], (char) ch);
}

const char *
Editor__mode_as_str(Mode mode) {
    const char *modes[] = {
        [MODE__INSERT] = "INSERT",
        [MODE__VISUAL] = "VISUAL",
        [MODE__NORMAL] = "NORMAL",
        [MODE__COMMAND] = "COMMAND",
    };

    return modes[mode];
}

void
Editor__release(Editor *editor) {
    Buffer *item = &editor->buffer;

    while (item) {
        Buffer *next = item->next;

        Buffer__release(item);

        item = next;
    }
}
