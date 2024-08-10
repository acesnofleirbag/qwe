#ifndef QUICKWARE_ERROR_H
#define QUICKWARE_ERROR_H

#include <stdio.h>

void panic(const char *format, ...);
void err(const char *format, ...);

#endif
