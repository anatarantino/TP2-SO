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

typedef struct sem_list{
    sem_t * first;
}sem_list;


int sem_init();

sem_t * sem_open(char * name, uint64_t value);     //returns semaphore descriptor or failure code

int sem_wait(sem_t * sem);  //if successfull returns 0, else -1 or error

int sem_post(sem_t * sem);  //if successfull returns 0, else -1 or error

int sem_close(sem_t * sem); //if successfull returns 0, else -1 or error

void sem_changeValue(sem_t * sem, uint64_t value);

void sems_print();

void sem_print(sem_t * sem);

#endif