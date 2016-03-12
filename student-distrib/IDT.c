#include "IDT.h"
#include "lib.h"

//Define values for IDT entry
#define RESERVED4 0x0000
#define DPL_0 0x0
#define DPL_1 0x1
#define DPL_2 0x2
#define DPL_3 0x3
#define SIZE_32 1
#define SIZE_16 0
#define PRESENT 1
#define NOT_PRESENT 0


/* 
 * Exception_Handler_0
 *   DESCRIPTION: the exception handler that handles the Divide-by-zero Error
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */

void Exception_Handler_0(){

	//print out the error message
	printf("Exception: Divide-by-zero Error. \n");

	//stop the execution 
	while(1);
}

/* 
 * Exception_Handler_1
 *   DESCRIPTION: the exception handler that handles the Debug 
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */

void Exception_Handler_1(){

	//print out the error message
	printf("Exception: Debug. \n");

	//stop the execution 
	while(1);
}

/* 
 * Exception_Handler_2
 *   DESCRIPTION: the exception handler that handles the Non-maskable Interrupt
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */

void Exception_Handler_2(){

	//print out the error message
	printf("Exception: Non-maskable Interrupt. \n");

	//stop the execution 
	while(1);
}

/* 
 * Exception_Handler_3
 *   DESCRIPTION: the exception handler that handles the Breakpoint
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */

void Exception_Handler_3(){

	//print out the error message
	printf("Exception: Breakpoint. \n");

	//stop the execution 
	while(1);
}

/* 
 * Exception_Handler_4
 *   DESCRIPTION: the exception handler that handles the Overflow
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */

void Exception_Handler_4(){

	//print out the error message
	printf("Exception: Overflow. \n");

	//stop the execution 
	while(1);
}

/* 
 * Exception_Handler_5
 *   DESCRIPTION: the exception handler that handles the Bound Range Exceeded
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */

void Exception_Handler_5(){

	//print out the error message
	printf("Exception: Bound Range Exceeded. \n");

	//stop the execution 
	while(1);
}

/* 
 * Exception_Handler_6
 *   DESCRIPTION: the exception handler that handles the Invalid Opcode
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */

void Exception_Handler_6(){

	//print out the error message
	printf("Exception: Invalid Opcode. \n");

	//stop the execution 
	while(1);
}

/* 
 * Exception_Handler_7
 *   DESCRIPTION: the exception handler that handles the Device Not Available
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */

void Exception_Handler_7(){

	//print out the error message
	printf("Exception: Device Not Available. \n");

	//stop the execution 
	while(1);
}

/* 
 * Exception_Handler_8
 *   DESCRIPTION: the exception handler that handles the Double Fault
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */

void Exception_Handler_8(){

	//print out the error message
	printf("Exception: Double Fault. \n");

	//stop the execution 
	while(1);
}

/* 
 * Exception_Handler_9
 *   DESCRIPTION: the exception handler that handles the Coprocessor Segment Overrun
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */

void Exception_Handler_9(){

	//print out the error message
	printf("Exception: Coprocessor Segment Overrun. \n");

	//stop the execution 
	while(1);
}

/* 
 * Exception_Handler_10
 *   DESCRIPTION: the exception handler that handles the Invalid TSS
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */

void Exception_Handler_10(){

	//print out the error message
	printf("Exception: Invalid TSS. \n");

	//stop the execution 
	while(1);
}

/* 
 * Exception_Handler_11
 *   DESCRIPTION: the exception handler that handles the Segment Not Present
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */

void Exception_Handler_11(){

	//print out the error message
	printf("Exception: Segment Not Present. \n");

	//stop the execution 
	while(1);
}

/* 
 * Exception_Handler_12
 *   DESCRIPTION: the exception handler that handles the Stack-Segment Fault
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */

void Exception_Handler_12(){

	//print out the error message
	printf("Exception: Stack-Segment Fault. \n");

	//stop the execution 
	while(1);
}

