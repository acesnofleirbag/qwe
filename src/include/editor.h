#ifndef QUICKWARE_EDITOR_H
#define QUICKWARE_EDITOR_H

#include "buffer.h"
#include "commander.h"
#include "cursor.h"
#include "display.h"
#include "finder.h"
#include "timeline.h"
#include <stdbool.h>
#include <stdint.h>

#define CTRL(x) ((x) &0x1f)
#define ENTER 10

typedef enum movement {
    MOVEMENT__UP,
    MOVEMENT__RIGHT,
    MOVEMENT__DOWN,
    MOVEMENT__LEFT,
} movement_t;

typedef enum editor_mode {
    MODE__NORMAL,
    MODE__INSERT,
    MODE__VISUAL,
    MODE__COMMAND,
    MODE__GOTO,
} editor_mode_t;

typedef struct editor {
    bool exit;
    editor_mode_t mode;
    int display;
    display_list_t display_list;
    buffer_t buffer;
    finder_t finder;
    commander_t commander;
    timeline_t timeline;
} editor_t;

editor_t editor__new(buffer_t buffer);
void editor__run();
void editor__compute(int ch);
void editor__mv_cursor(movement_t movement);
void editor__set_mode(editor_mode_t mode);
cursor_t *editor__get_cursor();
void editor__newline();
str_t *editor__get_current_line();
void editor__add_char(int ch);
void editor__delete_char();
const char *editor__mode_as_str(editor_mode_t mode);
void editor__release();

extern editor_t EDITOR;

#endif
