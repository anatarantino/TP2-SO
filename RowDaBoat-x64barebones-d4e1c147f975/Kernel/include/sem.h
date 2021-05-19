#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#include <stdint.h>

#define MAX_LEN 20


int sem_open(char * name, uint64_t value);

int sem_wait(int index);

int sem_post(int index);

int sem_close(int index);

void sem_changeValue(int index, uint64_t value);

void sems_print();

void sem_print(int index);    

#endif