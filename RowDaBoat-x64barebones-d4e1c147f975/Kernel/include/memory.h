#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

void initializeMem(void * base,uint64_t size);
void * memalloc(unsigned nbytes);
void memfree(void * ptr);
void memdata();

#endif
