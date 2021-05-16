#include <pipeLib.h>
#include <syscall.h>

int pinit(){
    return syscalls(PIPE_INIT,0,0,0,0,0,0);
}

int popen(char * name){
    return syscalls(PIPE_OPEN,name,0,0,0,0,0);
}

int pclose(int index){
    return syscalls(PIPE_CLOSE,index,0,0,0,0,0);
}

char pread(int index){
    return syscalls(PIPE_READ,index,0,0,0,0,0);
}

int pwrite(int index, char c){
    return syscalls(PIPE_WRITE,index,c,0,0,0,0);
}

void plist(){
    syscalls(PIPE_LIST,0,0,0,0,0,0);
}
