#ifndef SEMLIB_H
#define SEMLIB_H

#include <stdint.h>

int sem_open(char * name, uint64_t value);
int sem_wait(uint64_t index);
int sem_post(uint64_t index);
int sem_close(uint64_t index);
void sem_changeValue(uint64_t index, uint64_t value);
void sem_print();

#endif