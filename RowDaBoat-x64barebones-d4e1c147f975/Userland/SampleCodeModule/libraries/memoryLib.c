#include <memoryLib.h>

void * memalloc(uint32_t size){
    return (void *) syscalls(MALLOC,size,0,0,0,0,0);
}

void memfree(void * address){
    syscalls(FREE,(uint64_t)address,0,0,0,0,0);
}