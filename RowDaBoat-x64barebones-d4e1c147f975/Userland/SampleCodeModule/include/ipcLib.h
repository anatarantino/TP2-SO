#ifndef IPCLIB_H
#define IPCLIB_H

int pinit();
int popen(char * name);
int pclose(int index);
char pread(int index);
int pwrite(int index, char c);
void plist();
void catFunc();
void wcFunc();
void filterFunc();

#endif