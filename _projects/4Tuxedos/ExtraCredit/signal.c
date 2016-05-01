/*signal.c*/

#include "signal.h"
#include "lib.h"
#include "x86_desc.h"

extern uint32_t current_pid[3];
extern uint32_t scheduling_terminal;
extern uint32_t siginfo_index[3];
uint32_t regs[17];
extern int32_t syscall_halt(uint8_t status);
extern uint32_t sig_return_size, sf_start;
void do_signal()
{
	uint32_t cur_pid;
	uint32_t sig_num;
	uint32_t err_code;
	uint32_t sa_flags;
	uint32_t sa_mask;
	void* sa_handler;
	uint32_t siginfo_idx = siginfo_index[scheduling_terminal];

	pcb_struct_t * cur_pcb;
	//find the current pid scheduling_terminal
	cur_pid = current_pid[scheduling_terminal];
	//find the PCB of this process 
	cur_pcb = find_PCB(cur_pid);
	//have to do masking
    //???????
    //get the siginfo

    sig_num = cur_pcb->siginfo[siginfo_idx].sig_num;
    err_code = cur_pcb->siginfo[siginfo_idx].sig_err;
    sa_handler = cur_pcb->siginfo[siginfo_idx].sigaction.sa_handler;
    sa_flags = cur_pcb->siginfo[siginfo_idx].sigaction.sa_flags;
    sa_mask = cur_pcb->siginfo[siginfo_idx].sigaction.sa_mask;

    //set up the stack frame 
	setup_frame(sig_num);

	// //execute the signal handler 
	// if(sa_handler == NULL){
	// 	if(sa_flags == 0){
	// 		sig_kill(cur_pid);
	// 	}
	// 	else{
	// 		sig_ignore();
	// 	}
	// }
	// else {
	// 	asm volatile("movl %0, %%eax"
 //                     :
 //                     :"c"(sa_handler)
 //                     :"%eax"                     //could be wrong
 //                     );
 //       asm volatile("call *%eax");

	// }
	// printf("do_signal done\n");
	return;
}

void setup_frame(uint32_t sig_num)
{
	/* push the sigreturn on to the sigreturn */
	uint32_t esp;
	//uint32_t temp_num = sig_num;
	//push sigreturn context
	
	//asm volatile("mov %%esp, %0" :"=c"(esp));
	esp = regs[15];

	esp -= sig_return_size;               
	memcpy((void*)(esp), (void*)&sf_start, (sig_return_size));


	/* push the harware context on to the stack */
	esp -= sizeof(hardware_context_t);

	memcpy((void*)(esp), (void*)regs, sizeof(hardware_context_t));

	/* push the sig_num */
	esp -= 4;
	memcpy((void*)(esp), (void*)&sig_num, sizeof(uint32_t));


	esp -= 4;
	uint32_t ret_addr = &sf_start;
	memcpy((void*)(esp), (void*)&ret_addr, sizeof(uint32_t));

	regs[15] = esp;

	return;

}

 void sig_kill(uint32_t pid){//cannot be masked, cannot be blocked
 	syscall_halt(0);
 	return;
 } 

void sig_ignore(){ //does nothing
	return;
}
