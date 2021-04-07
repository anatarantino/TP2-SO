#ifndef LIB_H
#define LIB_H

#include <stdint.h>

#define SIZE 100
#define EOF -1

uint64_t* getRegisters();
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);
uint32_t uintToBaseHexa(uint64_t value, char * buffer, uint32_t base, uint32_t length);
void * memset(void * destination, int32_t c, uint64_t length);

#endif