/* 
 * Exception_Handler_13
 *   DESCRIPTION: the exception handler that handles the General Protection Fault
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */

void Exception_Handler_13(){

	//print out the error message
	printf("Exception: General Protection Fault. \n");

	//stop the execution 
	while(1);
}

/* 
 * Exception_Handler_14
 *   DESCRIPTION: the exception handler that handles the Page Fault
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */

void Exception_Handler_14(){

	//print out the error message
	printf("Exception: Page Fault. \n");

	//stop the execution 
	while(1);
}


/* 
 * Exception_Handler_16
 *   DESCRIPTION: the exception handler that handles the x87 Floating-Point Exception 
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */
void Exception_Handler_16()
{
	//print out the error message
	printf("EXCPETION: x87 Floating-Point Exception\n");
	//stop the execution 
	while(1);
}

/* 
 * Exception_Handler_17
 *   DESCRIPTION: the exception handler that handles the Alignment Check
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */
void Exception_Handler_17()
{
	//print out the error message
	printf("EXCPETION: Alignment Check\n");
	//stop the execution 
	while(1);
}

/* 
 * Exception_Handler_18
 *   DESCRIPTION: the exception handler that handles the Machine Check
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */
void Exception_Handler_18()
{
	//print out the error message
	printf("EXCPETION: Machine Check\n");
	//stop the execution 
	while(1);
}

/* 
 * Exception_Handler_19
 *   DESCRIPTION: the exception handler that handles the SIMD Floating-Point Exception
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */
void Exception_Handler_19()
{
	//print out the error message
	printf("EXCPETION: SIMD Floating-Point Exception\n");
	//stop the execution 
	while(1);
}

/* 
 * Exception_Handler_20
 *   DESCRIPTION: the exception handler that handles the Virtualization Exception
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */
void Exception_Handler_20()
{
	//print out the error message
	printf("EXCPETION: Virtualization Exception\n");
	//stop the execution 
	while(1);
}

/* 
 * Exception_Handler_30
 *   DESCRIPTION: the exception handler that handles the Security Exception 
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */
void Exception_Handler_30()
{
	//print out the error message
	printf("EXCPETION: Security Exception\n");
	//stop the execution 
	while(1);
}

/* 
 * system_calls
 *   DESCRIPTION: the handler for system calls
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: 
 *
 */
void system_calls()
{
	//print out the message
	printf("System Calls\n"); 
	
}


