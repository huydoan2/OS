/* systemCalls.c
 * - definition of system call functions 
 */
#include "systemCalls.h"
#include "file.h"
#include "lib.h"
#include "Paging.h"
#include "x86_desc.h"
#include "keyboard.h" // remove later

#define FILENAME_MAXLEN   32
#define FOUR_KB       0x1000
#define EIGHT_KB       0x2000
#define EIGHT_MB	   0x800000
#define PROG_ESP     0x83FFFFC

uint32_t current_pid = 0;
int32_t add_process(pcb_struct_t** pcb, uint32_t pid, uint32_t eip, const parent_info_t parent);
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////
 /*  helper functions for system calls  */

/* systcall_exec_parse: parse input for execute call
 *reutrn the filename and store the command parameters in the buffer
 */
void systcall_exec_parse(const uint8_t* command, uint8_t* buf, uint8_t* filename){

  uint32_t idx = 0;

  //get the first char of the filename
  while (*command == ' ') {
      command++; 
    }

    /*get the filename*/
    while( *command != '\0' && *command != ' '&& *command != '\n' ){\
      filename[idx] = *command;
      idx++;
      command++;
    }
    //get the first char of the arguments
   while (*command == ' ') {//get the first char 
         command++; 
     }
    /*assign arguments to the buffer*/
    idx = 0;
    while(*command != '\0'){
      buf[idx] = *command;
      idx++;
      command++;
    }

  return;

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

/*system call 1: halt function*/
int32_t syscall_halt(uint8_t status){
   pcb_struct_t* cur_PCB;
  uint32_t par_esp = 0;
  uint32_t par_ebp = 0;
  uint32_t * virtAddr;
  virtAddr = (uint32_t *)0x08048000;


  /*remap the parent code to the virtual memory*/
  //Set up paging
  --current_pid;
  map_page(current_pid-1);

  /*get parent PCB and information of the parent*/

  cur_PCB =find_PCB(current_pid);
  par_esp = cur_PCB->parent.esp;
  par_ebp = cur_PCB->parent.ebp;

  /*set tss registers*/
   tss.ss0 = KERNEL_DS;
   tss.esp0 = EIGHT_MB - 4 - EIGHT_KB * (current_pid-1);

   /*set esp and ebp to the parent stack*/
   asm volatile("movl %0, %%esp"
                     :
                     :"c"(par_esp)
                     :"%esp"
                     );
   asm volatile("movl %0, %%ebp" 
                     :
                     :"c"(par_ebp)
                     :"%ebp"
                     );


  /*jump back to the execute*/
asm volatile("jmp halt_ret_label;");

  return 0;
}

/*system call 2: execute function*/
int32_t syscall_execute(const uint8_t* command){
 	/*local variable declaration*/
 	uint32_t parent_pid = current_pid ;
 	pcb_struct_t* parent_PCB;
 	pcb_struct_t* current_PCB;
 	uint8_t filename[FILENAME_MAXLEN]={0};
 	uint8_t arg_buf[128]={0};
 	uint8_t  read_buf[4]={0};
 	uint8_t  ELF[4] = {0};
 	uint32_t par_esp = 0;
 	uint32_t par_ebp = 0;
 	uint32_t cur_eip = 0;
  uint32_t * virtAddr;
  uint32_t user_ds = USER_DS;
  uint32_t user_cs = USER_CS;
  uint32_t user_esp = PROG_ESP;
  virtAddr = (uint32_t *)0x08048000;
 	dentry_t dentry;

 	/*parse the input string*/
  systcall_exec_parse(command, arg_buf, filename);

 	/*check the file type, ELF*/
  ELF[0] = 0x7F;
  ELF[1] = 0x45;
  ELF[2] = 0x4C;
  ELF[3] = 0x46;
  read_dentry_by_name(filename, &dentry);
  read_data(dentry.inode_num, 0, read_buf, 4);
  if(strncmp((int8_t*)ELF, (int8_t*)read_buf, 4) != 0){
  	return -1;//not an executable
  } 
  
  /*Set up paging*/
  map_page(current_pid);
  /*Load the progrma file*/
  prog_loader(filename, virtAddr);


  /*Get the eip from the executable file*/
  read_data(dentry.inode_num, 24, read_buf, 4);
  cur_eip = ((uint32_t)read_buf[0] << 0)  | cur_eip;
  cur_eip = ((uint32_t)read_buf[1] << 8)  | cur_eip;
  cur_eip = ((uint32_t)read_buf[2] << 16) | cur_eip;
  cur_eip = ((uint32_t)read_buf[3] << 24) | cur_eip;

  /*Context switching*/
    /*Create PCB*/
  parent_info_t parent;
  parent.pid = parent_pid;
  asm volatile("mov %%esp, %0" :"=c"(par_esp));
  asm volatile("mov %%ebp, %0" :"=c"(par_ebp));
	parent.esp = par_esp;
  parent.ebp = par_ebp;
  //add a new PCB
  add_process(&current_PCB, current_pid, cur_eip, parent);
    

 
  //updating TSS
  tss.ss0 = KERNEL_DS;

  tss.esp0 = EIGHT_MB - 4 - EIGHT_KB * (current_pid-1);
  
  /*create artificial IRET*/ 
  cli();
  asm volatile ("mov $0x2B, %%ax   \n\
                 mov %%ax, %%ds    \n\
                 mov %%ax, %%es    \n\
                 mov %%ax, %%fs    \n\
                 mov %%ax, %%gs    \n\
                 "
                 :
                 :
                 :"%ax"
                );

   asm volatile("movl %%esp, %0":"=c"(par_esp));
   asm volatile("movl %%ebp, %0" :"=c"(par_ebp));
   

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
                : "r" (user_esp)         /* input */
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
                :"c"(cur_eip)); //push currnet EIP



  /*IRET*/
  asm volatile("IRET");
  /*set label for return point */
  asm volatile("halt_ret_label:");


  return 0;
}

