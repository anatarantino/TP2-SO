#ifndef MEMORYLIB_H
#define MEMORYLIB_H

#include <stdlib.h>
#include <syscall.h>

void * malloc(uint32_t size);
void free(void * address);

#endif