#ifndef IPC_H
#define IPC_H

#include <stdint.h>
#include <strings.h>

#define MAX_LEN 20
#define MAX_BUFF 1024   // lo maximo que puede escribir
#define ERROR -1
#define PIPES_MAX 15    

typedef struct pipe_node{
    char name[MAX_LEN];
    char buff[MAX_BUFF];
    int activity;
    int processes;
    int readSem;
    int writeSem;
    int writePos;
    int readPos;
} pipe_node;

typedef struct pipes_array{
    pipe_node parray[PIPES_MAX];
    int lockPipe;
} pipes_array;

int pinit();
int popen(char * name);
int pclose(int index);
char pread(int index);
int pwriteChar(int index, char c);
int pwriteStr(int index, char * str);
void plist();
char getCharP();
void printfP(char* str, uint8_t length, uint64_t f_color, uint64_t bg_color);

#endif