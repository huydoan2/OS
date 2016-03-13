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
#define RATE        6
#define POST_MASK   0x7F
#define PREV_MASK   0xF0


void rtc_init(){
	char previous;
	/*initialize the RTC hardware*/

	//set the register A

	outb(REGISTER_A|NMI_DISABLE, RTC_PORT);		//disable NMI and select reg A
	previous = inb(CMOS_PORT);					//read current value of reg A
	outb((previous & PREV_MASK) | MASK_TURNON_FIVEBIT | RATE, CMOS_PORT);
	
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

	int32_t i;
	
	send_eoi(RTC_IRQ_2);
	send_eoi(RTC_IRQ_8);

	for (i=0; i < NUM_ROWS*NUM_COLS; i++) {
		video_mem[i<<1]++;
	}
	
	outb(REGISTER_C , RTC_PORT);	// select register C
	inb(CMOS_PORT);		// just throw away contents
	

	asm volatile("                  \n\
		    leave                    \n\
			iret                    \n\
		    "
			);
}
