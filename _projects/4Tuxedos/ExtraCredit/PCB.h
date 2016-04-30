/*PCB.h
 * - structures and functions for process control blocks 
 */

#ifndef PCB_H
#define PCH_H

#include "types.h"
#include "file_desc.h"

#define PCB_OFFSET 0x1FFF
#define IN_USE		1
#define EMPTY		0
/*signal handling structures*/
/*structure that stores the properties of a signal */
typedef struct sigaction{
	 void* sa_handler;	// specifies the type of action (0 = default)
	 uint32_t sa_flags; // specifies how the signal must be handled
	 uint32_t sa_mask;  // specifies the signals to be masked  

}sigaction_t;

/*structure that stores all the information about a signal */
typedef struct siginfo{
	uint32_t sig_num;	//the signal numer
	uint32_t sig_err;   //the error code of the instruction 
	sigaction_t sigaction; //handler info
}siginfo_t;

/*crate a sigaction structure for each signal*/
sigaction_t sigaction_dividedbyzero;

sigaction_t sigaction_segfault;


sigaction_t sigaction_interrupt;


sigaction_t sigaction_alarm;


sigaction_t sigaction_user1;


/*structure for the parent process information */
typedef struct {
	uint32_t pid;	
	uint32_t esp;
	uint32_t esp0;
	uint32_t ss0;
	uint32_t ebp;
    file_desc_t* fd_array;
} parent_info_t;

 typedef struct {
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
	uint32_t eflags;
}process_info_t;

/*structure for PCB*/
typedef struct {
	uint32_t pid;	
	uint32_t eip;
	uint32_t esp;
	uint32_t ebp;
	file_desc_t fd_array[FD_SIZE];
	parent_info_t parent;
	process_info_t registers;
	uint8_t* arg_buf;
	uint32_t active; 
	siginfo_t siginfo[3];
}pcb_struct_t;




/*function that initializes a PCB for the new process */

void init_PCB(pcb_struct_t* pcb, uint32_t pid, uint32_t eip, uint32_t esp, uint32_t ebp, const parent_info_t parent );



/*function that finds the corresponding PCB given a pid*/
pcb_struct_t* find_PCB(uint32_t pid);

#endif
