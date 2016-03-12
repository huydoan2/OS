#include "keyboard.h"

#define keyboard_irq_num 1

char scancode [88] = {
	'1','2','3','4','5','6','7','8','9','0','-','=', 0 , 0,
	'q','w','e','r','t','y','u','i','o','p','[',']','\n',0,
	'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\',
	'z','x','c','v','b','n','m',',','.','/',0,'*',0,' ',0,
	0,0,0,0,0,0,0,0,0,0,0,0,'7','8','9','-','4','5','6','+','1','2','3','0','.',
	0,0,0,0,0,0
};

char getScancode()
{
	char c = inb(KEYBOARD_DATA);
	if (c > 0)
		return c;
	else
		return 0;
}

char getchar()
{
	return scancode[getScancode()-2];
}

void keyboard_init()
{
	enable_irq(keyboard_irq_num);
}        

void keyboard_handler()
{	
	printf(" %c ", getchar());
	send_eoi(keyboard_irq_num);
	asm volatile("                  \n\
		    leave                    \n\
			iret                    \n\
		    "
			);
}
