// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <ipcLib.h>
#include <syscall.h>
#include <prints.h>
#include <strings.h>

int pinit(){
    return syscalls(PIPE_INIT,0,0,0,0,0,0);
}

int popen(char * name){
    return syscalls(PIPE_OPEN,(uint64_t)name,0,0,0,0,0);
}

int pclose(int index){
    return syscalls(PIPE_CLOSE,index,0,0,0,0,0);
}

char pread(int index){
    return syscalls(PIPE_READ,index,0,0,0,0,0);
}

int pwrite(int index, char *c){
    return syscalls(PIPE_WRITE,index,(uint64_t)c,0,0,0,0);
}

void plist(){
    syscalls(PIPE_LIST,0,0,0,0,0,0);
}

void catFunc(){
    int c;
    while((c=getChar()) != -1){
        putChar(c);
    }
}

void wcFunc(){
    int c;
    int lines = 0;
    while((c=getChar()) != -1){
        putChar(c);
        if(c == '\n'){
            lines++;
        }
    }
    printf("\nTotal lines: ");
    printInt(lines);
}

void filterFunc(){
    int c;
    while((c=getChar()) != -1){
        if(c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u' && c != 'A' && c != 'E' && c != 'I' && c != 'O' && c != 'U'){
            putChar(c);
        }
    }
}