/*system call 3: read function*/
int32_t syscall_read(int32_t fd, void* buf, int32_t nbytes){
  pcb_struct_t * pcb = find_PCB(current_pid-1);
	return read_fd(pcb->fd_array, fd, buf, nbytes);

}

/*system call 4: write function*/
int32_t syscall_write(int32_t fd, const void* buf, int32_t nbytes){
  pcb_struct_t * pcb = find_PCB(current_pid-1);
	return write_fd(pcb->fd_array, fd, buf, nbytes);
}

/*system call 5: open function*/
int32_t syscall_open(const uint8_t* filename){
  pcb_struct_t * pcb = find_PCB(current_pid-1);
	return open_fd(pcb->fd_array, filename);
}

/*system call 6: close*/
int32_t syscall_close(int32_t fd){
  pcb_struct_t * pcb = find_PCB(current_pid-1);
	return close_fd(pcb->fd_array, fd);

}

/*system call 7: getargs function*/
int32_t syscall_getargs(uint8_t* buf, int32_t nbytes){
	return 0;

}

/*system call 8: vidmap function*/
int32_t syscall_vidmap(uint8_t** screen_start){
	return 0;

}

/*system call 9: set_handler function*/
int32_t syscall_set_handler(int32_t signum, void* handler_address){
	return 0;

}

/*system call 10: sigreturn function*/
int32_t syscall_sigreturn(){
	return 0;

}

/*function that updates the pid and PCB for next process*/
int32_t add_process(pcb_struct_t** pcb, uint32_t pid, uint32_t eip, const parent_info_t parent)
{
	if(current_pid >= MAX_NUM_PCB - 1)
		return -1;

	++current_pid;
	*pcb = find_PCB(pid);
	init_PCB(*pcb, pid, eip, parent);
	return 0;
}

