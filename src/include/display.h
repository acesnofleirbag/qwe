#ifndef QUICKWARE_DISPLAY_H
#define QUICKWARE_DISPLAY_H

#include "cursor.h"
#include <curses.h>
#include <stdint.h>

#define EDITOR_DISPLAY 0
#define STATUS_BAR_DISPLAY 1

typedef enum display_type {
    DISPLAY_TYPE__EDITOR,
    DISPLAY_TYPE__STATUS_BAR,
    DISPLAY_TYPE__FILE_TREE,
} display_type_t;

// hold the limits of the current display
typedef struct viewport {
    uint64_t x;
    uint64_t y;
} viewport_t;

typedef struct display {
    display_type_t type;
    cursor_t cursor;
    viewport_t viewport;
    WINDOW *win;
} display_t;

typedef struct display_list {
    uint8_t len;
    display_t *data;
} display_list_t;

display_t display__new(display_type_t type, WINDOW *win, viewport_t viewport);

display_list_t display_list__new();
void display_list__add(display_list_t *list, display_t display);
void display_list__release();

#endif
