/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

#define mask 0xff
#define EIGHT 8
#define SEVEN 7
#define TWO 2
/* Interrupt masks to determine which interrupts
 * are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7 */
uint8_t slave_mask; /* IRQs 8-15 */

/* Initialize the 8259 PIC */
void
i8259_init(void)
{
	/*save the mask values from the master and slave PICs*/
	master_mask = mask;            
	slave_mask = mask;	
	
	/*start the initialization sequence*/
	outb(ICW1, MASTER_COMMAND);			

	outb(ICW1, SLAVE_COMMAND);

	
	/*set the master and slave's vector offset*/
	outb(ICW2_MASTER, MASTER_DATA);           

	outb(ICW2_SLAVE, SLAVE_DATA);                 

	
	/*connect the master PIC with the slave PIC*/
	outb(ICW3_MASTER, MASTER_DATA);               //  a slave PIC at IRQ2 of the master PIC

	outb(ICW3_SLAVE, SLAVE_DATA);                 // inform PIC its cascade identity (0000 0010)

	
	outb(ICW4, MASTER_DATA);

	outb(ICW4, SLAVE_DATA);

	
 	/*resotre the PIC masks*/ 	
	outb(master_mask, MASTER_DATA);   			
	outb(slave_mask, SLAVE_DATA);
}

/* Enable (unmask) the specified IRQ */
void
enable_irq(uint32_t irq_num)
{
	uint8_t value;
	//if the interrupt request line's on the master PIC
    if(irq_num < EIGHT)
    {
    	value = inb(MASTER_DATA) & (~(1 << irq_num));
    	outb(value, MASTER_DATA);
    } 
    else //if the interrupt request line's on the slave PIC
    {
    	irq_num &= SEVEN;
    	value = inb(SLAVE_DATA) & (~(1 << irq_num));
        outb(value, SLAVE_DATA);
    }
}

/* Disable (mask) the specified IRQ */
void
disable_irq(uint32_t irq_num)
{
	uint8_t value;
	if(irq_num < EIGHT) //if the interrupt request line's on the master PIC
    {
    	value = inb(MASTER_DATA) | (1 << irq_num);
    	outb(value, MASTER_DATA);
    } 
    else //if the interrupt request line's on the slave PIC
    {
    	irq_num &= SEVEN;
    	value = inb(SLAVE_DATA) | (1 << irq_num);
        outb(value, SLAVE_DATA);
    }
}

/* Send end-of-interrupt signal for the specified IRQ */
void
send_eoi(uint32_t irq_num)
{
	//if the interrupt request line's on the slave PIC
	if(irq_num >= EIGHT){
		outb(EOI | TWO, MASTER_COMMAND);
		outb(EOI | (irq_num & SEVEN), SLAVE_COMMAND);
	}

	//if the interrupt request line's on the master PIC
	else
		outb(EOI | irq_num, MASTER_COMMAND);

}

