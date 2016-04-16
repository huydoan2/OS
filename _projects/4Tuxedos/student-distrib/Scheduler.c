#include "Scheduler.h"
#include "lib.h"
#include "i8259.h"
#include "keyboard.h"
#include "PCB.h"
#include "x86_desc.h"
#include "Paging.h"

#define pit_irq_num 0
#define MAX_TERMINAL 3

/*current active pid*/
uint32_t active_pid_index = 0;
/*the current terminal that is on display*/
extern int current_terminal;
extern uint32_t current_pid[MAX_TERMINAL];
uint32_t find_next_pid();

void pit_handler()
{
	send_eoi(pit_irq_num);
	
	uint32_t curr_pid, next_pid;
	curr_pid = current_pid[active_pid_index];
	next_pid = find_next_pid();
	if(curr_pid == next_pid)
		return;

	/*call the round robin scheduler*/
	pcb_struct_t *current_pcb, *next_pcb;
	uint32_t esp = 0;
    uint32_t ebp = 0;

    /*get esp and ebp*/
    asm volatile("movl %0, %%esp"
                     :
                     :"c"(esp)
                     :"%esp"
                     );
    asm volatile("movl %0, %%ebp" 
                     :
                     :"c"(ebp)
                     :"%ebp"
                     );

    current_pcb = find_PCB(curr_pid);
    current_pcb->esp = esp;
    current_pcb->ebp = ebp;

    next_pcb = find_PCB(next_pid);
  	/*set tss registers*/
    tss.ss0 = KERNEL_DS;
    tss.esp0 = next_pcb->esp;

    map_page(next_pid);
	
}

uint32_t find_next_pid()
{
	int original = active_pid_index;
	do{
		active_pid_index+=1;
		active_pid_index%=3;
		if(original == active_pid_index)
			break;
	}while(current_pid[active_pid_index] == 0);
	return current_pid[active_pid_index];
}
