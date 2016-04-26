/*signal.h*/
/*
- Intro to Signal:
	- A signal is a very short message that may be sent to ba process of a group of processes
	- An important characteristic of signals is that they may be sent at any time to a process whose state is usually unpredictable
	- Signals sent to a process that is not currently executing must be saved by the kernel until that process resumes execution
	- Signals that have been generated but not yet delivered are called pending signals
		- only one pending signal of a given type may exist for a process; 
		- additional pending signals of the same type to the same process are not queued but simply discarded 
	- Signals are usually delivered only to the currently running process (that is, to the current process).
	- When a process executes a signal-handler function, it usually masks the corresponding signal
*/


#ifndef SIGNAL_H
#define SIGNAL_H

#include "types.h"
#include "PCB.h"
#include "lib.h"
/*define the signal flag values*/
#define KILL_TASK 1 
#define IGNORE    2
/*define the signal numbers*/
#define DIV_ZERO  0
#define SEGFAULT  1
#define INTERRUPT 2
#define ALARM	  3
#define USER1     4

typedef struct sig_stack_frame{
	uint32_t ret_addr;
	uint32_t sig_num;
	hardware_context_t hw_context;
	uint32_t sigreturn_ptr;
}sig_stack_frame_t;

typedef struct hw_context{
	uint32_t ebx;
	uint32_t ecx;
	uint32_t edx;
	uint32_t esi;
	uint32_t edi;
	uint32_t ebp;
	uint32_t eax;
	uint32_t ds;
	uint32_t es;
	uint32_t fs;
	uint32_t irq_exp_num;
	uint32_t err_code;
	uint32_t  ret_addr;
	uint32_t cs;
	uint32_t eflags;
	uint32_t esp;
	uint32_t ss;
}hardware_context_t;

/*helper functions for signal handling*/
/*function that sends a signal to a specific process
  - @param: 
  	- sig_num: the signal number
  	- info: either a address to a siginfo struc, or a number
  	- des_pcb: the pcb of the destination process 
*/
 uint32_t specific_send_sig_info(uint32_t sig_num, void* info, pcb_struct_t* des_pcb);
 uint32_t send_signal( );
 uint32_t do_signal( );
 uint32_t handle_signal( );
 uint32_t setup_frame();
 uint32_t setup_rt_frame();
 uint32_t restore_sigcontext();
 void sig_kill(); //cannot be masked, cannot be blocked 
 void sig_ignore(); //does nothing





#endif

