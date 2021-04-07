#include <prints.h>
#include <colors.h>
#include <exceptions.h>
#include <interrupts.h>

#define ZERO_EXCEPTION 0
#define INVALID_OP	6
#define TOTALREGS 17

static void zero_division();
static void invalid_op();
static void printRegisters(uint64_t * registers);
static uint64_t returnRIP,returnRSP;


static char* dataRegisters[] = {"R15: ", "R14: ", "R13: ", "R12: ", "R11: ", "R10: ", "R9:  ", 
						"R8:  ", "RSI: ", "RDI: ", "RBP: ", "RDX: ", "RCX: ", "RBX: ",
                        "RAX: ", "RIP: ", "RSP: "};

void exceptionDispatcher(int exception, uint64_t * registers) {
	switch (exception){
	case ZERO_EXCEPTION:
		zero_division();
		break;
	case INVALID_OP:
		invalid_op();
		break;
	}
	printRegisters(registers);
	printNewLine();
	registers[TOTALREGS-2]=returnRIP;
	registers[TOTALREGS-2+3]=returnRSP;
}

static void zero_division() {
	printColor("\nZERO DIVISION ERROR: division by zero is undefined.\n",YELLOW,RED);
}

static void invalid_op(){
	printColor("\nINVALID OPERATION CODE EXCEPTION.\n",YELLOW,RED);
}

static void printRegisters(uint64_t * registers){
	for(int i=0 ; i < TOTALREGS-1 ; i++){
		printColor(dataRegisters[i],VIOLET,BLACK);
		printHexColor(registers[i],WHITE,BLACK); 
		printNewLine();
	}
	printColor(dataRegisters[TOTALREGS-1],VIOLET,BLACK);
	printHexColor(registers[15 + 3],WHITE,BLACK);
}

void initExceptionHandler(uint64_t rip,uint64_t rsp){
	returnRIP=rip;
	returnRSP=rsp;
}