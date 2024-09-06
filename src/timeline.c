#include "include/timeline.h"
#include "include/editor.h"
#include <stdlib.h>
#include <string.h>

timeline_t
timeline__new() {
    return (timeline_t) {
        .len = 0,
        .data = calloc(1, sizeof(char)),
        .capacity = 1,
    };
}

// FIXME: should recv a frame_t as argument
void
timeline__push(char ch) {
    timeline_t *timeline = &EDITOR.timeline;

    bool is_full = timeline->len == TIMELINE_LIMIT && timeline->capacity == TIMELINE_LIMIT;

    if (is_full) {
        memmove(&timeline->data[0], &timeline->data[1], timeline->len - 1);
    }

    if (timeline->len == timeline->capacity && !is_full) {
        unsigned long new_capacity = timeline->capacity * 2;

        timeline->data = realloc(timeline->data, new_capacity);
        timeline->capacity += new_capacity;
    }

    timeline->data[timeline->len] = ch;
    timeline->len += 1;
}

char *
timeline__pop() {
    timeline_t *timeline = &EDITOR.timeline;

    timeline->len -= 1;

    return &timeline->data[timeline->len];
}

void
timeline__release() {
    timeline_t *timeline = &EDITOR.timeline;

    free(timeline->data);
    timeline->data = NULL;
}
