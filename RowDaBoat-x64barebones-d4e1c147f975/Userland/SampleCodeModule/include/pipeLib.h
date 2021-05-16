#ifndef IPC_H
#define IPC_H

int pinit();
int popen(char * name);
int pclose(int index);
char pread(int index);
int pwrite(int index, char c);
void plist();

#endif