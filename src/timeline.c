#include "include/timeline.h"
#include "include/editor.h"
#include <stdlib.h>
#include <string.h>

Timeline
Timeline__new() {
    return (Timeline) {
        .len = 0,
        .data = calloc(1, sizeof(char)),
        .capacity = 1,
    };
}

// FIXME: should recv a Frame as argument
void
Timeline__push(char ch) {
    Timeline *timeline = &EDITOR.timeline;

    bool is_full = timeline->capacity == TIMELINE_LIMIT;

    if (is_full) {
        memmove(&timeline->data[0], &timeline->data[1], timeline->len - 1);
    }

    if (timeline->len == timeline->capacity && !is_full) {
        unsigned long new_capacity = timeline->capacity * 2;

        timeline->data = (char *) realloc(timeline->data, new_capacity);
        timeline->capacity += new_capacity;
    }

    timeline->data[timeline->len] = ch;
    timeline->len += 1;
}

char *
Timeline__pop() {
    Timeline *timeline = &EDITOR.timeline;

    timeline->len -= 1;

    return &timeline->data[timeline->len];
}

void
Timeline__release() {
    Timeline *timeline = &EDITOR.timeline;

    free(timeline->data);
    timeline->data = NULL;
}
