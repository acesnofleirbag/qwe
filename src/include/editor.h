#ifndef QUICKWARE_EDITOR_H
#define QUICKWARE_EDITOR_H

#include "buffer.h"
#include "commander.h"
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
    Commander commander;
} Editor;

Editor Editor__new(Buffer buffer);
void Editor__run();
void Editor__compute(int ch);
void Editor__mv_cursor(Movement movement);
void Editor__set_mode(Mode mode);
Cursor *Editor__get_cursor();
void Editor__newline();
Str *Editor__get_current_line();
void Editor__add_char(int ch);
void Editor__delete_char();
const char *Editor__mode_as_str(Mode mode);
void Editor__release();

extern Editor EDITOR;

#endif
