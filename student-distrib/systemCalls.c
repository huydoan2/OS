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
#define PROG_EIP     0x83FFFFC

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
  return 0;
}

/*system call 2: execute function*/
int32_t syscall_execute(const uint8_t* command){
 	/*local variable declaration*/
 	uint32_t parent_pid = current_pid - 1;
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
  uint32_t user_ds = USER_DS|0x3;
  uint32_t user_cs = USER_CS|0x3;
  uint32_t next_eip = PROG_EIP;
  virtAddr = (uint32_t *)0x08048000;
 	dentry_t dentry;

 	/*parse the input string*/
  systcall_exec_parse(command, arg_buf, filename);

 	/*check the file type*/
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
  
  //keyboard_write((int32_t*)virtAddr,187);

  /*Create PCB*/
  parent_info_t parent;
  parent.pid = parent_pid;

  if(current_pid != 0){//if NOT shell
    printf("not shell\n");
  	parent_PCB = find_PCB(parent_pid);
  	parent.fd_array = parent_PCB->fd_array; 
  }

  //get the eip
  read_data(dentry.inode_num, 24, read_buf, 4);
  cur_eip = read_buf[3];
  cur_eip = ((uint32_t)read_buf[2] << 8)  | cur_eip;
  cur_eip = ((uint32_t)read_buf[1] << 16) | cur_eip;
  cur_eip = ((uint32_t)read_buf[0] << 24) | cur_eip;
  printf("cur_eip: %x\n",cur_eip);
      
    
  /*Context switching*/
  //assign values to the parent 
 // if(current_pid != 0){  
    //assign current eip, esp, ebp parent_PCB 
    asm volatile("mov %%esp, %0":"=c"(par_esp));
	  asm volatile("mov %%ebp, %0":"=c"(par_ebp));
  	// parent_PCB->esp = par_esp;
  	// parent_PCB->ebp = par_ebp;
	//}
    // printf("par_esp: %x\n",par_esp);
  // printf("par_ebp: %x\n",par_ebp);

	parent.esp = par_esp;
  parent.ebp = par_ebp;

  add_process(&current_PCB, current_pid, cur_eip, parent);
    
  // current_PCB->esp = par_esp;
  // current_PCB->ebp = par_ebp;
 
  //updating TSS
  tss.ss0 = KERNEL_DS;

  tss.esp0 = EIGHT_MB - 4 - EIGHT_KB*(current_pid-1);

  /*create artificial IRET*/ 

  cli();
   asm volatile("mov %%esp, %0":"=c"(par_esp));
   //push user ds
   asm volatile("movl %0, %%eax;\n"
       "pushl %%eax;"
       :       /* output */
       : "r" (user_ds)         /* input */
       :"%eax"    /* clobbered register */
       );
   //push esp
   asm volatile("movl %0, %%eax;\n"
     "pushl %%eax;"
     :       /* output */
     : "r" (par_esp)         /* input */
     :"%eax"    /* clobbered register */
     );
  
  asm volatile("pushf"); //push the EFLAGS
  
  //re-enable interrupt after IRET is called
  asm volatile ("         \n\
      popl %%eax          \n\
      orl  $0x200,%%eax   \n\
      pushl %%eax         \n\
      " 
      :
      :
      :"%eax"
      );


  asm volatile("pushl %0":"=c"(user_cs)); //push CS to change to 
  asm volatile("pushl %0":"=c"(next_eip)); //push currnet EIP
  /*IRET*/
  asm volatile("IRET");
  printf("reached here\n");
  //halt_ret_label:
  asm volatile("halt_ret_label:");
  // asm volatile("RET");
  return 0;
}

/*system call 3: read function*/
int32_t syscall_read(int32_t fd, void* buf, int32_t nbytes){
	return read_fd(PCB_array[current_pid].fd_array, fd, buf, nbytes);

}

/*system call 4: write function*/
int32_t syscall_write(int32_t fd, const void* buf, int32_t nbytes){
	return write_fd(PCB_array[current_pid].fd_array, fd, buf, nbytes);
}

/*system call 5: open function*/
int32_t syscall_open(const uint8_t* filename){
	return open_fd(PCB_array[current_pid].fd_array, filename);
}

/*system call 6: close*/
int32_t syscall_close(int32_t fd){
	return close_fd(PCB_array[current_pid].fd_array, fd);

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
	*pcb = (pcb_struct_t*)(PCB_array + current_pid);
	init_PCB(*pcb, pid, eip, parent);
	return 0;
}

