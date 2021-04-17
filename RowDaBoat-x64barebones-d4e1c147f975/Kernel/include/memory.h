#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

void initializeMem(void * base,uint64_t size);
void * memalloc(uint32_t nbytes);
void memfree(void * ptr);
void memdata();

#endif
