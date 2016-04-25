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
};

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
 void kill_task();





#endif

