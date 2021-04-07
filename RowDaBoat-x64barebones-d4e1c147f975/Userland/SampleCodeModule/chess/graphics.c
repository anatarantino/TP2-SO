#include <graphics.h>
#include <prints.h>
#include <syscall.h>
#include <chess_piece.h>
#include <chess.h>
#include <shell.h>
#include <timeRTC.h>

#define WHITE 0xFFFFFF
#define BLACK 0x000000
#define SCREENWIDTH 1024
#define SCREENHEIGHT 768
#define BOARDSIZE 760
#define SQUARESIZE 87
#define STARTX 157
#define ENDX 847
#define STARTY 4
#define ENDY 700

void drawPixel(int x, int y, int color){
    syscalls(DRAW,0,0,color,0,x,y);
}

void drawBoard(int matrix[][8],uint64_t color1, uint64_t color2, int rotation){
    int currentLetter = 6;
    int numCount = 14;
    int ficha;
    int actualColor = color2;
    
    for(int h=0 ; h<SCREENHEIGHT ; h++){
        
        for(int w=0 ; w<SCREENWIDTH; w++){

            if((w>=STARTX && w<=ENDX) && (h>=STARTY && h<= ENDY)){
                
                if(((w-STARTX)%87==0) && (w<ENDX)){                    
                    if((h-STARTY)%87==0 && h < ENDY){
                        if(rotation == 0){
                            ficha = matrix[(h-STARTY)/87][(w-STARTX)/87]; 
                        }else if(rotation==1){
                            ficha = matrix[8-1 -(w-STARTX)/87][(h-STARTY)/87];
                        }else if(rotation==2){ 
                            ficha = matrix[8-1-(h-STARTY)/87][8-1-(w-STARTX)/87]; 
                        }else{
                            ficha = matrix[(w-STARTX)/87][8-1-(h-STARTY)/87]; 
                        }

                        pieces(ficha, w, h, actualColor);
                        
                    }
                }
                actualColor = (actualColor == color2)? color1 :color2;
            }
            if(w==(STARTX - 87) && h%87==0 && h<=ENDY){  
                if(numCount <= 21){
                    drawPiece(numCount,w,h,WHITE,BLACK);
                    numCount++;
                }
            }
            if (h==ENDY && w%87 == 0 && w>=STARTX){
                if(currentLetter <= 13){
                    drawPiece(currentLetter, w-15,h,WHITE,BLACK);
                    currentLetter++;
                }
            }
            
            
        }
    }
}

void pieces(int ficha, int w, int h, int actualColor){

    if(ficha >= 10 && ficha <= 17){
        drawPiece(PAWN,w,h,WHITE,actualColor);
    }
    else if(ficha >= -17 && ficha <= -10){
        drawPiece(PAWN,w,h,BLACK,actualColor);
    }
    else{
        switch(ficha){
                                    
            case WROOK:
                drawPiece(ROOK,w,h,WHITE,actualColor);
                break;
            case WHORSE:
                drawPiece(KNIGHT,w,h,WHITE,actualColor);
                break;
            case WBISHOP:
                drawPiece(BISHOP,w,h,WHITE,actualColor);
                break;
            case WQUEEN:
                drawPiece(QUEEN,w,h,WHITE,actualColor);
                break;
            case WKING:
                drawPiece(KING,w,h,WHITE,actualColor);
                break;
            case BROOK:
                drawPiece(ROOK,w,h,BLACK,actualColor);
                break;
            case BHORSE:
                drawPiece(KNIGHT,w,h,BLACK,actualColor);
                break;
            case BBISHOP:
                drawPiece(BISHOP,w,h,BLACK,actualColor);
                break;
            case BQUEEN:
                drawPiece(QUEEN,w,h,BLACK,actualColor);
                break;
            case BKING:
                drawPiece(KING,w,h,BLACK,actualColor);
                break;                            
            default:
                drawPiece(SQUARE,w,h,BLACK,actualColor);
        
        }
    }
    
}

void drawPiece(int piece, int  x, int  y, int pieceColor, int bgColor){
    int auxX=x;
    int auxY=y;
    const char * char_map = chess_bitMap(piece);
    for(int i=0 ; i<CHAR_WIDTH * CHAR_HEIGHT  ; i++ ){
            switch (char_map[i])
            {
            case ',':
                auxY++;
                auxX=x;
                break;
            case '_': //fondo
                drawPixel(auxX,auxY,bgColor);
                auxX++;
                break;
            default: 
                drawPixel(auxX,auxY,pieceColor);
                auxX++;
                break;
            }
    }
    
}


