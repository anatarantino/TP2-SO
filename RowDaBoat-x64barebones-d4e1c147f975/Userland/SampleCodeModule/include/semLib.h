#ifndef SEMLIB_H
#define SEMLIB_H

#include <stdint.h>

int sem_open(char * name, uint64_t value);
int sem_wait(int index);
int sem_post(int index);
int sem_close(int index);
void sem_changeValue(int index, uint64_t value);
void sem_print();

#endif