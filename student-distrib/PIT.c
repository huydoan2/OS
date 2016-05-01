#include "PIT.h"
#include "i8259.h"
#define DEFAULT_FREQ	48000		//40ms Period
#define RTC_IRQ_0 		0
#define mask 0xff
#define eight 8
/* 
 * pit_init
 *   DESCRIPTION: function that initilizes the PIT
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS:  pit initialized and enable interrupt in the PIC
 *
 */
void pit_init()
{	
	outb(INIT_VALUE, CMD);
	pit_set_freq(DEFAULT_FREQ);
	enable_irq(RTC_IRQ_0);
}

/* 
 * pit_set_freq
 *   DESCRIPTION: set the PIT frequency with the given frequency 
 *-----------------------------------------------------------------------------------
 *   INPUTS: 
 *		- freq: the freqency to be set
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: 
 *
 */
void pit_set_freq(uint32_t freq)
{
	uint16_t frequency = CONSTANT/freq; 
	outb(frequency & mask, CHANNEL_0);
	outb((frequency >> eight) & mask, CHANNEL_0);
}
