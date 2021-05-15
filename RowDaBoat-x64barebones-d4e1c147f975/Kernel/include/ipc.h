#ifndef IPC_H
#define IPC_H

#include <stdint.h>

#define MAX_LEN 20
#define SIZE 1024
#define ERROR -1
#define PIPES_MAX 15

typedef struct pipe_node{
    char name[MAX_LEN];
    char buff[SIZE];
    int activity;
    int processes;
} pipe_node;

typedef struct pipes_array{
    pipe_node parray[PIPES_MAX];
    int size;
    int dim;
    void * newPipe;
    void * endPipe; 
} pipes_array;

int pinit();
int pcreate(char * name);
int popen(char * name);
int pclose();
int pread();
int pwrite();
int plist();

#endif