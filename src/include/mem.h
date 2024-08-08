#ifndef QUICKWARE_MEMORY_H
#define QUICKWARE_MEMORY_H

#include <stddef.h>

void* MEM__request(unsigned long size);
void* MEM__refresh(void* target, unsigned long size);
void MEM__release(void* target);

#endif
