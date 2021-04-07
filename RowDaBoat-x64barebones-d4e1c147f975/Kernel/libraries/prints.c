#include <stdint.h>
#include <video_driver.h>
#include <prints.h>
#include <strings.h>
#include <colors.h>

void syscallWrite(char* str, uint8_t length, uint64_t f_color, uint64_t bg_color){
    
    for(int i=0; str[i]!= 0 && i < length; i++){
        if(str[i] == '\n'){
            newLine();
        }else if(str[i] == '\b'){
            deleteChar(bg_color);
        }else if(str[i] == '\t'){
            for(int i =0 ; i<3 ; i++){
                printChar(' ',f_color,bg_color);
            }
        }else{
            printChar(str[i],f_color,bg_color);
        }
    }
}

void syscallWriteOnCurrentPos(char *str,uint8_t length, uint64_t f_color, uint64_t bg_color,uint32_t posx, uint32_t posy ){
    uint32_t actualX = posx;
    uint32_t actualY = posy;
   
    for(int i=0; str[i]!= 0 && i < length; i++){
        if(str[i] == '\n'){
            actualY +=CHAR_HEIGHT;
            actualX = posx;
        }else if(str[i] == '\b'){
            actualX -= CHAR_WIDTH;
            printCharOnScreen(' ',f_color,bg_color,actualX,actualY);

        }else if(str[i] == '\t'){
            for(int i =0 ; i<3 ; i++){
                printCharOnScreen(' ',f_color,bg_color,actualX,actualY);
                actualX+=CHAR_WIDTH;
            }
        }else{
            if(actualX + CHAR_WIDTH == SCREEN_WIDTH){
                actualX = posx;
                actualY+=CHAR_HEIGHT;
            }
            printCharOnScreen(str[i],f_color,bg_color,actualX,actualY);
            actualX+=CHAR_WIDTH;
        }
    }
}

void putChar(char c){
    syscallWrite(&c,1,WHITE,BLACK);
}

void putCharColor(char c,uint64_t f_color,uint64_t bg_color){
    syscallWrite(&c,1,f_color,bg_color);
}


void printf(char* str){
    syscallWrite(str,strlen(str),WHITE,BLACK);
}

void printColor(char* str, uint64_t f_color, uint64_t bg_color){
    syscallWrite(str,strlen(str),f_color,bg_color);
}

void printColorOnPos(char *str, uint64_t f_color, uint64_t bg_color, uint32_t posX, uint32_t posY){
    syscallWriteOnCurrentPos(str,strlen(str),f_color,bg_color,posX,posY);
}

void printNewLine(){
    putChar('\n');
}

void printHex(uint64_t num){
    char buff[10]={0};
    uintToBase(num,buff,16);
    printf(buff);
}

void printHexColor(uint64_t num, uint64_t f_color, uint64_t bg_color){
    char buff[10]={0};
    uintToBaseHexa(num,buff,16,8);
    printColor(buff,f_color,bg_color);
}

void printInt(uint64_t num){
    char buff[10]={0};
    uintToBase(num,buff,10);
    printf(buff);
}


void printIntColor(uint64_t num, uint64_t f_color, uint64_t bg_color){
    char buff[10]={0};
    uintToBase(num,buff,10);
    printColor(buff,f_color,bg_color);
}

void printIntOnPos(int num, uint64_t f_color, uint64_t bg_color, uint32_t posX, uint32_t posY){
    char buff[10]={0};
    uintToBase(num,buff,10);
    printColorOnPos(buff,f_color,bg_color,posX,posY);
}
