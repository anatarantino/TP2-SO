#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>

void drawPixel(int x, int y, int color);

void drawPiece(int piece, int x, int y, int pieceColor, int bgColor);
void drawBoard(int matrix[][8],uint64_t color1, uint64_t color2,int rotation);

void pieces(int ficha, int w, int h, int actualColor);



#endif