#include <memoryLib.h>

void * memalloc(uint32_t size){
    syscalls(MALLOC,size,0,0,0,0,0);
}

void memfree(void * address){
    syscalls(FREE,address,0,0,0,0,0);
}