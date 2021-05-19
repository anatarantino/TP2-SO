#include <semLib.h>
#include <syscall.h>

int sem_open(char * name, uint64_t value){
    return syscalls(SEM_OPEN,(uint64_t)name,value,0,0,0,0);
}

int sem_wait(int index){
return syscalls(SEM_WAIT,index,0,0,0,0,0);
}

int sem_post(int index){
    return syscalls(SEM_POST,index,0,0,0,0,0);
}

int sem_close(int index){
    return syscalls(SEM_CLOSE,index,0,0,0,0,0);
}

void sem_changeValue(int index, uint64_t value){
    syscalls(SEM_CHANGE_VALUE,index,value,0,0,0,0);
}

void sem_print(){
    syscalls(SEMS_PRINT,0,0,0,0,0,0);
}