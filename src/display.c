#include "include/display.h"
#include <stdlib.h>

DisplayList
DisplayList__new() {
    return (DisplayList) {
        .len = 0,
        .data = NULL,
    };
}

void
DisplayList__add(DisplayList *list, Display display) {
    if (list->data == NULL) {
        list->data = calloc(1, sizeof(Display));
        list->data[0] = display;
    } else {
        list->data = realloc(list->data, sizeof(Display) * (list->len + 1));
        list->data[list->len] = display;
    }

    list->len += 1;
}
