// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <video_driver.h>
#include <prints.h>
#include <idtLoader.h>
#include <keyboard_driver.h>
#include <colors.h>
#include <exceptions.h>
#include <scheduler.h>
#include <interrupts.h>
#include <memory.h>
#include <sem.h>
#include <ipc.h>

#define HEAP_SIZE (1024*1024*128)

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel; 

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;
static void * const sampleCodeModuleHeapAddress = (void*)0x600000;

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize){
	memset(bssAddress, 0, bssSize);
}

void * getStackBase(){
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary(){
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	clearBSS(&bss, &endOfKernel - &bss);
	return getStackBase();
}

int main(){
	printf("Este mensaje deberia aparecer al principio :X\n");
	load_idt();

	initializeVideo(WHITE,BLACK);

	initExceptionHandler((uint64_t)sampleCodeModuleAddress,getRSP());

    initializeMem(sampleCodeModuleHeapAddress,HEAP_SIZE);

	pinit();

	initializeKD();

    initializeSch();

	char* argv[] = {"Sample Code Module"};
	addProcess(sampleCodeModuleAddress,1,argv,1,0);
	_hlt();
	printf("Error in kernel\n");
	return 0;
}
