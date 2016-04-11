    /* keyboard.c
 * - scancode lookup array
 * - key stroke output functions
 * - keyboard initialization functions
 * vim:ts=4 noexpandtab
 */

#include "keyboard.h"
#include "systemCalls.h"
#define keyboard_irq_num 1
#define left_shift_on 0x2A
#define left_shift_off 0xAA
#define right_shift_on 0x36
#define right_shift_off 0xB6
#define left_control_on 0x1D
#define left_control_off 0x9D
#define right_control_on 0x1D
#define right_control_off 0x9D
#define caps_lock_on 0x3A
#define caps_lock_off 0xBA
#define end_of_press 0x81
#define size_of_keys 128
#define max_keys 126
#define max_flag	8
#define L_pressed 0x26
#define C_pressed 0x2E



//scancode array for keyboard
//0 = no output
//size of the scancode is 88
char garbage [size_of_keys] = 
{
	'\0','1','2','3','4','5','6','7','8','9','0','-','=','\b',0,//backspace and tab
	'q','w','e','r','t','y','u','i','o','p','[',']','\n',0,//Control
	'a','s','d','f','g','h','j','k','l',';','\'','`',0,//Left shift
  	'\\','z','x','c','v','b','n','m',',','.','/',0,
  	'*',0,' ',0,0,0,0,0,0,0,0,0,0,0,0,0,
  	'7',0,'9','-',0,'5',0,'+','1',0,'3','0','.',0,0,0,0,0,0, /* All Release keys are undefined */
};
char scancode [size_of_keys] = 
{
	'\0','1','2','3','4','5','6','7','8','9','0','-','=','\b',0,//backspace and tab
	'q','w','e','r','t','y','u','i','o','p','[',']','\n',0,//Control
	'a','s','d','f','g','h','j','k','l',';','\'','`',0,//Left shift
  	'\\','z','x','c','v','b','n','m',',','.','/',0,
  	'*',0,' ',0,0,0,0,0,0,0,0,0,0,0,0,0,
  	'7',0,'9','-',0,'5',0,'+','1',0,'3','0','.',0,0,0,0,0,0, /* All Release keys are undefined */
};

char shift_scancode[size_of_keys] =
{
	'\0','!','@','#','$','%','^','&','*','(',')','_','+','\b',0, //backspace and tab
 	'Q','W','E','R','T','Y','U','I','O','P','{','}','\n',0,//Control
  	'A','S','D','F','G','H','J','K','L',':','"','~',0,//Left shift
  	'|','Z','X','C','V','B','N','M','<','>','?',0,//Right shift
  	'*',0,' ',0,0,0,0,0,0,0,0,0,0,0,0,0,
  	'7',0,'9','-',0,'5',0,'+','1',0,'3','0','.',0,0,0,0,0,0, /* All Release keys are undefined */
};

char caps_scancode[size_of_keys] =
{
	'\0','1','2','3','4','5','6','7','8','9','0','-','=','\b',0,//backspace and tab
	'Q','W','E','R','T','Y','U','I','O','P','[',']','\n',0,//Control
 	'A','S','D','F','G','H','J','K','L',';','\'','`',0,//Left shift
  	'\\','Z','X','C','V','B','N','M',',','.','/',0,
  	'*',0,' ',0,0,0,0,0,0,0,0,0,0,0,0,0,
  	'7',0,'9','-',0,'5',0,'+','1',0,'3','0','.',0,0,0,0,0,0, /* All Release keys are undefined */
};

char caps_shift_scancode[size_of_keys] =
{
	'\0','!','@','#','$','%','^','&','*','(',')','_','+','\b',0, //backspace and tab
 	'q','w','e','r','t','y','u','i','o','p','{','}','\n',0,//Control
 	'a','s','d','f','g','h','j','k','l',':','"','~',0,//Left shift
 	'|','z','x','c','v','b','n','m','<','>','?',0,//Right shift
 	'*',0,' ',0,0,0,0,0,0,0,0,0,0,0,0,0,
 	'7',0,'9','-',0,'5',0,'+','1',0,'3','0','.',0,0,0,0,0,0, /* All Release keys are undefined */
};

