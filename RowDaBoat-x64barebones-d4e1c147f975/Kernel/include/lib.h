#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char *cpu_vendor(char *result);

uint64_t getRSP();

void callTimerTick();

//int exchange(uint8_t * lock, uint8_t value);

extern void enter_region(uint8_t * lock);

extern void leave_region(uint8_t * lock);

#endif