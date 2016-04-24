    /* keyboard.c
 * - scancode lookup array
 * - key stroke output functions
 * - keyboard initialization functions
 * vim:ts=4 noexpandtab
 */

#include "keyboard.h"
#include "systemCalls.h"
#include "Paging.h"
#include "PCB.h"
//Define values for Keyboard
#define KEYBOARD_DATA 0x60
#define KEYBOARD_COMMAND 0x64
#define size_of_keys 128
#define num_command_history 10
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
#define empty_char 87
#define max_keys 126
#define max_flag	8
#define L_pressed 0x26
#define C_pressed 0x2E
#define num_terminal 3
#define left_alt_on 0x38	
#define left_alt_off 0xB8	
#define right_alt_on 0x38	
#define right_alt_off 0xB8	
#define F1_pressed 0x3B
#define F2_pressed 0x3C
#define F3_pressed 0x3D
#define Up_pressed 0x48
#define Down_pressed 0x50
#define up 1
#define down 0 
/*current pid for each terminal*/
extern uint32_t current_pid[MAX_TERMINAL];
/*terminal that is being processed*/
extern uint32_t scheduling_terminal;
/*number of active process*/
extern uint32_t num_active_process;
/*check if there are maximum number of process running*/
int32_t check_for_max_process();
/*update commmand array*/
void update_command_array();
/*get commmand history*/
void get_command_history(int dir);

