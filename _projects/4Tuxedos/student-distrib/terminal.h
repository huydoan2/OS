#ifndef TERMINAL_H
#define TERMINAL_H

#define MAX_ROW 80
#define MAX_COLUMN 25
#define VIDEO_MEM_START 0xB000

typedef struct text{
	uint8_t back_color : 4;
	uint8_t fore_color : 4;
	char	character;
} text_t __attribute((packed));

int32_t cursor;











#endif

