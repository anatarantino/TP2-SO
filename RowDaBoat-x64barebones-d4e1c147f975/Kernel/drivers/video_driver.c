#include <video_driver.h>

struct vbe_mode_info_structure {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;

	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;

	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

struct vbe_mode_info_structure * screenData = (void *)0x0000000000005C00; //definida en sysvar.asm - Bootloader

typedef struct{
	uint32_t current_x;
	uint32_t current_y;
	int default_bg_color;
	int default_f_color;
	uint32_t offset;
	uint32_t height;
}screen_t;

screen_t sc;

void initializeVideo(int f_color,int bg_color){
	sc.current_x =0;
	sc.current_y=0;
	sc.default_bg_color = bg_color;
	sc.default_f_color = f_color;
	sc.offset = CHAR_WIDTH;
	sc.height= SCREEN_HEIGHT;
}


void drawPixel(int x,int y,int color){ //RGB
    char * curpos = (char *)((uint64_t)screenData->framebuffer); 
    int offset = 3 * (x + y * screenData->width);
    curpos[offset] = (char)(color & 0xFF); //RED    
    curpos[offset + 1] = (char)((color >> 8) & 0xFF); //GREEN
    curpos[offset + 2] = (char)((color >> 16) & 0xFF); //BLUE        
}

void printChar(char c, uint64_t f_color, uint64_t bg_color){
	printCharOnScreen(c,f_color,bg_color,sc.current_x,sc.current_y);
}
void printCharOnScreen(char c, uint64_t f_color, uint64_t bg_color,uint32_t posX,uint32_t posY){
	int isCurrentPos =( (posX == sc.current_x) && (posY == sc.current_y)) ? 1:0;
	uint32_t x = posX;
	if(( (screenData->width -(x + sc.offset)) < CHAR_WIDTH )){
        x = 0;
        posY += CHAR_HEIGHT;
		if (screenData->height - posY < CHAR_HEIGHT) {
              posY -= CHAR_HEIGHT;
              scrollScreen();
			  clearSpace(x,posY,SCREEN_WIDTH,posY+CHAR_HEIGHT, 0x000000);
     	}
			
	}
	
	uint32_t y = posY;
	x += sc.offset;
	
	unsigned char * char_map = charMap(c);

	for(int i=0 ; i<CHAR_HEIGHT ; i++){
		for(int j=0 ; j<CHAR_WIDTH ; j++){
			int8_t isMarked = (char_map[i] >> (CHAR_WIDTH - j - 1)) & 0x01;
			if(isMarked){
				drawPixel(x,y,f_color);
			}else{
				drawPixel(x,y,bg_color); 	
			}
			x++;
		}
		x = posX + sc.offset;
		y++;
	}
	posX += CHAR_WIDTH;

	if(isCurrentPos){
		sc.current_x = posX;
		sc.current_y = posY;
	}
	
}

void deleteChar(uint64_t bg_color){
	if(sc.current_x == 0 && sc.current_y == 0){
		return;
	}
	if(sc.current_x == 0){
		sc.current_x = screenData->width - sc.offset;	
		
		sc.current_y -= CHAR_HEIGHT;
	}
	sc.current_x -= CHAR_WIDTH;
	printChar(' ',bg_color,bg_color);
	sc.current_x -= CHAR_WIDTH;
}


void newLine(){
	sc.current_y += CHAR_HEIGHT;
	sc.current_x =0;
	if (screenData->height - sc.current_y < CHAR_HEIGHT) {
        sc.current_y -= CHAR_HEIGHT;
        scrollScreen();
		clearSpace(sc.current_x, sc.current_y,SCREEN_WIDTH,sc.current_y+CHAR_HEIGHT, 0x000000);
    }
}

void clearScreen(uint64_t bg_color){
	sc.current_x = 0;
	sc.current_y = 0;
	for(int i=0 ; i<=screenData->width ;i+=CHAR_WIDTH){
		for(int j=0 ; j<=screenData->height ; j+=CHAR_HEIGHT){
			printCharOnScreen(' ',bg_color,bg_color,i,j);
		}
	}
	sc.current_x = 0;
	sc.current_y = 0;
}

void clearSpace(uint32_t startx, uint32_t starty, uint32_t endx, uint32_t endy, uint64_t bg_color){
	uint32_t aux_currentx=sc.current_x;
	uint32_t aux_currenty=sc.current_y;
	sc.current_x = startx;
	sc.current_y = starty;
	for(int i=starty ; i<=endy ;i++){            
		for(int j=startx ; j<=endx ; j++){
			drawPixel(j,i,bg_color);
		}
	}
	sc.current_x = aux_currentx;
	sc.current_y = aux_currenty;
}

void scrollScreen(){
    memcpy((void *)((uint64_t)screenData->framebuffer), (void *) ((uint64_t)screenData->framebuffer+CHAR_HEIGHT*PIXEL_SIZE*SCREEN_WIDTH),SCREEN_WIDTH*3*(SCREEN_HEIGHT-CHAR_HEIGHT));
}

	