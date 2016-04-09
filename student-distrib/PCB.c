#include "PCB.h"

#define FIRST_PCB_ADDR 0x7FE000 //starting address of the first PCB 
#define EIGHT_KB       0x2000


void init_PCB(pcb_struct_t* pcb, uint32_t pid, uint32_t eip, const parent_info_t parent )
{
	pcb->pid = pid;
	pcb->eip = eip;
	//pcb->fd_array = (file_desc_t*)(esp - PCB_OFFSET);	//fd_array address = stack - 8KB
	init_FD(pcb->fd_array);
	pcb->parent.pid = parent.pid;
	pcb->parent.esp = parent.esp;
	pcb->parent.ebp = parent.ebp;
	pcb->parent.fd_array = parent.fd_array;
}


/*return the pointer of PCB block, given a pid*/
pcb_struct_t* find_PCB(uint32_t pid){

	pcb_struct_t* PCB =(pcb_struct_t*)(FIRST_PCB_ADDR - (pid)*EIGHT_KB );

	return PCB;
}
