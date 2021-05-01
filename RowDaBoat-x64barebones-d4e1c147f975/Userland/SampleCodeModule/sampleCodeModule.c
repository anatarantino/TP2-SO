#include <shell.h>
#include <prints.h>
#include <processLib.h>
#include <syscall.h>

int main() {
	char * argv[] = {"Shell"};
	addProcess(&startShell,1,argv,1);
	return 0;
}
