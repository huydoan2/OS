#include "keyboard.h"

#define keyboard_irq_num 1

char scancode [10] = {
	'1','2','3','4','5','6','7','8','9','0'
};

char getScancode()
{
	char c = inb(KEYBOARD_DATA);
	if (c > 0)
		return c;
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

	printf("Value:%c ", getchar());
	send_eoi(keyboard_irq_num);
}
