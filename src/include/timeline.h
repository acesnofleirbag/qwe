#ifndef QUICKWARE_TIMELINE_H
#define QUICKWARE_TIMELINE_H

#define TIMELINE_LIMIT (2 << 8)

#include <stdint.h>

typedef struct {
    void *data;
} Frame;

// Timeline is the abstraction to perform undo/redo behavior on the editor
typedef struct {
    uint64_t len;
    uint64_t capacity;
    char *data;
} Timeline;

Timeline Timeline__new();
void Timeline__push(char ch);
char *Timeline__pop();
void Timeline__release();

#endif
