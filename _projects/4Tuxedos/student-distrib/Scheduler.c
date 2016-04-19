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
uint32_t active_pid_index = 0;
uint32_t curr_pid, next_pid;
/*the current terminal that is on display*/
extern int current_terminal;
extern uint32_t current_pid[MAX_TERMINAL];
extern uint32_t process_info[9];
uint32_t find_next_pid();

void pit_handler()
{
	send_eoi(pit_irq_num);
    curr_pid = current_pid[active_pid_index];
    next_pid = find_next_pid();
    switch_task(curr_pid, next_pid);
}

void switch_task(uint32_t curr_pid,uint32_t next_pid)
{
    if(curr_pid == next_pid)
        return;

    /*call the round robin scheduler*/
    pcb_struct_t *current_pcb, *next_pcb;

    uint32_t esp = 0;
    uint32_t ebp = 0;

    current_pcb = find_PCB(curr_pid);
    current_pcb->esp = esp;
    current_pcb->ebp = ebp;
    //save all the registers values
    current_pcb->registers.eax = process_info[0];
    current_pcb->registers.ecx = process_info[1];
    current_pcb->registers.edx = process_info[2];
    current_pcb->registers.ebx = process_info[3];
    current_pcb->registers.esp = process_info[4];
    current_pcb->registers.ebp = process_info[5];
    current_pcb->registers.esi = process_info[6];
    current_pcb->registers.edi = process_info[7];
    current_pcb->registers.eflags = process_info[8];
   
 
    /*set tss registers*/
    tss.ss0 =  KERNEL_DS;
    tss.esp0 = EIGHT_MB - tss_offset - EIGHT_KB * (next_pid); 

    /*update esp and ebp of the next process*/
    next_pcb = find_PCB(next_pid);
    esp = next_pcb->esp;
    ebp = next_pcb->ebp;
    
    process_info[0] =  next_pcb->registers.eax;
    process_info[1] =  next_pcb->registers.ecx;
    process_info[2] =  next_pcb->registers.edx;
    process_info[3] =  next_pcb->registers.ebx;
    process_info[4] =  next_pcb->registers.esp;
    process_info[5] =  next_pcb->registers.ebp;
    process_info[6] =  next_pcb->registers.esi;
    process_info[7] =  next_pcb->registers.edi;
    process_info[8] =  next_pcb->registers.eflags;
    
    /*swtich the program image and video memory mapping */
    process_switch_mem_map(next_pid, active_pid_index, current_terminal); 

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
