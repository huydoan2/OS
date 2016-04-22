#include "PIT.h"
#include "i8259.h"
#define DEFAULT_FREQ	36000		//40ms Period
#define RTC_IRQ_0 		0

void pit_init()
{	
	outb(INIT_VALUE, CMD);
	pit_set_freq(DEFAULT_FREQ);
	enable_irq(RTC_IRQ_0);
}

void pit_set_freq(uint32_t freq)
{
	uint16_t frequency = CONSTANT/freq; 
	outb(frequency & 0xff, CHANNEL_0);
	outb((frequency >> 8) & 0xff, CHANNEL_0);
}