//scancode array for keyboard
//0 = no output
//size of the scancode is 88
char garbage [size_of_keys] = 
{
	'\0','1','2','3','4','5','6','7','8','9','0','-','=','\b',0,//backspace and tab
	'q','w','e','r','t','y','u','i','o','p','[',']','\n',0,//Control
	'a','s','d','f','g','h','j','k','l',';','\'','`',0,//Left shift
  	'\\','z','x','c','v','b','n','m',',','.','/',0,//Right shift
  	'*',0/*left Alt*/,' ',0, /*start Functionkeys*/0,0,0,0,0,0,0,0,0,0,
  	0,0,'7',0,'9','-',0,'5',0,'+','1',0,'3','0','.',0,0,0,0,0,0, /* All Release keys are undefined */
};
char scancode [size_of_keys] = 
{
	'\0','1','2','3','4','5','6','7','8','9','0','-','=','\b',0,//backspace and tab
	'q','w','e','r','t','y','u','i','o','p','[',']','\n',0,//Control
	'a','s','d','f','g','h','j','k','l',';','\'','`',0,//Left shift
  	'\\','z','x','c','v','b','n','m',',','.','/',0,//Right shift
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

int shift_flag[num_terminal];				/*flag for shift*/
int caps_lock_flag[num_terminal];			/*flag for caps*/
int control_flag[num_terminal];			/*flag for control*/
int alt_flag[num_terminal];				/*flag for alt*/
int lb_index[num_terminal];				/*line buffer index*/
char line_buffer[num_terminal][size_of_keys] = {{0}};		/*initialize line buffer*/
int current_terminal = 0;				/*index of current terminal*/
volatile int enter_flag[num_terminal];	/*flag for enter*/


char command_history[num_terminal][num_command_history][size_of_keys] = {{{0}}};
int command_iter[num_terminal] = {0};
int command_starting_idx[num_terminal] = {0};
int num_existing_command[num_terminal] = {0};
int num_up[num_terminal] = {0};
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
	uint32_t prev_terminal_id;
	uint32_t curr_pid;
 	uint32_t esp;
	uint32_t ebp;
	pcb_struct_t *current_pcb;
	/*if the left shift or right shift is pressed, set the shift flag to 1*/
	if(c == left_shift_on || c == right_shift_on)
		shift_flag[current_terminal] = 1;
	/*if the left shift or right shift is released, set the shift flag to 0*/
	else if(c == left_shift_off || c == right_shift_off)
		shift_flag[current_terminal] = 0;
	
	/*if the left control or right control is pressed, set the control flag to 1*/
	if(c == left_control_on || c == right_control_on)
		control_flag[current_terminal] = 1;
	/*if the left control or right control is released, set the control flag to 0*/
	else if(c == left_control_off || c == right_control_off)
		control_flag[current_terminal] = 0;
	
	/*if the left control or right control is pressed, set the control flag to 1*/
	if(c == left_alt_on || c == right_alt_on)
		alt_flag[current_terminal] = 1;
	/*if the left control or right control is released, set the control flag to 0*/
	else if(c == left_alt_off || c == right_alt_off)
		alt_flag[current_terminal] = 0;


	/*if the caps lock is pressed, change the capslock flag*/
	if(c == caps_lock_on)
		caps_lock_flag[current_terminal]++;
	else if(c == caps_lock_off)
		caps_lock_flag[current_terminal] = caps_lock_flag[current_terminal];

	/*reset the caps lock flag once it goes over the max*/
	if(caps_lock_flag[current_terminal] == max_flag)
		caps_lock_flag[current_terminal] = 0;	

	/*if control+L, clear the display */
	if(control_flag[current_terminal] && c == L_pressed)
	{
		clear();
		reset_linebuffer();
		printf("391OS> ");
		return 0;
	}

	if(control_flag[current_terminal] && c == C_pressed)
	{
		putc('\n');
		syscall_halt(1);
		reset_linebuffer();
		return 0;
	}
	/*Check for change of terminal*/
	if(alt_flag[current_terminal])
	{
		switch(c)
		{
			/* F1: change to the first terminal */
			case F1_pressed:
			{
				/*change terminal if not at first terminal already*/
				if(current_terminal!=0){
					/* Check for maximum number of processes */
		            if(check_for_max_process() && current_pid[0] == 0)
					{
						printf("\nreached maximum process number\n");
						printf("391OS> ");
						return 0;
					}
					/*update terminal IDs*/
					prev_terminal_id = current_terminal;
					current_terminal = 0;
					control_flag[current_terminal] = 0;
					/* change the video memory for the next terminal */
					set_vid_mem(prev_terminal_id, current_terminal);
    				process_switch_mem_map(scheduling_terminal); 
    				/* up date cursor location */
					cursor_update_terminal();
					/* if there is no process in the terminal*/
					if(current_pid[current_terminal] == 0)
					{
						/* save informaiton for the process running in the current terminal */
						curr_pid = current_pid[prev_terminal_id];
					    scheduling_terminal = 0;
					    asm volatile("mov %%esp, %0" :"=c"(esp));
					  	asm volatile("mov %%ebp, %0" :"=c"(ebp)); 
					    current_pcb = find_PCB(curr_pid);
					    current_pcb->esp = esp;
					    current_pcb->ebp = ebp;    
					    /* luanch a new shell in the next terminal*/
						syscall_execute((uint8_t*)"shell");
					}
				}
				break;
			}
			case F2_pressed:
			{
				/*change terminal if not at first terminal already*/
				if(current_terminal!=1)
				{   
					/* Check for maximum number of processes */
					if(check_for_max_process() &&  current_pid[1] == 0)
					{
						printf("\nreached maximum process number\n");
						printf("391OS> ");
						return 0;
					}
					/*update terminal IDs*/
					prev_terminal_id = current_terminal;
					current_terminal = 1;
					control_flag[current_terminal] = 0;
					/* change the video memory for the next terminal */
					set_vid_mem(prev_terminal_id, current_terminal);
   					process_switch_mem_map(scheduling_terminal);
   					/* up date cursor location */ 
					cursor_update_terminal();
					/* if there is no process in the terminal*/
					if(current_pid[current_terminal] == 0)
					{
						/* save informaiton for the process running in the current terminal */
						curr_pid = current_pid[prev_terminal_id];
					    scheduling_terminal = 1;
					    asm volatile("mov %%esp, %0" :"=c"(esp));
					  	asm volatile("mov %%ebp, %0" :"=c"(ebp)); 
					    current_pcb = find_PCB(curr_pid);
					    current_pcb->esp = esp;
					    current_pcb->ebp = ebp;    
					    /* luanch a new shell in the next terminal*/
						syscall_execute((uint8_t*)"shell");
					}				
				}
				break;
			}
			case F3_pressed:
			{
				if(current_terminal!=2)
				{   
					/*change terminal if not at first terminal already*/
					if(check_for_max_process() &&  current_pid[2] == 0)
					{
						printf("\nreached maximum process number\n");
						printf("391OS> ");
						return 0;
					}
					/*update terminal IDs*/
					prev_terminal_id = current_terminal;
					current_terminal = 2;
					control_flag[current_terminal] = 0;
					/* change the video memory for the next terminal */
					set_vid_mem(prev_terminal_id, current_terminal);
    				process_switch_mem_map(scheduling_terminal); 
    				/* up date cursor location */ 
					cursor_update_terminal();
					/* if there is no process in the terminal*/
					if(current_pid[current_terminal] == 0)
					{
						/* save informaiton for the process running in the current terminal */
						curr_pid = current_pid[prev_terminal_id];
					    scheduling_terminal = 2;
					    asm volatile("mov %%esp, %0" :"=c"(esp));
					  	asm volatile("mov %%ebp, %0" :"=c"(ebp)); 
					    current_pcb = find_PCB(curr_pid);
					    current_pcb->esp = esp;
					    current_pcb->ebp = ebp;    
					     /* luanch a new shell in the next terminal*/
						syscall_execute((uint8_t*)"shell");
					}
				}
				break;
			}
		}
	}

	int i;
	/*if control+L, clear the display */
	if(c == Up_pressed)
	{
		for(i = 1; i < strlen(line_buffer[current_terminal]); ++i)
		{
			if(line_buffer[current_terminal][i] != '\n')
			{
				delete(); //delete the character 
			}
		}
		reset_linebuffer();
		get_command_history(up);
		for(i=0; i<strlen(line_buffer[current_terminal]); i++)
		{
			if(line_buffer[current_terminal][i] != '\n')
			{
				putc(line_buffer[current_terminal][i]);
			}
		}
		return 0;
	}

	if(c == Down_pressed)
	{
		for(i=1; i<strlen(line_buffer[current_terminal]); ++i)
		{
			delete(); //delete the character 
		}
		reset_linebuffer();
		get_command_history(down);
		for(i=0; i<strlen(line_buffer[current_terminal]); i++)
		{
			if(line_buffer[current_terminal][i] != '\n')
			{
				putc(line_buffer[current_terminal][i]);
			}
		}
		return 0;
	}

	/*caps lock off case, checking if it's even or odd*/
	if((caps_lock_flag[current_terminal] % 2) ==0)
	{
		/*caps lock off and shift on*/
		if(shift_flag[current_terminal] == 0)
		{
	    	if(c < end_of_press)
	      		return scancode[c-1];
	    	else
	      		return scancode[empty_char];
		}
		/*caps lock off and shift off*/
		else
		{
			if(c < end_of_press)
	      		return shift_scancode[c-1];
	    	else
	      		return shift_scancode[empty_char];
		}
	}

	/*caps lock on case*/
	else
	{
		/*caps lock on and shift on*/
		if(shift_flag[current_terminal] == 1)
		{
	    	if(c < end_of_press)
	      		return caps_shift_scancode[c-1];
	    	else
	      		return caps_shift_scancode[empty_char];
		}
		/*caps lock on and shift off*/
		else
		{
			if(c < end_of_press)
	      		return caps_scancode[c-1];
	    	else
	      		return caps_scancode[empty_char];
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
		update_command_array();
		if(lb_index[scheduling_terminal] <= max_keys)
	    {
	    	lb_index[scheduling_terminal]++;
			line_buffer[scheduling_terminal][lb_index[scheduling_terminal]] = c;
		}
		else
			reset_linebuffer();
		enter_flag[scheduling_terminal] = 1;
		newline();
	}
	//handle the backspace input
	else if(c == '\b')
	{
		if(lb_index[scheduling_terminal] >= 0)
		{
			lb_index[scheduling_terminal]--;
			delete(); //delete the character 
		}
	}
	//limit the maximum number of input characters
	else if(c != '\0' && lb_index[scheduling_terminal] < max_keys)			/*if the scancode value is not empty, print out the character*/
	{
		
		    lb_index[scheduling_terminal]++;
			line_buffer[scheduling_terminal][lb_index[scheduling_terminal]] = c;
			putc(c);	
	}
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
	for (i = 0; i < lb_index[scheduling_terminal]; ++i)
		line_buffer[scheduling_terminal][i] = 0;
	lb_index[scheduling_terminal] = -1;
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
	int i;
	for (i = 0; i < num_terminal; ++i)
	{
		shift_flag[i] = 0;					/*initialize the shift flag to zero*/
		caps_lock_flag[i] = 0;				/*initialize the caps lock flag to zero*/
		control_flag[i] = 0;				/*initialize flag for control*/
		enter_flag[i] = 0;					/*initialize enter to zero*/
		lb_index[i] = -1;					/*initialize line buffer index*/
	}
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
 * terminal_read
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



int32_t terminal_read(int32_t * buff, uint32_t offset, int32_t num_bytes, int32_t var)
{
	int i = 0;
	char* read_buff = (char*)buff;
	//check if the input is valid
	if(read_buff  == NULL)
		return -1;
	reset_linebuffer();
	//wait for the user to finish typing (hit enter)
	while(enter_flag[scheduling_terminal] == 0);
 	enter_flag[scheduling_terminal] = 0;
 	//copy the characters in the line buffer
 	while(line_buffer[scheduling_terminal][i] != '\n' && i < num_bytes)
 	{
 		read_buff [i] = line_buffer[scheduling_terminal][i];
 		i++;
 	}
 	read_buff [i] = '\n';
 	i++;
 	buff = (int32_t*)read_buff;
 	reset_linebuffer();	//reset line buffer by reset the index
 	return i;
}

/* 
 * terminal_write
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

int32_t terminal_write(int32_t * buff, int32_t num_bytes)
{ 
	cli();
	int i =0;
	char* write_buff = (char*)buff;
	//Check if the inputs are valid
	if(buff == NULL || num_bytes < 0)
		return -1;
	//write character to the screen]
	
	while(i < num_bytes)
	{
		putc(write_buff[i]);
		i++;
	}
	reset_linebuffer();	//reset line buffer by reset the index
	sti();
	return i;
}

/* 
 * check_for_max_process
 *   DESCRIPTION: check if there are maximum number of process running
 *-----------------------------------------------------------------------------------
 *   INPUTS: 
 *   OUTPUTS: 1 if max if running, 0 if not
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: 
 *
 *
 */
int32_t check_for_max_process()
{
	/*if num_active_process is 6, return 1*/
	if(num_active_process == 6)
		return 1;
	else
		return 0;
}


/* 
 * update_command_array
 *   DESCRIPTION: check if there are maximum number of process running
 *-----------------------------------------------------------------------------------
 *   INPUTS: 
 *   OUTPUTS: 1 if max if running, 0 if not
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: 
 *
 *
 */
void update_command_array()
{
	strcpy(command_history[current_terminal][command_starting_idx[current_terminal]], line_buffer[current_terminal]);
	command_iter[current_terminal]++ == 10 ? 0: command_iter[current_terminal];
	command_starting_idx[current_terminal]++ == 10 ? 0: command_starting_idx[current_terminal];
	num_existing_command[current_terminal]++ == 10 ? 10: num_existing_command[current_terminal];
	num_up[0] = 0;
	num_up[1] = 0;
	num_up[2] = 0;
}


void get_command_history(int dir)
{
	if(dir)
	{
		//case 1: up
		if(num_up[current_terminal] <= num_existing_command[current_terminal])
		{
			command_iter[current_terminal]-- == -1 ? 9: command_iter[current_terminal];
			num_up[current_terminal]++;
			strcpy(line_buffer[current_terminal], command_history[current_terminal][command_iter[current_terminal]]);
		}
	}
	else
	{
		//case 2: down
		if(num_up[current_terminal] > 0)
		{
			command_iter[current_terminal]++ == 10 ? 0: command_iter[current_terminal];
			num_up[current_terminal]--;
			strcpy(line_buffer[current_terminal], command_history[current_terminal][command_iter[current_terminal]]);
		}
	}
}

