#ifndef TERMINAL_H
#define TERMINAL_H

#define MAX_ROW 80
#define MAX_COLUMN 25
#define VIDEO_MEM_START 0xB8000 //starting address of the video memory buffer
#define ATTRIB_COLOR_DEFULT 0x0F00 //the default attribute color for attriute (back:black, fore:white)
#define BLANK 0x20|ATTRIB_COLOR_DEFULT


#include "lib.h"


/*--------global variables and structures-------*/
/*text structure for a single memory entry*/
typedef struct text{
	union{ 
		uint16_t val;
		struct{
			uint8_t back_color : 4;
			uint8_t fore_color : 4;
			char	character;
		} __attribute__((packed));
	};
} text_t;


text_t text_color;
 
/*variables to locate the cursor location on screen*/
int32_t cursor_x = 0;	//initialized to be the right upper corner 
int32_t cursor_y = 0;

/*--------functions for TUI in the terminal--------*/
/*function that scrolls the screen */
void scroll_screen(void);
/*function that update the position of teh cursor*/
void update_cursor(void);
/*function that clears the screen*/
//extern void clear_screen(void);
/*function that initializes the video memory*/
extern void init_videoMem();
/*functions that set the color of the text*/
extern void set_text_color(text_t text_attrib);










#endif

