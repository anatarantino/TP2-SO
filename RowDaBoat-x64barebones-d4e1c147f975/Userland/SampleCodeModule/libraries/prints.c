#include <prints.h>
#include <syscall.h>
#include <strings.h>
#include <colors.h>

#define CHAR_WIDTH 8

void putChar(char c){
    syscalls(WRITE, (uint64_t)&c, 1, WHITE, BLACK, 0, 0);
}

void putCharColor(char c,uint64_t f_color,uint64_t bg_color){
    syscalls(WRITE, (uint64_t)&c, 1, f_color, bg_color, 0, 0);
}

void printf(char* str){
    syscalls(WRITE, (uint64_t)str, strlen(str), WHITE, BLACK, 0, 0);
}

void printHex(uint64_t num){
    char buff[10];
    uintToBaseHexa(num,buff,16,8);
    printf(buff);
}

void printColor(char* str, uint64_t f_color, uint64_t bg_color){
    syscalls(WRITE, (uint64_t)str, strlen(str), f_color, bg_color, 0, 0);
}
void printColorOnPos(char *str, uint64_t f_color, uint64_t bg_color, uint32_t posX, uint32_t posY){
	syscalls(WRITEONSCREEN,(uint64_t)str, strlen(str), f_color, bg_color, posX, posY);
}

void printIntOnPosColor(uint64_t num,uint64_t f_color,uint64_t bg_color,uint32_t posX, uint32_t posY){
    char buff[10];
    uintToBase(num,buff,10);
    printColorOnPos(buff, f_color, bg_color, posX, posY);
}

void printInt(uint64_t num){
    char buff[10];
    uintToBase(num,buff,10);
    printf(buff);
}

void clearScreen(){
	syscalls(CLEAR,0,0,0,0,0,0);
}

void clearSpace(uint32_t startx, uint32_t starty, uint32_t endx, uint32_t endy, uint64_t bg_color){
	syscalls(CLEARSPACE,startx,starty,endx,bg_color,endy,0);
}

void printTimer(uint64_t seconds,uint64_t posX, uint64_t posY, uint64_t f_color, uint64_t bg_color){
    int h = (seconds/3600); 
	int m = (seconds -(3600*h))/60;	
	int s = (seconds -(3600*h)-(m*60));
    char divider = ':';
	if(h<10){
        printIntOnPosColor(0,f_color,bg_color,posX,posY);   
        posX+=CHAR_WIDTH;
    }
    printIntOnPosColor(h,f_color,bg_color,posX,posY);
    posX+=CHAR_WIDTH;
    printColorOnPos(&divider,f_color,bg_color,posX,posY);
    posX+=CHAR_WIDTH;
    if(m<10){
        printIntOnPosColor(0,f_color,bg_color,posX,posY);   
        posX+=CHAR_WIDTH;
    }
    printIntOnPosColor(m,f_color,bg_color,posX,posY);
    posX+=CHAR_WIDTH;
    printColorOnPos(&divider,f_color,bg_color,posX,posY);
    posX+=CHAR_WIDTH;
    if(s<10){
        printIntOnPosColor(0,f_color,bg_color,posX,posY);   
        posX+=CHAR_WIDTH;
    }
    printIntOnPosColor(s,f_color,bg_color,posX,posY);
   
}

void newln(){
    putChar('\n');
}