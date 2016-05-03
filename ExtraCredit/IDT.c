/* IDT.c 
 * - implementation of exception handlers, interrupt handlers, and 
 *   system calls;
 * - functions for filling the IDT
 * vim:ts=4 noexpandtab
 */
#include "IDT.h"
#include "syscall_linkage.h"
#include "irq_handler.h"
#include "systemCalls.h"
#include "PCB.h"

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
#define RESERVED3_0 0
#define RESERVED3_1 1
#define RESERVED2 1
#define RESERVED1 1
#define RESERVED0 0
#define exception_status 2
#define exception_retval 255

void exception_termination();
extern uint32_t current_pid[3];
extern uint32_t scheduling_terminal;
void update_siginfo_exp(uint32_t sig_num, uint32_t err_code);
uint32_t find_avail_siginfo(pcb_struct_t * cur_pcb);
uint32_t siginfo_index[3] = {-1};


/* 
 * EX_0
 *   DESCRIPTION: the exception handler that handles the Divide-by-zero Error
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */

void EX_0(){

	//print out the error message
    clear();
	printf("Exception: Divide-by-zero Error. \n");
	update_siginfo_exp(0, 0);

}

/* 
 * EX_1
 *   DESCRIPTION: the exception handler that handles the Debug 
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */

void EX_1(){

	//print out the error message
    clear();
	printf("Exception: Debug. \n");
	update_siginfo_exp(1, 1);
}
/* 
 * EX_2
 *   DESCRIPTION: the exception handler that handles the Non-maskable Interrupt
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */

void EX_2(){

	//print out the error message
    clear();
	printf("Exception: Non-maskable Interrupt. \n");
	update_siginfo_exp(1, 2);
}

/* 
 * EX_3
 *   DESCRIPTION: the exception handler that handles the Breakpoint
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */

void EX_3(){

	//print out the error message
    clear();
	printf("Exception: Breakpoint. \n");
	update_siginfo_exp(1, 3);
}

/* 
 * EX_4
 *   DESCRIPTION: the exception handler that handles the Overflow
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */

void EX_4(){

	//print out the error message
    clear();
	printf("Exception: Overflow. \n");
	update_siginfo_exp(1, 4);
}

/* 
 * EX_5
 *   DESCRIPTION: the exception handler that handles the Bound Range Exceeded
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */

void EX_5(){

	//print out the error message
    clear();
	printf("Exception: Bound Range Exceeded. \n");
	update_siginfo_exp(1, 5);
}

/* 
 * EX_6
 *   DESCRIPTION: the exception handler that handles the Invalid Opcode
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */

void EX_6(){

	//print out the error message
    clear();
	printf("Exception: Invalid Opcode. \n");
	update_siginfo_exp(1, 6);
}

/* 
 * EX_7
 *   DESCRIPTION: the exception handler that handles the Device Not Available
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */

void EX_7(){

	//print out the error message
    clear();
	printf("Exception: Device Not Available. \n");
	update_siginfo_exp(1, 7);
}

/* 
 * EX_8
 *   DESCRIPTION: the exception handler that handles the Double Fault
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */

void EX_8(){

	//print out the error message
    clear();
	printf("Exception: Double Fault. \n");
	update_siginfo_exp(1, 8);
}

/* 
 * EX_9
 *   DESCRIPTION: the exception handler that handles the Coprocessor Segment Overrun
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */

void EX_9(){

	//print out the error message
    clear();
	printf("Exception: Coprocessor Segment Overrun. \n");
	update_siginfo_exp(1, 9);
}

/* 
 * EX_10
 *   DESCRIPTION: the exception handler that handles the Invalid TSS
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */

void EX_10(){

	//print out the error message
    clear();
	printf("Exception: Invalid TSS. \n");
	update_siginfo_exp(1, 10);
}

/* 
 * EX_11
 *   DESCRIPTION: the exception handler that handles the Segment Not Present
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */

