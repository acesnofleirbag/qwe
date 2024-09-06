#ifndef QUICKWARE_TIMELINE_H
#define QUICKWARE_TIMELINE_H

#define TIMELINE_LIMIT (2 << 8)

#include <stdint.h>

typedef struct frame {
    void *data;
} frame_t;

// Timeline is the abstraction to perform undo/redo behavior on the editor
typedef struct timeline {
    uint64_t len;
    uint64_t capacity;
    char *data;
} timeline_t;

timeline_t timeline__new();
void timeline__push(char ch);
char *timeline__pop();
void timeline__release();

#endif
