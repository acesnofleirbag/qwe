#ifndef QUICKWARE_DISPLAY_LIST_H
#define QUICKWARE_DISPLAY_LIST_H

#include "cursor.h"
#include <curses.h>
#include <stdint.h>

#define EDITOR_DISPLAY 0
#define STATUS_BAR_DISPLAY 1

typedef enum {
    DISPLAY_TYPE__ROOT,
    DISPLAY_TYPE__STATUS_BAR,
    DISPLAY_TYPE__FILETREE,
} DisplayType;

typedef struct {
    DisplayType type;
    WINDOW *win;
    Cursor cursor;
} Display;

typedef struct {
    uint8_t len;
    Display *data;
} DisplayList;

DisplayList DisplayList__new();
void DisplayList__add(DisplayList *list, Display display);
void DisplayList__refresh_displays(DisplayList *list);

#endif
