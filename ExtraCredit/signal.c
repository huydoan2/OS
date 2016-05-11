/*signal.c*/

#include "signal.h"
#include "lib.h"
#include "x86_desc.h"

extern uint32_t current_pid[3];
extern uint32_t scheduling_terminal;
//extern uint32_t current_terminal;
extern uint32_t siginfo_index[3];
uint32_t curr_signal = -1;
uint32_t regs[17];
uint32_t regs_x[17];
extern int32_t syscall_halt(uint8_t status);
extern uint32_t sig_return_size, sf_start;
extern int32_t counter;
void do_signal()//for exception 
{
	uint32_t cur_pid;
	uint32_t ret_esp;
	uint32_t sig_num;
	uint32_t err_code;
	uint32_t sa_flags;
	uint32_t sa_mask;
	void* sa_handler;
	cli();
	uint32_t siginfo_idx = siginfo_index[scheduling_terminal];

	pcb_struct_t * cur_pcb;
	//find the current pid scheduling_terminal
	cur_pid = current_pid[scheduling_terminal];
	//find the PCB of this process 
	cur_pcb = find_PCB(cur_pid);

    sig_num = cur_pcb->siginfo[siginfo_idx].sig_num;
    cur_pcb->siginfo[siginfo_idx].sig_num = -1;
    err_code = cur_pcb->siginfo[siginfo_idx].sig_err;
    sa_handler = cur_pcb->siginfo[siginfo_idx].sigaction.sa_handler;
    sa_flags = cur_pcb->siginfo[siginfo_idx].sigaction.sa_flags;
    sa_mask = cur_pcb->siginfo[siginfo_idx].sigaction.sa_mask;
    curr_signal = sig_num;
    //set up the stack frame 
	ret_esp = setup_frame(sig_num);

	//execute the signal handler
	

	if(sa_handler == NULL){
			 sti(); 
		if(sa_flags == 0){
			sig_kill(cur_pid);
		}
		else{
			sig_ignore();
		}
	}
	else {

		// asm volatile("movl %0, %%esp"
  //                    :
  //                    :"c"(regs[15])
  //                    );
		// asm volatile("movl %0, %%eax"
  //                    :
  //                    :"c"(sa_handler)
  //                    );
  //      	asm volatile("jmp *%eax");

       	uint32_t user_ds = USER_DS;
  		uint32_t user_cs = USER_CS;

		       	//push user ss
		   asm volatile("movl %0, %%eax;\n"
		                "pushl %%eax;"
		               :                       /* output */
		               : "r" (user_ds)         /* input */
		               :"%eax"    /* clobbered register */
		               );
		   //push esp
		   asm volatile("movl %0, %%eax;\n"
		                "pushl %%eax;"
		                :                        /* output */
		                : "r" (regs[15])         /* input */
		                :"%eax"    /* clobbered register */
		                );

		  asm volatile("pushf"); //push the EFLAGS
		  
		  //re-enable interrupt after IRET is called
		  asm volatile ("popl %%eax          \n\
		                 orl  $0x200,%%eax   \n\
		                 pushl %%eax         \n\
		                 " 
		                 :
		                 :
		                 :"%eax"
		                 );

		  asm volatile("pushl %0"
		                :
		                :"c"(user_cs)); //push CS to change to 

		  asm volatile("pushl %0"
		                :
		                :"c"(sa_handler)); //push currnet EIP

		  asm volatile("movl %0, %%ebp"
                     :
                     :"c"(regs[15])
                     );
		  /*IRET*/
		  asm volatile("IRET");

	return;
	}
}
void do_signal_int()
{
	uint32_t cur_pid;
	uint32_t ret_esp;
	uint32_t sig_num;
	uint32_t err_code;
	uint32_t sa_flags;
	uint32_t sa_mask;
	void* sa_handler;
	cli();
	uint32_t siginfo_idx = siginfo_index[scheduling_terminal];
	if(siginfo_idx == -1){
		sti();
		return;
	}
	pcb_struct_t * cur_pcb;
	//find the current pid scheduling_terminal
	cur_pid = current_pid[scheduling_terminal];
	//find the PCB of this process 
	cur_pcb = find_PCB(cur_pid);

    sig_num = cur_pcb->siginfo[siginfo_idx].sig_num;
    err_code = cur_pcb->siginfo[siginfo_idx].sig_err;
    sa_handler = cur_pcb->siginfo[siginfo_idx].sigaction.sa_handler;
    sa_flags = cur_pcb->siginfo[siginfo_idx].sigaction.sa_flags;
    sa_mask = cur_pcb->siginfo[siginfo_idx].sigaction.sa_mask;
    /*check for valid interrupt*/
    if(sig_num < 2 || sig_num == -1)
    {
    	cur_pcb->siginfo[siginfo_idx].sig_num  = -1;
    	sti();
   		return;
	}
	cur_pcb->siginfo[siginfo_idx].sig_num  = -1;
	
	curr_signal = sig_num;
	
    printf("Triggered Signal from interrupt...\n");
    printf("signal numebr = %d \n", sig_num);

    //set up the stack frame 
	//ret_esp = setup_frame_x(sig_num);

	//execute the signal handler 
	sti();
	if(sa_handler == NULL)
	{
		if(sa_flags == 0)
		{
			sig_kill(cur_pid);
		}
		else{
			sig_ignore();
		}
	}
	else 
	{
		// asm volatile("movl %0, %%eax"
  //                    :
  //                    :"c"(sig_num)
  //                    );
		// asm volatile("movl %0, %%eax"
  //                    :
  //                    :"c"(sa_handler)
  //                    );
  //      	asm volatile("call *%eax");
		uint32_t user_ds = USER_DS;
  		uint32_t user_cs = USER_CS;

		       	//push user ss
		   asm volatile("movl %0, %%eax;\n"
		                "pushl %%eax;"
		               :                       /* output */
		               : "r" (user_ds)         /* input */
		               :"%eax"    /* clobbered register */
		               );
		   //push esp
		   asm volatile("movl %0, %%eax;\n"
		                "pushl %%eax;"
		                :                        /* output */
		                : "r" (regs[15])         /* input */
		                :"%eax"    /* clobbered register */
		                );

		  asm volatile("pushf"); //push the EFLAGS
		  
		  //re-enable interrupt after IRET is called
		  asm volatile ("popl %%eax          \n\
		                 orl  $0x200,%%eax   \n\
		                 pushl %%eax         \n\
		                 " 
		                 :
		                 :
		                 :"%eax"
		                 );

		  asm volatile("pushl %0"
		                :
		                :"c"(user_cs)); //push CS to change to 

		  asm volatile("pushl %0"
		                :
		                :"c"(sa_handler)); //push currnet EIP

		  asm volatile("movl %0, %%ebp"
                     :
                     :"c"(regs[15])
                     );
		  /*IRET*/
		  asm volatile("IRET");

	}
	return;
}

