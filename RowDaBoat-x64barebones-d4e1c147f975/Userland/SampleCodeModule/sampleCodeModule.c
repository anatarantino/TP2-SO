// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <shell.h>
#include <prints.h>
#include <processLib.h>
#include <syscall.h>

int main() {
	char * argv[] = {"Shell"};
	addProcess(&startShell,1,argv,1,0);
	return 0;
}
