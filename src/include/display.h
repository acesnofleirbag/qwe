#ifndef QUICKWARE_DISPLAY_H
#define QUICKWARE_DISPLAY_H

#include "cursor.h"
#include <curses.h>
#include <stdint.h>

#define EDITOR_DISPLAY 0
#define STATUS_BAR_DISPLAY 1

typedef enum display_type {
    DISPLAY_TYPE__ROOT,
    DISPLAY_TYPE__STATUS_BAR,
    DISPLAY_TYPE__FILETREE,
} display_type_t;

typedef struct display {
    display_type_t type;
    WINDOW *win;
    cursor_t cursor;
} display_t;

typedef struct display_list {
    uint8_t len;
    display_t *data;
} display_list_t;

display_list_t display_list__new();
void display_list__add(display_list_t *list, display_t display);
void display_list__release();

#endif
