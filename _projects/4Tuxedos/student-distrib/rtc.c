/*rtc.c 
 * - function for initializing RTC and its PIC connection
 */
#include "rtc.h"

#define RTC_PORT	0x70
#define CMOS_PORT   0x71
#define REGISTER_A  0x0A
#define REGISTER_B  0x0B
#define REGISTER_C  0x0C
#define REGISTER_D  0x0D
#define RTC_IRQ_8   8
#define RTC_IRQ_2   2
#define MASK_TURNON_SIXBIT  0x40
#define MASK_TURNON_FIVEBIT 0x20
#define NMI_DISABLE 0x80
#define POST_MASK   0x7F
#define PREV_MASK   0xF0
#define VIDEO 0xB8000
#define NUM_COLS 80
#define NUM_ROWS 25
#define time_offset_100 100
#define time_offset_10 10
#define column_offset_1 1
#define column_offset_2 2
#define column_offset_3 3
#define rtc_num_byte	4
#define rtc_freq 32768

#define rate_0 15
#define rate_1 14
#define rate_2 13
#define rate_3 12
#define rate_4 11
#define rate_5 10
#define rate_6 9
#define rate_7 8
#define rate_8 7
#define rate_9 6

#define freq_0 2
#define freq_1 4
#define freq_2 8
#define freq_3 16
#define freq_4 32
#define freq_5 64
#define freq_6 128
#define freq_7 256
#define freq_8 512
#define freq_9 1024

//static char* video_mem = (char *)VIDEO;
void rtc_set_rate();

int counter = 0;	//count number of half seconds
int clk = 0;		//timer to be printed at the right top corner
int rate = 15;		//rate of rtc, initialize to 15 which is slowest rate

extern uint32_t scheduling_terminal;
extern uint32_t current_terminal;
volatile int interrupt_flag[3];	//flag for interrupt

void rtc_init(){
	char previous;
	/*initialize the RTC hardware*/

	//set the register A

	outb(REGISTER_A|NMI_DISABLE, RTC_PORT);		//disable NMI and select reg A
	previous = inb(CMOS_PORT);					//read current value of reg A
	outb(REGISTER_A|NMI_DISABLE, RTC_PORT);		//disable NMI and select reg A
	outb((previous & PREV_MASK) | MASK_TURNON_FIVEBIT |rate, CMOS_PORT);

	
	//set the register B

	outb(REGISTER_B|NMI_DISABLE, RTC_PORT);
	//read the value from register B	
    previous = inb(CMOS_PORT);	
    //set the index value
    outb(REGISTER_B|NMI_DISABLE, RTC_PORT);		
    //turn on the bit 6 of register B
    outb(previous | MASK_TURNON_SIXBIT, CMOS_PORT);	

	/*initialize the PIC*/
	enable_irq(RTC_IRQ_8);
	enable_irq(RTC_IRQ_2);

	/*enable the NMI*/
	outb(inb(RTC_PORT)&POST_MASK,RTC_PORT);
}


/*
* void rtc_handler(void)
*   Inputs: void
*   Return Value: void
*	Function: increments video memory. To be used to test rtc
*/
void
rtc_handler(void)
{

    interrupt_flag[scheduling_terminal] = 1;
	send_eoi(RTC_IRQ_8);
	outb(REGISTER_C , RTC_PORT);	// select register C
	inb(CMOS_PORT);		            // just throw away contents
	

}
/* 
 * rtc_set_rate
 *   DESCRIPTION: sets the rtc rate to current calculated rate
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: 
 *
 */
void rtc_set_rate()
{
	char previous;
	//cli();
	//set the register A
	outb(REGISTER_A|NMI_DISABLE, RTC_PORT);		//disable NMI and select reg A
	previous = inb(CMOS_PORT);					//read current value of reg A
	outb(REGISTER_A|NMI_DISABLE, RTC_PORT);		//disable NMI and select reg A
	outb((previous & PREV_MASK) | rate, CMOS_PORT);
	//sti();
}

/* 
 * rtc_open
 *   DESCRIPTION: open rtc driver, initialize interrupt flag
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: 
 *
 */
int32_t rtc_open()
{
	interrupt_flag[0] = 0;
    interrupt_flag[1] = 0;
    interrupt_flag[2] = 0;
	return 0;
}

/* 
 * rtc_close
 *   DESCRIPTION: disable RTC IRQ and close rtc driver
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: 
 *
 */
int32_t rtc_close()
{
	return 0;
}

/* 
 * rtc_read
 *   DESCRIPTION: disable RTC IRQ and close rtc driver
 *-----------------------------------------------------------------------------------
 *   INPUTS: char * buff = pointer to Hz value 
 			int num_bytes = size of integer (should always be 4)
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: 
 *
 */
int32_t rtc_read(int32_t * buff, uint32_t offset,int32_t num_bytes, int32_t var)
{	
	while(!interrupt_flag[current_terminal]);
	interrupt_flag[current_terminal] = 0;
	return 0;
}

/* 
 * rtc_close
 *   DESCRIPTION: disable RTC IRQ and close rtc driver
 *-----------------------------------------------------------------------------------
 *   INPUTS: char * buff = pointer to Hz value 
 			int num_bytes = size of integer (should always be 4)
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: 
 *
 */
int32_t rtc_write(int32_t * buff, int32_t num_bytes)
{
	if((num_bytes != rtc_num_byte) || buff == NULL)
		return -1;

	switch(*buff)
	{
		case freq_0:
			rate = rate_0;
			break;
		case freq_1:
			rate = rate_1;
			break;
		case freq_2:
			rate = rate_2;
			break;
		case freq_3:
			rate = rate_3;
			break;
		case freq_4:
			rate = rate_4;
			break;
		case freq_5:
			rate = rate_5;
			break;
		case freq_6:
			rate = rate_6;
			break;
		case freq_7:
			rate = rate_7;
			break;
		case freq_8:
			rate = rate_8;
			break;
		case freq_9:
			rate = rate_9;
			break;
		default:
			rate = rate;
			return -1;
	}
	rtc_set_rate();
	return 0;
}

