#ifndef MEMORYLIB_H
#define MEMORYLIB_H

#include <syscall.h>

void * memalloc(uint32_t size);
void memfree(void * address);
void memdata();
uint64_t * shm_open(uint64_t id, uint64_t size);

#endif