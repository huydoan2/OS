/*signal.c*/

#include "signal.h"
#include "lib.h"
extern uint32_t current_pid[3];
extern uint32_t scheduling_terminal;
extern uint32_t siginfo_index[3];
extern uint32_t regs[16];
extern int32_t syscall_halt(uint8_t status);
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

	//execute the signal handler 
	if(sa_handler == NULL){
		if(sa_flags == 0){
			sig_kill(cur_pid);
		}
		else{
			sig_ignore();
		}
	}
	else {
		asm volatile("movl %0, %%eax"
                     :
                     :"c"(sa_handler)
                     :"%eax"                     //could be wrong
                     );
       asm volatile("call *%eax");

	}

	return;
}

void setup_frame(uint32_t sig_num)
{
	/* push the sigreturn on to the sigreturn */
	uint32_t size, variable_start, esp;
	uint32_t top_esp;
	uint32_t esp_offset;
	asm volatile (" sf_start:       ;   \n"
					"movl $10, %eax  ;  \n"
                 	"INT $0x80        ;  \n"
                    "sf_end:          ;  \n"
                    );

	asm volatile (
 					"size:  ; \n"
	               ".long sf_end - sf_start - 1 ;  \n"
	               "movl $size, %%ecx ;  \n" 
	               "movl %%ecx, %0     ;  \n" 
	               : "=c"(size)
				    );
	asm volatile ("	leal sf_start, %%ecx; \n"
	               	"movl %%ecx, %0     ;  \n"
					:"=c"(variable_start)
				     );
		asm volatile ("		leal regs, %%ecx ;  \n"
	               	  "	movl 60(%%ecx), %0 ; \n" 
					:"=c"(esp)
				     );

               
	memcpy((void*)(esp - size*4), (void*)variable_start, (size*4));


	/* push the harware context on to the stack */
	memcpy((void*)(esp - size*4 - sizeof(uint32_t)*16), (void*)regs, sizeof(uint32_t)*16 );

	/* push the sig_num */
	esp_offset = (size*4 + sizeof(uint32_t)*16)/4 ; //the number of address lines we have to move up
	top_esp = esp - esp_offset;
	asm volatile(" movl %0, %%esp;  \n "
				:
				:"c"(top_esp)
		);
	asm volatile(" movl %0, %%ecx ; \n"
				 " pushl %%ecx  ; \n"
				 :
				 :"c"(sig_num)
		);
	/*push the return address*/
		asm volatile(" movl %0, %%ecx ; \n"
				 " pushl %%ecx  ; \n"
				 :
				 :"c"(esp)
		);

	
}

 void sig_kill(uint32_t pid){//cannot be masked, cannot be blocked
 	syscall_halt(0);
 	return;
 } 

void sig_ignore(){ //does nothing

	return;

}