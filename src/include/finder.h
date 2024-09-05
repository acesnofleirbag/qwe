#ifndef QUICKWARE_FINDER_H
#define QUICKWARE_FINDER_H

#include "str.h"

typedef enum {
    FIND_MODE__FORWARD,
    FIND_MODE__BACKWARD,
} FindMode;

typedef struct {
    FindMode mode;
    Str query;
} Finder;

Finder Finder__new();
void Finder__release();

#endif
