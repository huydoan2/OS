#include "Scheduler.h"
#include "lib.h"
#include "i8259.h"
#include "keyboard.h"
#include "PCB.h"
#include "x86_desc.h"
#include "Paging.h"

#define pit_irq_num 0
#define MAX_TERMINAL 3
#define tss_offset   4
#define EIGHT_KB       0x2000
#define EIGHT_MB     0x800000

/*current active pid*/
uint32_t active_pid_index = 0;
/*the current terminal that is on display*/
extern int current_terminal;
extern uint32_t current_pid[MAX_TERMINAL];
uint32_t find_next_pid();

void pit_handler()
{
	cli(); //WHERE IS THE CORESP STI FOR THIS?
	send_eoi(pit_irq_num);
    uint32_t curr_pid, next_pid;
    curr_pid = current_pid[active_pid_index];
    next_pid = find_next_pid();
	switch_task(curr_pid, next_pid);
    sti();
}

void switch_task(uint32_t curr_pid,uint32_t next_pid)
{
    if(curr_pid == next_pid)
        return;

    /*call the round robin scheduler*/
    pcb_struct_t *current_pcb, *next_pcb;
    uint32_t esp = 0;
    uint32_t ebp = 0;

    /*get esp and ebp*/
    asm volatile("movl %%esp, %0"
                     :"=c"(esp)
                     :
                     :"%esp"
                     );
    asm volatile("movl %%ebp, %0" 
                     :"=c"(ebp)
                     :
                     :"%ebp"
                     );

    current_pcb = find_PCB(curr_pid);
    current_pcb->esp = esp;
    current_pcb->ebp = ebp;

   

    /*set tss registers*/
    tss.ss0 = KERNEL_DS;
    tss.esp0 = EIGHT_MB - tss_offset - EIGHT_KB * (next_pid); 
    process_switch_mem_map(next_pid, active_pid_index, current_terminal); next_pcb = find_PCB(next_pid);
    
    esp = next_pcb->esp;
    ebp = next_pcb->ebp;
    /*update esp and ebp*/
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
}

uint32_t find_next_pid()
{
	int original = active_pid_index;
	do{
		active_pid_index+=1;
		active_pid_index%=3;
		if(original == active_pid_index) break;
	}while(current_pid[active_pid_index] == 0);
	return current_pid[active_pid_index];
}
