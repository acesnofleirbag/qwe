#ifndef QUICKWARE_EDITOR_H
#define QUICKWARE_EDITOR_H

#include "buffer.h"
#include "cursor.h"
#include "finder.h"
#include "viewport.h"
#include <curses.h>
#include <stdbool.h>

#define CTRL(x) ((x) &0x1f)

typedef enum {
    MODE__NORMAL,
    MODE__INSERT,
    MODE__VISUAL,
    MODE__COMMAND,
} Mode;

typedef struct {
    Viewport viewport;
    Cursor cursor;
    Buffer buffer;
    Finder finder;
    Mode mode;
    bool exit;
} Editor;

Editor Editor__new();
void Editor__run(Editor *editor);
void Editor__mv_cursor();
void Editor__compute(Editor *editor, int ch);
void Editor__addch(Editor *editor, int ch);
const char *Editor__mode_as_str(Mode mode);
void Editor__release(Editor *editor);
void Editor__newline(Editor *editor);

#endif
