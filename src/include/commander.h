#ifndef QUICKWARE_COMMANDER_H
#define QUICKWARE_COMMANDER_H

#include "str.h"

typedef struct {
    Str cmd;
} Commander;

Commander Commander__new();
void Commander__eval();
void Commander__release();

#endif
