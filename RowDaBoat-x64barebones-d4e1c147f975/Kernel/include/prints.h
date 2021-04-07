#ifndef PRINTS_H
#define PRINTS_H

#include <stdint.h>

void putChar(char c);
void putCharColor(char c,uint64_t f_color,uint64_t bg_color);
void syscallWrite(char* str, uint8_t length, uint64_t f_color, uint64_t bg_color);
void syscallWriteOnCurrentPos(char *str,uint8_t length, uint64_t f_color, uint64_t bg_color,uint32_t posx, uint32_t posy );
void printf(char* str);
void printColor(char* str, uint64_t f_color, uint64_t bg_color);
void printColorOnPos(char *str, uint64_t f_color, uint64_t bg_color, uint32_t posX, uint32_t posY);
void printNewLine();
void printHex(uint64_t num);
void printHexColor(uint64_t num, uint64_t f_color, uint64_t bg_color);
void printInt(uint64_t num);
void printIntColor(uint64_t num, uint64_t f_color, uint64_t bg_color);
void printIntOnPos(int num, uint64_t f_color, uint64_t bg_color, uint32_t posX, uint32_t posY);
#endif