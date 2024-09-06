#ifndef QUICKWARE_COMMANDER_H
#define QUICKWARE_COMMANDER_H

#include "str.h"

typedef struct commander {
    str_t cmd;
} commander_t;

commander_t commander__new();
void commander__eval();
void commander__release();

#endif
