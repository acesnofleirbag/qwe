#include "include/display.h"
#include "include/editor.h"
#include <stdlib.h>

display_list_t
display_list__new() {
    return (display_list_t) {
        .len = 0,
        .data = NULL,
    };
}

void
display_list__add(display_list_t *list, display_t display) {
    if (list->data == NULL) {
        list->data = calloc(1, sizeof(display_t));
        list->data[0] = display;
    } else {
        list->data = realloc(list->data, sizeof(display_t) * (list->len + 1));
        list->data[list->len] = display;
    }

    list->len += 1;
}

void
display_list__release() {
    for (int i = 0; i < EDITOR.display_list.len; i++) {
        delwin(EDITOR.display_list.data[i].win);
    }
}
