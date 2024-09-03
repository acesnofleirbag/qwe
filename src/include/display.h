#ifndef QUICKWARE_DISPLAY_LIST_H
#define QUICKWARE_DISPLAY_LIST_H

#include <curses.h>
#include <stdint.h>

typedef enum {
    DISPLAY_TYPE__ROOT,
    DISPLAY_TYPE__STATUS_BAR,
    DISPLAY_TYPE__FILETREE,
} DisplayType;

typedef struct {
    DisplayType type;
    WINDOW *window;
} Display;

typedef struct {
    uint8_t len;
    Display *data;
} DisplayList;

DisplayList DisplayList__new();
void DisplayList__add(DisplayList *list, Display display);

#endif
