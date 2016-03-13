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
#define RTC_IRQ_NUM 8
#define MASK_TURNON_SIXBIT  0x40
#define MASK_TURNON_FIVEBIT 0x20
#define NMI_DISABLE 0x80
#define RATE        6


void rtc_init(){
	char previous;
	/*initialize the RTC hardware*/

	//set the register A

	outb(REGISTER_A|NMI_DISABLE, RTC_PORT);		//disable NMI and select reg A
	previous = inb(CMOS_PORT);					//read current value of reg A
	outb((previous & 0xF0) | MASK_TURNON_FIVEBIT | RATE, CMOS_PORT);
	
	//set the register B

	outb(REGISTER_B|NMI_DISABLE, RTC_PORT);
	//read the value from register B	
    previous = inb(CMOS_PORT);	
    //set the index value
    outb(REGISTER_B|NMI_DISABLE, RTC_PORT);		
    //turn on the bit 6 of register B
    outb(previous | MASK_TURNON_SIXBIT, CMOS_PORT);	

	/*initialize the PIC*/
	enable_irq(RTC_IRQ_NUM);
	enable_irq(2);

	/*enable the NMI*/
	outb(inb(0x70)&0x7F,0x70);
}
