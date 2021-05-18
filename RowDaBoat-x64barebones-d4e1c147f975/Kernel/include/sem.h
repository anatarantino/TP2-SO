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

int sem_open(char * name, uint64_t value);     //returns semaphore descriptor or failure code

int sem_wait(int index);  //if successfull returns 0, else -1 or error

int sem_post(int index);  //if successfull returns 0, else -1 or error

int sem_close(int index); //if successfull returns 0, else -1 or error

void sem_changeValue(int index, uint64_t value);

void sems_print();

void sem_print(sem_t * sem);    //este esta bien porque solo se usa en kernel en un lugar donde guardamos semaforos
// chequear que eso no de error though
#endif