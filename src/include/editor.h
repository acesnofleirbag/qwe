#ifndef QUICKWARE_EDITOR_H
#define QUICKWARE_EDITOR_H

#include "buffer.h"
#include "cursor.h"
#include "display.h"
#include "finder.h"
#include <stdbool.h>
#include <stdint.h>

#define CTRL(x) ((x) &0x1f)
#define ENTER 10

typedef enum {
    MOVEMENT__UP,
    MOVEMENT__RIGHT,
    MOVEMENT__DOWN,
    MOVEMENT__LEFT,
} Movement;

typedef enum {
    MODE__NORMAL,
    MODE__INSERT,
    MODE__VISUAL,
    MODE__COMMAND,
    MODE__GOTO,
} Mode;

typedef struct {
    bool exit;
    Mode mode;
    int display;
    DisplayList display_list;
    Buffer buffer;
    Finder finder;
    Str cmdline;
} Editor;

Editor Editor__new();
void Editor__run(Editor *editor);
void Editor__mv_cursor(Editor *editor, Movement movement);
void Editor__compute(Editor *editor, int ch);
void Editor__add_char(Editor *editor, int ch);
void Editor__release(Editor *editor);
void Editor__newline(Editor *editor);
void Editor__delete_char(Editor *editor);
Str *Editor__get_current_line(Editor *editor);
void Editor__render_statusbar(Editor *editor);
void Editor__goto_endline(Editor *editor);
void Editor__goto_start(Editor *editor);
void Editor__goto_end(Editor *editor);
void Editor__exec_command(Editor *editor);
void Editor__render_editor(Editor *editor);

#endif