int shift_flag;				/*flag for shift*/
int caps_lock_flag;			/*flag for caps*/
int control_flag;			/*flag for control*/
volatile int enter_flag;	/*flag for enter*/
int lb_index;				/*line buffer index*/
char line_buffer [size_of_keys] = {0};		/*initialize line buffer*/

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

	/*if the left shift or right shift is pressed, set the shift flag to 1*/
	if(c == left_shift_on || c == right_shift_on)
		shift_flag = 1;
	/*if the left shift or right shift is released, set the shift flag to 0*/
	else if(c == left_shift_off || c == right_shift_off)
		shift_flag = 0;
	
	/*if the left control or right control is pressed, set the control flag to 1*/
	if(c == left_control_on || c == right_control_on)
		control_flag = 1;
	/*if the left control or right control is released, set the control flag to 0*/
	else if(c == left_control_off || c == right_control_off)
		control_flag = 0;
	

	/*if the caps lock is pressed, change the capslock flag*/
	if(c == caps_lock_on)
		caps_lock_flag++;
	else if(c == caps_lock_off)
		caps_lock_flag = caps_lock_flag;

	/*reset the caps lock flag once it goes over the max*/
	if(caps_lock_flag == max_flag)
		caps_lock_flag = 0;	

	/*if control+L, clear the display */
	if(control_flag && c == L_pressed)
	{
		clear();
		reset_linebuffer();
		return 0;
	}

	if(control_flag && c == C_pressed)
	{
		if(current_pid!=1)
		{
			display_c('\n');
			syscall_halt(0);
			reset_linebuffer();
			return 0;
		}
	}

	/*caps lock off case, checking if it's even or odd*/
	if((caps_lock_flag % 2) ==0)
	{
		/*caps lock off and shift on*/
		if(shift_flag == 0)
		{
	    	if(c < end_of_press){
	      		return scancode[c-1];
	      	}
	    	else
	      		return scancode[87];
		}
		/*caps lock off and shift off*/
		else
		{
			if(c < end_of_press)
	      		return shift_scancode[c-1];
	    	else
	      		return shift_scancode[87];
		}
	}

	/*caps lock on case*/
	else
	{
		/*caps lock on and shift on*/
		if(shift_flag == 1)
		{
	    	if(c < end_of_press)
	      		return caps_shift_scancode[c-1];
	    	else
	      		return caps_shift_scancode[87];
		}
		/*caps lock on and shift off*/
		else
		{
			if(c < end_of_press)
	      		return caps_scancode[c-1];
	    	else
	      		return caps_scancode[87];
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
	send_eoi(keyboard_irq_num);
	char c = getchar();
	//handle next line input
	if(c == '\n'|| c == '\r')
	{
	    lb_index++;
		line_buffer[lb_index] = c;
		enter_flag = 1;
		newline();
	}
	//handle the backspace input
	else if(c == '\b')
	{
		if(lb_index >= 0)
		{
			lb_index--;
			delete(); //delete the character 
		}
	}
	//limit the maximum number of input characters
	else if(c != '\0' && lb_index < max_keys)			/*if the scancode value is not empty, print out the character*/
	{
		display_c(c);
	    lb_index++;
		line_buffer[lb_index] = c;
	}
	asm volatile("                  \n\
		    leave                    \n\
			iret                    \n\
		    "
			);
}

/* 
 * reset_linebuffer
 *   DESCRIPTION: reset the line buffer and buffer index
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: 
 *
 */
void reset_linebuffer()
{	
	int i;
	for (i = 0; i < lb_index; ++i)
	{
		line_buffer[i] = 0;
	}
	lb_index = -1;
}

/* 
 * keyboard_open
 *   DESCRIPTION: open keyboard driver and initialize keyboard flags and keyboard
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: all the flags and PIC are initialized 
 *
 */
int32_t keyboard_open()
{
	shift_flag = 0;					/*initialize the shift flag to zero*/
	caps_lock_flag = 0;				/*initialize the caps lock flag to zero*/
	control_flag = 0;				/*initialize flag for control*/
	enter_flag = 0;					/*initialize enter to zero*/
	lb_index = -1;					/*initialize line buffer index*/
	keyboard_init();
	return 0;
}

/* 
 * keyboard_close
 *   DESCRIPTION: close keyboard driver
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: does nothing
 *
 */
int32_t keyboard_close()
{
	return 0;
}


/* 
 * keyboard_read
 *   DESCRIPTION: read keyboard
 *-----------------------------------------------------------------------------------
 *   INPUTS: char * buff = buffer to copy chars from the line buffer 
 			int num_bytes = number of bytes
 *   OUTPUTS: none
 *   RETURN VALUE: none

 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: 
 *			- the characters from the line buffer is stored in the input buffer
 *			- the line buffer and its index variable are cleared 
 *
 */



int32_t keyboard_read(int32_t * buff, uint32_t offset, int32_t num_bytes)
{
	
	int i = 0;
	char* read_buff = (char*)buff;
	//check if the input is valid
	if(read_buff  == NULL)
		return -1;
    reset_linebuffer();
	//wait for the user to finish typing (hit enter)
	while(enter_flag == 0);
 	enter_flag = 0;

 	cli();
 	//copy the characters in the line buffer
 	while(line_buffer[i] != '\n' && i < num_bytes)
 	//while(i < num_bytes)
 	{
 		read_buff [i] = line_buffer[i];
 		i++;
 	}
 	read_buff [i] = '\n';
 	i++;
 	sti();
 	buff = (int32_t*)read_buff;
 	reset_linebuffer();	//reset line buffer by reset the index
 	return i;
}

/* 
 * keyboard_write
 *   DESCRIPTION: write keyboard, display the content from the buffer to the screen
 *-----------------------------------------------------------------------------------
 *   INPUTS: char * buff = buffer that has characters to be written
 			int num_bytes = number of bytes to write
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: 
 *			- characters passed in by the buffer are displayed on the screen
 *
 */
int32_t keyboard_write(int32_t * buff, int32_t num_bytes)
{
	int i =0;
	char*write_buff = (char*)buff;
	//Check if the inputs are valid
	if(buff == NULL || num_bytes < 0)
		return -1;
	cli();
	//write character to the screen]
	while(i < num_bytes)
	{
		putc(write_buff[i]);
		i++;
	}
	sti();
	return i;
}