void EX_11(){

	//print out the error message
    clear();
	printf("Exception: Segment Not Present. \n");
	update_siginfo_exp(1, 11);
}

/* 
 * EX_12
 *   DESCRIPTION: the exception handler that handles the Stack-Segment Fault
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */

void EX_12(){

	//print out the error message
    clear();
	printf("Exception: Stack-Segment Fault. \n");
	update_siginfo_exp(1, 12);
}

/* 
 * EX_13
 *   DESCRIPTION: the exception handler that handles the General Protection Fault
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */

void EX_13(){

	//print out the error message
    clear();
	printf("Exception: General Protection Fault. \n");
	update_siginfo_exp(1, 13);
}

/* 
 * EX_14
 *   DESCRIPTION: the exception handler that handles the Page Fault
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */

void EX_14(){

	//print out the error message
    //clear(); 
	printf("Exception: Page Fault. \n");
	update_siginfo_exp(1, 14);
}


/* 
 * EX_16
 *   DESCRIPTION: the exception handler that handles the x87 Floating-Point Exception 
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */
void EX_16()
{
	//print out the error message
    clear();
	printf("EXCPETION: x87 Floating-Point Exception\n");
	update_siginfo_exp(1, 16);
}

/* 
 * EX_17
 *   DESCRIPTION: the exception handler that handles the Alignment Check
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */
void EX_17()
{
	//print out the error message
    clear();
	printf("EXCPETION: Alignment Check\n");
	update_siginfo_exp(1, 17);
}

/* 
 * EX_18
 *   DESCRIPTION: the exception handler that handles the Machine Check
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */
void EX_18()
{
	//print out the error message
    clear();
	printf("EXCPETION: Machine Check\n");
	update_siginfo_exp(1, 18);
}

/* 
 * EX_19
 *   DESCRIPTION: the exception handler that handles the SIMD Floating-Point Exception
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */
void EX_19()
{
	//print out the error message
    clear();
	printf("EXCPETION: SIMD Floating-Point Exception\n");
	update_siginfo_exp(1, 19);
}

/* 
 * EX_20
 *   DESCRIPTION: the exception handler that handles the Virtualization Exception
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */
void EX_20()
{
	//print out the error message
    clear();
	printf("EXCPETION: Virtualization Exception\n");
	update_siginfo_exp(1, 20);
}

/* 
 * EX_30
 *   DESCRIPTION: the exception handler that handles the Security Exception 
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: print out the error message and stop the current execution 
 *
 */
