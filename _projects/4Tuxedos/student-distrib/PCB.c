
/*PCB.c
 * - functions to initialize and find the process contol blocks
 */
#include "PCB.h"

#define FIRST_PCB_ADDR 0x7FE000 //starting address of the first PCB 
#define EIGHT_KB       0x2000

uint32_t num_active_process = 0;

/*
* void init_PCB(
*   Description:
*       function that initialize a PCB for the new process
*   Inputs: 
*		-pcb: the pointer to the new PCB structure
*		-pid: the pid of the new process
*       -eip: the eip of the new process
*       -parent: structure that stores the information about the parent process
*   Return Value: void
*	Side Effect: new pcb initialized in the proper memory location
*/
void init_PCB(pcb_struct_t* pcb, uint32_t pid, uint32_t eip, uint32_t esp, uint32_t ebp, const parent_info_t parent)
{  
	
    pcb->esp = esp;
    pcb->ebp = ebp;
	pcb->pid = pid;
	pcb->eip = eip;	
	init_FD(pcb->fd_array);
	pcb->parent.pid = parent.pid;
	pcb->parent.esp = parent.esp;
	pcb->parent.ebp = parent.ebp;
	pcb->parent.esp0 = parent.esp0;
	pcb->parent.ss0 = parent.ss0;
	pcb->parent.fd_array = parent.fd_array;
	pcb->active = IN_USE;

}

/*
* pcb_struct_t* find_PCB(
*   Description:
		function that returns the pcb structure with given pid
*   Inputs: 
*		-pid: the pid of the PCB block corresponding to 
*   Return Value: void
*   Side Effect: none
*/
pcb_struct_t* find_PCB(uint32_t pid)
{
	pcb_struct_t* PCB =(pcb_struct_t*)(FIRST_PCB_ADDR - (pid-1)*EIGHT_KB );
	return PCB;
}
