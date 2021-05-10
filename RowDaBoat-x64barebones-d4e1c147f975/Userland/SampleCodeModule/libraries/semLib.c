#include <semLib.h>
#include <syscall.h>

uint64_t * sem_open(char * name, uint64_t value){
    return syscalls(SEM_OPEN,name,value,0,0,0,0);
}

int sem_wait(uint64_t * sem){
return syscalls(SEM_WAIT,sem,0,0,0,0,0);
}

int sem_post(uint64_t * sem){
    return syscalls(SEM_POST,sem,0,0,0,0,0);
}

int sem_close(uint64_t * sem){
    return syscalls(SEM_CLOSE,sem,0,0,0,0,0);
}

void sem_changeValue(uint64_t * sem, uint64_t value){
    syscalls(SEM_CHANGE_VALUE,sem,value,0,0,0,0);
}

void sems_print(){
    syscalls(SEMS_PRINT,0,0,0,0,0,0);
}