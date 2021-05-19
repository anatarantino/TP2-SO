#ifndef SHELL_H
#define SHELL_H

#include <stdint.h>

typedef struct t_command {
    void (*command)(int, char **);
    char *name;
    char *description;
}t_command;

int startShell(int argc, char *argv[]);
void invalidAmount();

#endif