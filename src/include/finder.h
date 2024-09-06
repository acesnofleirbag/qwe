#ifndef QUICKWARE_FINDER_H
#define QUICKWARE_FINDER_H

#include "str.h"

typedef enum find_mode {
    FIND_MODE__FORWARD,
    FIND_MODE__BACKWARD,
} find_mode_t;

typedef struct {
    find_mode_t mode;
    str_t query;
} finder_t;

finder_t finder__new();
void finder__release();

#endif