void EX_30()
{
	//print out the error message
    clear();
	printf("EXCPETION: Security Exception\n");
	update_siginfo_exp(1, 30);
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
 	fill_interrupt_descriptor_table_entry(&idt_enry, KERNEL_CS, RESERVED4, RESERVED3_1, RESERVED2, RESERVED1, SIZE_32, RESERVED0, DPL_0, PRESENT);
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

 	//PIT
 	//32 is 0x20 corresponding to irq 0
 	fill_interrupt_descriptor_table_entry(&idt_enry, KERNEL_CS, RESERVED4, RESERVED3_0, RESERVED2, RESERVED1, SIZE_32, RESERVED0, DPL_0, PRESENT);
 	SET_IDT_ENTRY(idt_enry, pit_irq);
 	idt[32] = idt_enry;	

 	//keyboard
 	//33 is 0x21 corresponding to irq 1
 	fill_interrupt_descriptor_table_entry(&idt_enry, KERNEL_CS, RESERVED4, RESERVED3_0, RESERVED2, RESERVED1, SIZE_32, RESERVED0, DPL_0, PRESENT);
 	SET_IDT_ENTRY(idt_enry, keyboard_irq);
 	idt[33] = idt_enry;

 	//RTC
 	//40 is 0x28 corresponding to irq 8
 	fill_interrupt_descriptor_table_entry(&idt_enry, KERNEL_CS, RESERVED4, RESERVED3_0, RESERVED2, RESERVED1, SIZE_32, RESERVED0, DPL_0, PRESENT);
 	SET_IDT_ENTRY(idt_enry, rtc_irq);
 	idt[40] = idt_enry;

 	/*fill in the system call entry*/
 	//128 is 0x80 corresponding to irq 8
 	fill_interrupt_descriptor_table_entry(&idt_enry, KERNEL_CS, RESERVED4, RESERVED3_1, RESERVED2, RESERVED1, SIZE_32, RESERVED0, DPL_3, PRESENT);
 	SET_IDT_ENTRY(idt_enry, syscalls);
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

void exception_termination()
{
	exception_flag = 1;
	syscall_halt(exception_retval);
}
/**/
uint32_t find_avail_siginfo(pcb_struct_t * cur_pcb){
	uint32_t i;
	for(i = 0; i < 3; i++){
		if(cur_pcb->siginfo[i].sig_num == -1)
			return i;
	}
	return -1;
}
/* update the signal information in the PCB block */
void update_siginfo_exp(uint32_t sig_num, uint32_t err_code ){
	uint32_t cur_pid;
	uint32_t siginfo_idx;
	pcb_struct_t * cur_pcb;
	//find the current pid scheduling_terminal
	cur_pid = current_pid[scheduling_terminal];
	//find the PCB of this process 
	cur_pcb = find_PCB(cur_pid);

	//update the signal informatio 
	siginfo_idx = find_avail_siginfo(cur_pcb);
	
	cur_pcb->siginfo[siginfo_idx].sig_num = sig_num;
    cur_pcb->siginfo[siginfo_idx].sig_err = err_code;
    if(sig_num == 0){
   		cur_pcb->siginfo[siginfo_idx].sigaction.sa_handler = sigaction_dividedbyzero.sa_handler;
   	}
   	else{
   		cur_pcb->siginfo[siginfo_idx].sigaction.sa_handler = sigaction_segfault.sa_handler;
   	}

   	cur_pcb->siginfo[siginfo_idx].sigaction.sa_flags = 0; //kill the task

   	cur_pcb->siginfo[siginfo_idx].sigaction.sa_mask = 0; //DONT KNOW THAT THIS IS SHABI
    siginfo_index[scheduling_terminal] = siginfo_idx;

}
void update_siginfo_int(uint32_t sig_num, uint32_t err_code ){
	uint32_t cur_pid;
	uint32_t siginfo_idx;
	pcb_struct_t * cur_pcb;
	//find the current pid scheduling_terminal
	cur_pid = current_pid[scheduling_terminal];
	//find the PCB of this process 
	cur_pcb = find_PCB(cur_pid);

	//update the signal informatio 
	siginfo_idx = find_avail_siginfo(cur_pcb);
	
	cur_pcb->siginfo[siginfo_idx].sig_num = sig_num;
    cur_pcb->siginfo[siginfo_idx].sig_err = err_code;
    if(sig_num == 2){
   		cur_pcb->siginfo[siginfo_idx].sigaction.sa_handler = sigaction_interrupt.sa_handler;
   		cur_pcb->siginfo[siginfo_idx].sigaction.sa_flags = 0; //kill the task
   	}
   	else if(sig_num == 3){
   		cur_pcb->siginfo[siginfo_idx].sigaction.sa_handler = sigaction_alarm.sa_handler;
   		cur_pcb->siginfo[siginfo_idx].sigaction.sa_flags = 1; //ignore the task
   	}
   	else{
   		cur_pcb->siginfo[siginfo_idx].sigaction.sa_handler = sigaction_user1.sa_handler;
   		cur_pcb->siginfo[siginfo_idx].sigaction.sa_flags = 1; //ignore the task
   	}
   	cur_pcb->siginfo[siginfo_idx].sigaction.sa_mask = 0; //DONT KNOW THAT THIS IS SHABI
    siginfo_index[scheduling_terminal] = siginfo_idx;

}

