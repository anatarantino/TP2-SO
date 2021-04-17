#include <memoryLib.h>

void * malloc(uint32_t size){
    syscalls(MALLOC,size,0,0,0,0,0);
}

void free(void * address){
    syscalls(FREE,address,0,0,0,0,0);
}