/* 
 * fill_interrupt_descriptor_table
 *   DESCRIPTION: fills in the interrupt table 
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: fill in the interrupt destable 
 *
 */
 void fill_interrupt_descriptor_table(){

 	/*create temporary structure to hold the entry*/ 
 	idt_desc_t idt_enry;

 
 	/*fill in all the exception entries*/
 	//exception_0
 	fill_interrupt_descriptor_table_entry(&idt_enry, KERNEL_CS, RESERVED4, 0, 1, 1, SIZE_32, 0, DPL_0, PRESENT);
 	SET_IDT_ENTRY(idt_enry, Exception_Handler_0);
 	idt[0] = idt_enry;

 	//exception_1
 	SET_IDT_ENTRY(idt_enry, Exception_Handler_1);
 	idt[1] = idt_enry;

 	//exception_2
 	SET_IDT_ENTRY(idt_enry, Exception_Handler_2);
 	idt[2] = idt_enry;

 	//exception_3
 	SET_IDT_ENTRY(idt_enry, Exception_Handler_3);
 	idt[3] = idt_enry;

 	//exception_4
 	SET_IDT_ENTRY(idt_enry, Exception_Handler_4);
 	idt[4] = idt_enry;

 	//exception_5
 	SET_IDT_ENTRY(idt_enry, Exception_Handler_5);
 	idt[5] = idt_enry;

 	//exception_6
 	SET_IDT_ENTRY(idt_enry, Exception_Handler_6);
 	idt[6] = idt_enry;

 	//exception_7
 	SET_IDT_ENTRY(idt_enry, Exception_Handler_7);
 	idt[7] = idt_enry;

 	//exception_8
 	SET_IDT_ENTRY(idt_enry, Exception_Handler_8);
 	idt[8] = idt_enry;

 	//exception_10
 	SET_IDT_ENTRY(idt_enry, Exception_Handler_10);
 	idt[10] = idt_enry;

 	//exception_11
 	SET_IDT_ENTRY(idt_enry, Exception_Handler_11);
 	idt[11] = idt_enry;

 	//exception_12
 	SET_IDT_ENTRY(idt_enry, Exception_Handler_12);
 	idt[12] = idt_enry;

 	//exception_13
 	SET_IDT_ENTRY(idt_enry, Exception_Handler_13);
 	idt[13] = idt_enry;

 	//exception_14
 	SET_IDT_ENTRY(idt_enry, Exception_Handler_14);
 	idt[14] = idt_enry;

 	//exception_16
 	SET_IDT_ENTRY(idt_enry, Exception_Handler_16);
 	idt[16] = idt_enry;

 	//exception_17
 	SET_IDT_ENTRY(idt_enry, Exception_Handler_17);
 	idt[17] = idt_enry;

 	//exception_18
 	SET_IDT_ENTRY(idt_enry, Exception_Handler_18);
 	idt[18] = idt_enry;

 	//exception_19
 	SET_IDT_ENTRY(idt_enry, Exception_Handler_19);
 	idt[19] = idt_enry;

 	//exception_20
 	SET_IDT_ENTRY(idt_enry, Exception_Handler_20);
 	idt[20] = idt_enry;

 	//exception_30
 	SET_IDT_ENTRY(idt_enry, Exception_Handler_30);
 	idt[30] = idt_enry;

 	//keyboard
 	fill_interrupt_descriptor_table_entry(&idt_enry, KERNEL_CS, RESERVED4, 0, 1, 1, SIZE_32, 0, DPL_0, PRESENT);
 	SET_IDT_ENTRY(idt_enry, keyboard_handler);
 	idt[33] = idt_enry;

 	//RTC
 	fill_interrupt_descriptor_table_entry(&idt_enry, KERNEL_CS, RESERVED4, 0, 1, 1, SIZE_32, 0, DPL_0, PRESENT);
 	SET_IDT_ENTRY(idt_enry, test_interrupts);
 	idt[40] = idt_enry;
 	

 	/*fill in the system call entry*/
 	fill_interrupt_descriptor_table_entry(&idt_enry, KERNEL_CS, RESERVED4, 1, 1, 1, SIZE_32, 0, DPL_0, PRESENT);
 	SET_IDT_ENTRY(idt_enry, system_calls);
 	idt[128] = idt_enry;
 }


 /* 
 * fill_interrupt_descriptor_table_entry
 *   DESCRIPTION: fills in one entry in the interrupt descriptor table
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: str.offset_31_16 
 *
 */

  void fill_interrupt_descriptor_table_entry(idt_desc_t * idt_enry, uint16_t seg_selector, uint8_t reserved4, 
  										uint32_t reserved3, uint32_t reserved2, uint32_t reserved1,	uint32_t size,
  									    uint32_t reserved0, uint32_t dpl, uint32_t present)
  {      
  		/*set IDT entry values*/
  		//set the segment selector
  		idt_enry->seg_selector = seg_selector;

  		//set all the reserved values
  		idt_enry->reserved4 = reserved4;
  		idt_enry->reserved3 = reserved3;
  		idt_enry->reserved2 = reserved2;
  		idt_enry->reserved1 = reserved1;
  		idt_enry->reserved0 = reserved0;

  		//set the size
  		idt_enry->size = size;

  		//set privilege level  
  		idt_enry->dpl = dpl;

  		//set the present bit
  		idt_enry->present = present;
  }


