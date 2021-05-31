#ifndef SHARED_MEMORY
#define SHARED_MEMORY

#include <stdint.h>

uint64_t * shm_open(uint64_t id, uint32_t size);

#endif