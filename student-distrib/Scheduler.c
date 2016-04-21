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

/*current active pid*/
uint32_t scheduling_terminal = 0;
uint32_t curr_pid, next_pid;
/*the current terminal that is on display*/
extern uint32_t current_pid[MAX_TERMINAL];
extern uint32_t process_info[2];
extern int current_terminal;
uint32_t find_next_pid();

int32_t pit_handler()
{
	send_eoi(pit_irq_num);
    curr_pid = current_pid[scheduling_terminal];
    next_pid = find_next_pid();
    if(next_pid == 0 || curr_pid == next_pid)
        return 1;    
    process_switch_mem_map(scheduling_terminal); 
    switch_task(curr_pid, next_pid);
    return 0;

}


void switch_task_from_pit(uint32_t curr_pid,uint32_t next_pid)
{

    //return if there is noly one process
    if(curr_pid == next_pid)
        return ;

    //store the esp and ebp of the current process
    pcb_struct_t *current_pcb, *next_pcb;
    uint32_t esp, ebp;

    asm volatile("mov %%esp, %0" :"=c"(esp));
    asm volatile("mov %%ebp, %0" :"=c"(ebp)); 

    current_pcb = find_PCB(curr_pid);
    current_pcb->esp = esp;
    current_pcb->ebp = ebp;    
   
    //set tss registers
    tss.ss0 =  KERNEL_DS;
    tss.esp0 = EIGHT_MB - tss_offset - EIGHT_KB * (next_pid); 

    //re-map the program image
    map_page(next_pid);

    //update esp and ebp of the next process
    next_pcb = find_PCB(next_pid);
    // esp = next_pcb->esp;
    // ebp = next_pcb->ebp;
    process_info[0] = next_pcb->esp;
    process_info[1] = next_pcb->ebp;

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


void switch_task(const uint32_t curr_pid,const uint32_t next_pid)
{
	if(curr_pid == next_pid)
        return;

    if(scheduling_terminal == current_terminal)
        enable_irq(1);
    else
        disable_irq(1);

    pcb_struct_t *current_pcb, *next_pcb;
    uint32_t esp = 0;
    uint32_t ebp = 0;

    asm volatile("mov %%esp, %0" :"=c"(esp));
  	asm volatile("mov %%ebp, %0" :"=c"(ebp)); 

    current_pcb = find_PCB(curr_pid);
    current_pcb->esp = esp;
    current_pcb->ebp = ebp;    
   

    //set tss registers
    tss.ss0 =  KERNEL_DS;
    tss.esp0 = EIGHT_MB - tss_offset - EIGHT_KB * (next_pid); 

	map_page(next_pid);
    //update esp and ebp of the next process
    next_pcb = find_PCB(next_pid);
    // esp = next_pcb->esp;
    // ebp = next_pcb->ebp;
    
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
   

    //swtich the program image and video memory mapping 
    //process_switch_mem_map(next_pid, scheduling_terminal, current_terminal); 
}

uint32_t find_next_pid()
{
	int original;
    original = scheduling_terminal;
	do{
		scheduling_terminal+=1;
		scheduling_terminal%=3;
		if(original == scheduling_terminal)
         break;
	}while(current_pid[scheduling_terminal] == 0);

	return current_pid[scheduling_terminal];

    //return current_pid[(current_terminal+1) % 3];
}
