#ifndef CHESS_H
#define CHESS_H

#define COLORP1 0xFFFFFF
#define COLORP2 0xF69F08

#define POSP1X 856
#define POSP2X 940
#define POSTITLEY 5
#define POSTIMERY 40
#define POSLOGSY 72
#define DIM 8

#define NOPIECE 0
#define BLACKPIECE 1
#define WHITEPIECE 2
#define INVALIDPOS 3
#define INVALIDMOVE 4

typedef enum{
    WROOK = 2,
    WHORSE = 3,
    WBISHOP = 4,
    WQUEEN = 5,
    WKING = 6,
    BROOK = -2,
    BHORSE = -3,
    BBISHOP = -4,
    BQUEEN = -5,
    BKING = -6
}Pieces;

enum game_state{new_game=0,game_started};

void playChess();

#endif