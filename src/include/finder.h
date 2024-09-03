#ifndef QUICKWARE_FINDER_H
#define QUICKWARE_FINDER_H

#include "str.h"

typedef struct {
    Str query;
} Finder;

Finder Finder__new();

#endif
