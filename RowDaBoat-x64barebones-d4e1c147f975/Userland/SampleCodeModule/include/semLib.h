#ifndef SEMLIB_H
#define SEMLIB_H

#include <stdint.h>

uint64_t * sem_open(char * name, uint64_t value);
int sem_wait(uint64_t * sem);
int sem_post(uint64_t * sem);
int sem_close(uint64_t * sem);
void sem_changeValue(uint64_t * sem, uint64_t value);
void sems_print();

#endif