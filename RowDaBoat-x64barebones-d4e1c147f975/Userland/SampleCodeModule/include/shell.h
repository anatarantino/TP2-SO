#ifndef SHELL_H
#define SHELL_H

#include <stdint.h>

typedef struct t_command {
    void (*command)(int, char **);
    char *name;
}t_command;

void startShell(int argc, char *argv[]);

#endif