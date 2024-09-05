#ifndef QUICKWARE_TIMELINE_H
#define QUICKWARE_TIMELINE_H

#include <stdint.h>

// Timeline is the abstraction to perform undo/redo behavior on the editor. Implemented as a stack data structure
typedef struct {
    uint64_t len;
    void *data;
} Timeline;

Timeline Timeline__new();
void Timeline__release(Timeline *timeline);

#endif
