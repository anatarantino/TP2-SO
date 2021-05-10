#ifndef SEMAPHORES_H
#define SEMAPHORES_H

void sem_init();


sem_t * sem_open(char * name, uint64_t value);     //returns semaphore descriptor or failure code

int sem_wait(sem_t * sem);  //if successfull returns 0, else -1 or error

int sem_post(sem_t * sem);  //if successfull returns 0, else -1 or error

int sem_close(sem_t * sem); //if successfull returns 0, else -1 or error

#endif