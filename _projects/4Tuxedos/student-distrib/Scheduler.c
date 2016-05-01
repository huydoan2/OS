#include "Scheduler.h"
#include "lib.h"
#include "i8259.h"
#include "keyboard.h"
#include "x86_desc.h"
#include "Paging.h"

#define pit_irq_num 0
#define MAX_TERMINAL 3
#define tss_offset   4
#define EIGHT_KB       0x2000
#define EIGHT_MB     0x800000

/* the terminal currently on schedule */
uint32_t scheduling_terminal = 0;
/* current and next process ID */
uint32_t curr_pid, next_pid;
/* array of active pids*/
extern uint32_t current_pid[MAX_TERMINAL];
/* terminal ID that is currently being displayed*/
extern int current_terminal;
/* function that finds next active process ID*/
uint32_t find_next_pid();


/* 
 * pit_handler
 *   DESCRIPTION: interrupt handler for the pit, switch tasks for scheduler
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: video memory mapping is changed 
 *
 */
void pit_handler()
{
    /* send EOI to the PIC */
	send_eoi(pit_irq_num);
    /* obtain the current and next pid*/
    curr_pid = current_pid[scheduling_terminal];
    next_pid = find_next_pid();
    /*if there is only one process, no need to switch*/
    if(next_pid == 0 || curr_pid == next_pid)
        return;        
    /* switch the video memory mapping */
    process_switch_mem_map(scheduling_terminal); 
    /* switch from the current pid to the next pid */
    switch_task(curr_pid, next_pid);
    return;

}

/* 
 * switch_task
 *   DESCRIPTION: function that switches from the current process to the next process
 *-----------------------------------------------------------------------------------
 *   INPUTS: 
 *      - curr_pid: pid of the currently running process
 *      - next_pid: the pid of the process to be switched to
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: CPU is executing the process has the next_pid after switching 
 *
 */

void switch_task(const uint32_t curr_pid,const uint32_t next_pid)
{
    /* enable the keyboard only if next process belongs to the terminal on display */
    if(scheduling_terminal == current_terminal)
        enable_irq(1);
    else
        disable_irq(1);

    //set tss registers
    tss.ss0 =  KERNEL_DS;
    tss.esp0 = EIGHT_MB - EIGHT_KB * (next_pid - 1); 

    pcb_struct_t *current_pcb, *next_pcb;
    uint32_t esp = 0;
    uint32_t ebp = 0;

    /* save esp and ebp from the currnet process */
    asm volatile("mov %%esp, %0" :"=c"(esp));
  	asm volatile("mov %%ebp, %0" :"=c"(ebp)); 
    current_pcb = find_PCB(curr_pid);
    current_pcb->esp = esp;
    current_pcb->ebp = ebp;    

    /* map the program image from the next process */
	map_page(next_pid);
    //update esp and ebp of the next process
    next_pcb = find_PCB(next_pid);
    asm volatile("movl %0, %%esp"
                     :
                     :"c"(next_pcb->esp)
                     :"%esp"
                     );
   asm volatile("movl %0, %%ebp" 
                     :
                     :"c"(next_pcb->ebp)
                     :"%ebp"
                     );
}


/* 
 * find_next_pid
 *   DESCRIPTION: function that finds the next active pid
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: next pid 
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: 
 *
 */
uint32_t find_next_pid()
{
	int original;
    original = scheduling_terminal;
	do{
		scheduling_terminal+=1;
		scheduling_terminal%=MAX_TERMINAL;
		if(original == scheduling_terminal)
         break;
	}while(current_pid[scheduling_terminal] == 0);
	return current_pid[scheduling_terminal];
}
