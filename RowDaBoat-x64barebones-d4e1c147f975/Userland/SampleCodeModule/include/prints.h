#ifndef PRINTS_H
#define PRINTS_H

#include <stdint.h>

void putChar(char c);
void putCharColor(char c,uint64_t f_color,uint64_t bg_color);
void syscallWrite(char* str, uint64_t f_color, uint64_t bg_color);
void printf(char* str);
void printColor(char* str, uint64_t f_color, uint64_t bg_color);
void printColorOnPos(char *str, uint64_t f_color, uint64_t bg_color, uint32_t posX, uint32_t posY);
void printInt(uint64_t num);
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);
uint32_t uintToBaseHexa(uint64_t value, char * buffer, uint32_t base, uint32_t length);
void clearScreen();
uint64_t* getRegisters();
void printHex(uint64_t num);
void clearSpace(uint32_t startx, uint32_t starty, uint32_t endx, uint32_t endy, uint64_t bg_color);
void printIntOnPosColor(uint64_t num,uint64_t f_color,uint64_t bg_color,uint32_t posX, uint32_t posY);
void printTimer(uint64_t seconds,uint64_t posX, uint64_t posY, uint64_t f_color, uint64_t bg_color);
void newln();

#endif