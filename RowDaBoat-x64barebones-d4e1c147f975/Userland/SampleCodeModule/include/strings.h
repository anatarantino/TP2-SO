#ifndef STRINGS_H
#define STRINGS_H

#include <stdint.h>

#define SIZE 100
#define EOF -1
#define FALSE 0
#define TRUE 1 

char getChar();
char waitCharInterruption();
int strlen(char * str);
int stringcmp(char * str1, char * str2);
int scanf(const char* format,...);
uint64_t hexastrToInt(char *str);
char* strToInt(char* string, int* num);
int strtok(char * str, const char delim, char * array[], int arraySize);
void read(char *buff);

#endif