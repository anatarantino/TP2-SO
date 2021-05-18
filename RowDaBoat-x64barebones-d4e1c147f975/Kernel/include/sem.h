#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#include <stdint.h>

#define MAX_LEN 20

typedef struct process_node{
    struct process_node * next;
    uint64_t pid;
} process_node;

typedef struct sem_t{
    char name[MAX_LEN];
    uint64_t value;
    struct sem_t * next;
    uint64_t lock;
    uint64_t pcount;
    process_node * firstInLine;
    process_node * lastInLine;
} sem_t;

int sem_init();

int sem_open(char * name, uint64_t value);

int sem_wait(int index);

int sem_post(int index);

int sem_close(int index);

void sem_changeValue(int index, uint64_t value);

void sems_print();

void sem_print(int index);    

#endif