// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <memoryLib.h>

void * memalloc(uint32_t size){
    return (void *) syscalls(MALLOC,size,0,0,0,0,0);
}

void memfree(void * address){
    syscalls(FREE,(uint64_t)address,0,0,0,0,0);
}

void memdata(){
    syscalls(MEMDATA,0,0,0,0,0,0);
}

uint64_t * shm_open(uint64_t id, uint64_t size){
    return (uint64_t *) syscalls(SHM_OPEN,id,size,0,0,0,0);
}  