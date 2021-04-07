#ifndef VIDEO_DRIVER_H
#define VIDEO_DRIVER_H

#include <font.h>
#include <stdint.h>
#include <lib.h>
#include <prints.h>
#include <lib.h>

#define SCREEN_HEIGHT 768
#define SCREEN_WIDTH 1024
#define PIXEL_SIZE 3

void initializeVideo(int bg_color,int f_color);
void drawPixel(int x,int y,int color);
void printChar(char c, uint64_t f_color, uint64_t bg_color);
void printCharOnScreen(char c, uint64_t f_color, uint64_t bg_color,uint32_t posX,uint32_t posY);
void deleteChar(uint64_t bg_color);
void newLine();
void clearScreen(uint64_t bg_color);
void scrollScreen();
void clearSpace(uint32_t startx, uint32_t starty, uint32_t endx, uint32_t endy, uint64_t bg_color);

#endif