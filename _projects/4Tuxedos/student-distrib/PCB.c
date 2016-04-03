#include "PCB.h"

void init_PCB(pcb_struct_t* pcb, uint32_t pid, uint32_t esp, uint32_t ebp, uint32_t eip, const parent_info_t parent )
{
	pcb->pid = pid;
	pcb->esp = esp;
	pcb->ebp = ebp;
	pcb->eip = eip;
	pcb->fd_array = esp & NEV_8KB;	//fd_array address = stack - 8KB
	init_FD(pcb->fd_array);
	pcb->parent.pid = parent.pid;
	pcb->parent.fd_array = parent.fd_array;
}


