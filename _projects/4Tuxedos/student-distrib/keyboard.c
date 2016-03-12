#include "keyboard.h"

#define keyboard_irq_num 1

//scancode array for keyboard
//0 = no output
char scancode [88] = {
	'1','2','3','4','5','6','7','8','9','0','-','=', 0 , 0,
	'q','w','e','r','t','y','u','i','o','p','[',']','\n',0,
	'a','s','d','f','g','h','j','k','l',';','\'','`',0,'\\',
	'z','x','c','v','b','n','m',',','.','/',0,'*',0,' ',0,
	0,0,0,0,0,0,0,0,0,0,0,0,'7','8','9','-','4','5','6','+','1','2','3','0','.',
	0,0,0,0,0,0
};

/* 
 * getScancode
 *   DESCRIPTION: get a keyboard input from keyboard_data address and return the data
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: if invalid input, return 0. Else return keyboard input
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: if the input data is less than 0, return 0
 *
 */
char getScancode()
{
	char c = inb(KEYBOARD_DATA);
	if (c > 0)
		return c;
	else
		return 0;
}

/* 
 * getchar
 *   DESCRIPTION: access the getScancode return value to return correct char value
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: scancode element
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: 
 *
 */
char getchar()
{
	return scancode[getScancode()-2];
}

/* 
 * keyboard_init
 *   DESCRIPTION: enables the keyboard irq (1)
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: allows keyboard press 
 *
 */
void keyboard_init()
{
	enable_irq(keyboard_irq_num);
}        

/* 
 * keyboard_handler
 *   DESCRIPTION: handles keyboard input and print the char. Then return control to kernel
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: end the keyboard interrupt and send control back to kernel 
 *
 */
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