uint32_t setup_frame(uint32_t sig_num)
{
	/* push the sigreturn on to the sigreturn */
	uint32_t esp, ret_esp;

	regs[10] = sig_num;
	
	esp = regs[15];

	esp -= 8;
	uint32_t ret_addr = esp;
	memcpy((void*)(esp), (void*)&sf_start, (sig_return_size));               


	/* push the harware context on to the stack */
	esp -= sizeof(hardware_context_t);

	memcpy((void*)(esp), (void*)regs, sizeof(hardware_context_t));

	/* push the sig_num */
	esp -= 4;
    ret_esp = esp;
	memcpy((void*)(esp), (void*)&sig_num, sizeof(uint32_t));


	esp -= 4;
	//uint32_t ret_addr = &sf_start;
	memcpy((void*)(esp), (void*)&ret_addr, sizeof(uint32_t));

	regs[15] = esp;

	return ret_esp;

}

uint32_t setup_frame_x(uint32_t sig_num)
{
	/* push the sigreturn on to the sigreturn */
	uint32_t esp, ret_esp;

	regs_x[10] = sig_num;
	
	esp = regs_x[15];

	esp -= 8;
	memcpy((void*)(esp), (void*)&sf_start, (8));               


	/* push the harware context on to the stack */
	esp -= sizeof(hardware_context_t);

	memcpy((void*)(esp), (void*)regs_x, sizeof(hardware_context_t));

	/* push the sig_num */
	esp -= 4;
    ret_esp = esp;
	memcpy((void*)(esp), (void*)&sig_num, sizeof(uint32_t));


	esp -= 4;
	uint32_t ret_addr = &sf_start;
	memcpy((void*)(esp), (void*)&ret_addr, sizeof(uint32_t));

	regs_x[15] = esp;

	return ret_esp;

}

 void sig_kill(uint32_t pid){//cannot be masked, cannot be blocked
 	syscall_halt(1);
 	return;
 } 

void sig_ignore(){ //does nothing
	return;
}
