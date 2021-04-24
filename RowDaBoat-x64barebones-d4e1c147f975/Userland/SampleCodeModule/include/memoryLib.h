#ifndef MEMORYLIB_H
#define MEMORYLIB_H

#include <syscall.h>

void * memalloc(uint32_t size);
void memfree(void * address);

#endif