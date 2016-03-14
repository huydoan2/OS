/* keyboard.c
 * - scancode lookup array
 * - key stroke output functions
 * - keyboard initialization functions
 * vim:ts=4 noexpandtab
 */

#include "keyboard.h"

#define keyboard_irq_num 1
#define left_shift_on 0x2A
#define left_shift_off 0xAA
#define right_shift_on 0x36
#define right_shift_off 0xB6
#define caps_lock_on 0x3A
#define caps_lock_off 0xBA
//scancode array for keyboard
//0 = no output
//size of the scancode is 88
char scancode [128] = {
	'\0','1','2','3','4','5','6','7','8','9','0','-','=','\b','\t',//backspace and tab
	'q','w','e','r','t','y','u','i','o','p','[',']','\n',0,//Control
	'a','s','d','f','g','h','j','k','l',';','\'','`',0,//Left shift
  '\\','z','x','c','v','b','n','m',',','.','/',0,
  '*',0,' ',0,0,0,0,0,0,0,0,0,0,0,0,0,
  '7','8','9','-','4','5','6','+','1','2','3','0','.',
 	0,0,0,0,0,0, /* All Release keys are undefined */
};


char shift_scancode[128] =
{
	'\0','!','@','#','$','%','^','&','*','(',')','_','+','\b','\t', //backspace and tab
 	'Q','W','E','R','T','Y','U','I','O','P','{','}','\n',0,//Control
  'A','S','D','F','G','H','J','K','L',':','"','~',0,//Left shift
  '|','Z','X','C','V','B','N','M','<','>','?',0,//Right shift
  '*',0,' ',0/* Caps lock */,0,0,0,0,0,0,0,0,0,0,0/* 69 - Num lock*/,0/* Scroll Lock */,
  0/* Home key */,0/* Up Arrow */,0/* Page Up */,'-',0/* Left Arrow */,0,0/* Right Arrow */,
  '+',0/* 79 - End key*/,0/* Down Arrow */,0/* Page Down */,0/* Insert Key */,0/* Delete Key */,
  0,0,0,0/* F11 Key */,0/* F12 Key */,0,  /* All Release keys are undefined */ 
};

char caps_scancode[128] =
{
	'\0','1','2','3','4','5','6','7','8','9','0','-','=','\b','\t',//backspace and tab
	'Q','W','E','R','T','Y','U','I','O','P','[',']','\n',0,//Control
  'A','S','D','F','G','H','J','K','L',';','\'','`',0,//Left shift
  '\\','Z','X','C','V','B','N','M',',','.','/',0,
  '*',0,' ',0,0,0,0,0,0,0,0,0,0,0,0,0,
  '7','8','9','-','4','5','6','+','1','2','3','0','.',
 	0,0,0,0,0,0, /* All Release keys are undefined */
};

char caps_shift_scancode[128] =
{
	'\0','!','@','#','$','%','^','&','*','(',')','_','+','\b','\t', //backspace and tab
 	'q','w','e','r','t','y','u','i','o','p','{','}','\n',0,//Control
  'a','s','d','f','g','h','j','k','l',':','"','~',0,//Left shift
  '|','z','x','c','v','b','n','m','<','>','?',0,//Right shift
  '*',0,' ',0/* Caps lock */,0,0,0,0,0,0,0,0,0,0,0/* 69 - Num lock*/,0/* Scroll Lock */,
  0/* Home key */,0/* Up Arrow */,0/* Page Up */,'-',0/* Left Arrow */,0,0/* Right Arrow */,
  '+',0/* 79 - End key*/,0/* Down Arrow */,0/* Page Down */,0/* Insert Key */,0/* Delete Key */,
  0,0,0,0/* F11 Key */,0/* F12 Key */,0,  /* All Release keys are undefined */ 
};

int shift_flag = 0;
int caps_lock_flag = 0;
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
	return c;
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
	unsigned char c = getScancode();
	if(c == left_shift_on || c == right_shift_on)
		shift_flag = 1;
	else if(c == left_shift_off || c == right_shift_off)
		shift_flag = 0;
	
	if(c == caps_lock_on)
		caps_lock_flag++;
	else if(c == caps_lock_off)
		caps_lock_flag = caps_lock_flag;
	
	if(caps_lock_flag == 8)
		caps_lock_flag = 0;	

	if((caps_lock_flag%2) ==0)
	{
		if(shift_flag == 1)
		{
	    	if(c < 0x81)
	      		return shift_scancode[c-1];
	    	else
	      		return shift_scancode[0];
		}
		else
		{
			if(c < 0x81)
	      		return scancode[c-1];
	    	else
	      		return scancode[0];
		}
	}
	else
	{
		if(shift_flag == 1)
		{
	    	if(c < 0x81)
	      		return caps_shift_scancode[c-1];
	    	else
	      		return caps_shift_scancode[0];
		}
		else
		{
			if(c < 0x81)
	      		return caps_scancode[c-1];
	    	else
	      		return caps_scancode[0];
		}
	}
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
	shift_flag = 0;
	caps_lock_flag = 0;
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
	if(getchar() != '\0')
		printf("%c", getchar());
	send_eoi(keyboard_irq_num);
	asm volatile("                  \n\
		    leave                    \n\
			iret                    \n\
		    "
			);
}
