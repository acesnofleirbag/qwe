#include "include/editor.h"
#include "include/ascii.h"
#include "include/buffer.h"
#include "include/cursor.h"
#include "include/mem.h"
#include "include/tui.h"
#include "include/viewport.h"
#include <curses.h>
#include <stdbool.h>

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

    Editor__clean(editor);

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
                default:
                    addch(ch);
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
Editor__clean(Editor *editor) {
    Buffer *root = &editor->buffer;

    while (root->next) {
        Buffer *erase = root;
        root = root->next;

        MEM__release(erase);
    }

    MEM__release(root);
}
