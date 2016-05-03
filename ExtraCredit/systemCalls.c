/* systemCalls.c
 * - definition of system call functions 
 */
#include "systemCalls.h"
#include "file.h"
#include "lib.h"
#include "Paging.h"
#include "x86_desc.h"
#include "keyboard.h" // remove later
#include "PCB.h"

#define SHIFT_8   8
#define SHIFT_16   16
#define SHIFT_24   24
#define FILENAME_MAXLEN   32
#define small_buf_size   4
#define elf_size   4
#define eip_size   4
#define eip_offset   24
#define tss_offset   4
#define arg_buf_size   128
#define exception_ret_val  256
#define parsing_index_max   127
#define vidmap_limit   0x400000
#define reenable_int 0x200
#define FOUR_KB       0x1000
#define EIGHT_KB       0x2000
#define EIGHT_MB     0x800000
#define OneTwentyEight_MB 0x8000000
#define OneThirtyTwo_MB 0x8400000
#define PROG_ESP     0x83FFFFC
#define program_img_start_addr     0x08048000
#define ELF_0     0x7F
#define ELF_1     0x45
#define ELF_2     0x4C
#define ELF_3     0x46
//uint32_t current_pid = 0;
uint8_t arg_buf[arg_buf_size]={0}; //buffer for arguments
int32_t buf_length = -1;
/*index for terminal being dsiplayed*/
extern uint32_t current_terminal;
/*index for terminal being processed*/
extern uint32_t scheduling_terminal;
/*number of active process*/
extern uint32_t num_active_process;

extern uint32_t regs[17];

extern uint32_t regs_x[17];

extern uint32_t curr_signal;
//extern uint32_t current_ter;
uint32_t current_pid[MAX_TERMINAL] = {0};
/*function that updates the pid and PCB for next process*/
int32_t add_process(pcb_struct_t** pcb, uint32_t eip, const parent_info_t parent);
/*function that parses filename and arguments*/
void systcall_exec_parse(const uint8_t* command, uint8_t* buf, uint8_t* filename);

/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/


/*system call 1: halt function
 *   DESCRIPTION: halts the process
 *-----------------------------------------------------------------------------------
 *   INPUTS: status: halting status (normal halt, exception halt, abnormal halt)
 *   OUTPUTS: 
 *   RETURN VALUE: 0
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: if user try to exit the first shell on each terminal, it prevents it
 *                and starts a new shell
 *
 */
int32_t syscall_halt(uint8_t status)
{
  cli();
  uint32_t curr_pid = current_pid[scheduling_terminal];
  
  pcb_struct_t* cur_PCB;
  uint32_t par_esp = 0;
  uint32_t par_ebp = 0;
  uint32_t * virtAddr;
  virtAddr = (uint32_t *)program_img_start_addr;


  /*get parent PCB and information of the parent*/

  cur_PCB =find_PCB(curr_pid);
  par_esp = cur_PCB->parent.esp;
  par_ebp = cur_PCB->parent.ebp;
  cur_PCB->active = EMPTY;
  current_pid[scheduling_terminal] = cur_PCB->parent.pid;

  if(cur_PCB->parent.pid == 0)
  {
    //remove the current proccess from the PCB array and start a new shell
    printf("Can't exit the first shell!   ");
    syscall_execute((uint8_t*)"shell");
  }

  num_active_process--;
  /*remap the parent code to the virtual memory*/
  //Set up paging
  curr_pid = cur_PCB->parent.pid;
  map_page(curr_pid);

  /*set tss registers*/
   tss.ss0 = cur_PCB->parent.ss0;
   tss.esp0 = cur_PCB->parent.esp0;

   current_pid[scheduling_terminal] = curr_pid;
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
  sti();
  asm volatile("jmp halt_ret_label;");

  return 0;
}

/*system call 2: execute function
 *   DESCRIPTION: execute a process
 *-----------------------------------------------------------------------------------
 *   INPUTS: command: user typed command. contains a executable name and arguments
 *   OUTPUTS: 
 *   RETURN VALUE: -1 if error
 *                  256 if exception
 *                  1~255 if success depending on process
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: if the number of running process is reached, stops the execution
 *
 */
int32_t syscall_execute(const uint8_t* command)
{
  cli();
  /*local variable declaration*/
  uint32_t parent_pid = current_pid[current_terminal] ;
  pcb_struct_t* current_PCB;
  uint8_t filename[FILENAME_MAXLEN]={0};  
  uint8_t  read_buf[small_buf_size]={0};
  uint8_t  ELF[small_buf_size] = {0};
  uint8_t status = 0;
  uint32_t par_esp = 0;
  uint32_t par_ebp = 0;
  uint32_t cur_eip = 0;
  uint32_t * virtAddr;
  uint32_t user_ds = USER_DS;
  uint32_t user_cs = USER_CS;
  uint32_t user_esp = PROG_ESP;
  virtAddr = (uint32_t *)program_img_start_addr;
  dentry_t dentry;
  int32_t new_pid = 0;

  /*parse the input string*/
  systcall_exec_parse(command, arg_buf, filename);
  
  /*check the file type, ELF*/
  ELF[0] = ELF_0;
  ELF[1] = ELF_1;
  ELF[2] = ELF_2;
  ELF[3] = ELF_3;

  if(read_dentry_by_name(filename, &dentry)==-1){
    return -1;
  }

  read_data(dentry.inode_num, 0, read_buf, elf_size);
  if(strncmp((int8_t*)ELF, (int8_t*)read_buf, elf_size) != 0){
    return -1;//not an executable
  } 
  
  /*Get the eip from the executable file*/
  read_data(dentry.inode_num, eip_offset, read_buf, eip_size);
  cur_eip = ((uint32_t)read_buf[0])  | cur_eip;
  cur_eip = ((uint32_t)read_buf[1] << SHIFT_8)  | cur_eip;
  cur_eip = ((uint32_t)read_buf[2] << SHIFT_16) | cur_eip;
  cur_eip = ((uint32_t)read_buf[3] << SHIFT_24) | cur_eip;

  /*Create PCB*/
  parent_info_t parent;
  parent.pid = parent_pid;
  asm volatile("mov %%esp, %0" :"=c"(par_esp));
  asm volatile("mov %%ebp, %0" :"=c"(par_ebp)); 
  parent.esp = par_esp;
  parent.ebp = par_ebp;
  parent.esp0 = tss.esp0;
  parent.ss0 = tss.ss0;

  //add a new PCB
  new_pid = add_process(&current_PCB, cur_eip, parent);
  if(new_pid == -1)
  {
    printf("exceeds maximum number of processes\n");
    return -1;
  }

  //updating TSS
  tss.ss0 = KERNEL_DS;
  tss.esp0 = EIGHT_MB - EIGHT_KB * (new_pid - 1);
  
  /*Set up paging*/
  map_page(new_pid);
  /*Load the progrma file*/
  prog_loader(filename, virtAddr);

  /*create artificial IRET*/ 
 
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
  
  asm volatile("mov %%bl, %0":"=c"(status));
  if (exception_flag == 1)
  {
    exception_flag = 0;
    return exception_ret_val;
  }
  else
    return (int32_t) status;
}

/*system call 3: read function
 *   DESCRIPTION: systemm read function
 *-----------------------------------------------------------------------------------
 *   INPUTS: fd: file descriptor to read
            buf: buffer to store the read data
            nbytes: number of bytes to read
 *   OUTPUTS: 
 *   RETURN VALUE: number of bytes read
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: 
 *
 */
int32_t syscall_read(int32_t fd, void* buf, int32_t nbytes)
{
  pcb_struct_t * pcb = find_PCB(current_pid[scheduling_terminal]);
  return read_fd(pcb->fd_array, fd, buf, nbytes);
}

/*system call 4: write function
 *   DESCRIPTION: systemm write function
 *-----------------------------------------------------------------------------------
 *   INPUTS:  fd: file descriptor to write
            buf: buffer to store the read write
            nbytes:  number of bytes to write
 *   OUTPUTS: 
 *   RETURN VALUE: number of bytes written
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: 
 *
 */
int32_t syscall_write(int32_t fd, const void* buf, int32_t nbytes)
{
  pcb_struct_t * pcb = find_PCB(current_pid[scheduling_terminal]);
  return write_fd(pcb->fd_array, fd, buf, nbytes);
}

/*system call 5: open function
 *   DESCRIPTION: systemm open function
 *-----------------------------------------------------------------------------------
 *   INPUTS: filename: file name to be opened
 *   OUTPUTS: 
 *   RETURN VALUE: the index of FD block that stores the newly opened fd (fd value)
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: 
 *
 */
int32_t syscall_open(const uint8_t* filename)
{
  pcb_struct_t * pcb = find_PCB(current_pid[scheduling_terminal]);
  return open_fd(pcb->fd_array, filename);
}

/*system call 6: close
 *   DESCRIPTION: systemm close function
 *-----------------------------------------------------------------------------------
 *   INPUTS: fd: file descriptor to close
 *   OUTPUTS: 
 *   RETURN VALUE: 0 for successed and -1 for failed
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: 
 *
 */
int32_t syscall_close(int32_t fd)
{
  pcb_struct_t * pcb = find_PCB(current_pid[scheduling_terminal]);
  return close_fd(pcb->fd_array, fd);

}

/*system call 7: getargs function
 *   DESCRIPTION: 
 *-----------------------------------------------------------------------------------
 *   INPUTS: buf: buffer that contained parsed arguments
            nbytes: number of bytes to get
 *   OUTPUTS: 
 *   RETURN VALUE: 0
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: 
 *
 */
int32_t syscall_getargs(uint8_t* buf, int32_t nbytes)
{
  int32_t length;

  //error checking
  if(buf == NULL || buf_length == -1)
    return -1;

  //get the min(buf_length, nbytes)
  if(nbytes < buf_length)
    length = nbytes;
  else
    length = buf_length;

  //copy to user space
  memcpy(buf, arg_buf, length);

  return 0;

}

/*system call 8: vidmap function
 *   DESCRIPTION: map video memory
 *-----------------------------------------------------------------------------------
 *   INPUTS: screen_start: video memory start address
 *   OUTPUTS: 
 *   RETURN VALUE: -1 on fial 0 on success
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: 
 *
 */
int32_t syscall_vidmap(uint8_t** screen_start)
{
  cli();
  if(screen_start == NULL || screen_start < (uint8_t**)OneTwentyEight_MB || screen_start >= (uint8_t**)OneThirtyTwo_MB)
    return -1;
  vidmap_mapping();
  *screen_start = (uint8_t*)EIGHT_MB;
  sti();
  return 0;
}

/*system call 9: set_handler function
 *   DESCRIPTION: not used
 *-----------------------------------------------------------------------------------
 *   INPUTS: signum: 
 *            handler_address:
 *   OUTPUTS: 
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: 
 *
 */
int32_t syscall_set_handler(int32_t signum, void* handler_address)
{
  //get the pcb of the current process
  pcb_struct_t * cur_pcb = find_PCB(current_pid[scheduling_terminal]);
  if(cur_pcb == NULL)
    return -1;
  switch (signum){
    case 0:
      sigaction_dividedbyzero.sa_handler = handler_address;
      break;
    case 1:
      sigaction_segfault.sa_handler = handler_address;
      break;
    case 2:
      sigaction_interrupt.sa_handler = handler_address;
      break;
    case 3:
      sigaction_alarm.sa_handler = handler_address;
      break;
    case 4:
      sigaction_user1.sa_handler = handler_address;
      break;
  }

  return 0;
}

/*system call 10: sigreturn function
 *   DESCRIPTION: not used
 *-----------------------------------------------------------------------------------
 *   INPUTS: pcb: 
 *            eip:
 *            parent:
 *   OUTPUTS: 
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: 
 *
 */
int32_t syscall_sigreturn()
{
  int esp;
  if(curr_signal < 2)
    esp = regs[15];
  else
    esp = regs_x[15];
  
  /*find the current esp, copy the hardware context on the processor */
   asm volatile("movl %0, %%esp;\n"
               :                       /* output */
               : "c" (esp)         /* input */
               );
  asm volatile("add $8, %esp ;\n");
  asm volatile("popl %ebx;\n");
  asm volatile("popl %ecx;\n");
  asm volatile("popl %edx;\n");
  asm volatile("popl %esi;\n");
  asm volatile("popl %edi;\n");
  asm volatile("popl %ebp;\n");
  asm volatile("popl %eax;\n");
  asm volatile("popl %ds;\n");
  asm volatile("popl %es;\n");
  asm volatile("popl %fs;\n");
  asm volatile("add $8, %esp ;\n");
  asm volatile("IRET");
  return -1;
}

/* 
 * add_process
 *   DESCRIPTION: add process to correct terminal
 *-----------------------------------------------------------------------------------
 *   INPUTS: pcb: pcb structure to be added
 *            eip: current eip
 *            parent: infomation about parent process
 *   OUTPUTS: 
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: 
 *
 */
int32_t add_process(pcb_struct_t** pcb, uint32_t eip, const parent_info_t parent)
{
  int i, flag = 0;
  uint32_t esp = 0;
  uint32_t ebp = 0;
  /*get esp and ebp*/
  asm volatile("movl %%esp, %0"
                   :"=c"(esp)
                   :
                   :"%esp"
                   );
  asm volatile("movl %%ebp, %0" 
                   :"=c"(ebp)
                   :
                   :"%ebp"
                   );
  /*loop through pcbs and find the empty pcb and add a new process*/
  for (i = 1 ; i <= MAX_NUM_PCB; ++i)
  {
    pcb_struct_t* pcb_i = find_PCB(i);
    if (pcb_i->active == EMPTY)
    {
      current_pid[current_terminal] = i;
      init_PCB(pcb_i, i, eip, esp, ebp, parent);  
      num_active_process++;    
      flag = 1;
      break;
    }
  }
  //cannot find any empty PCB, return failure
  if (flag == 0)
    return -1;
 
  return i;
}

/* 
 * systcall_exec_parse
 *   DESCRIPTION: parse input for execute call
 *-----------------------------------------------------------------------------------
 *   INPUTS: command: user typed command to be parsed
 *            buf: parsed argument
 *            filename: parsed filename
 *   OUTPUTS: filename and store the command parameters in the buffer
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: 
 *
 */
void systcall_exec_parse(const uint8_t* command, uint8_t* buf, uint8_t* filename)
{

  uint32_t idx = 0;
  buf_length = -1;    //reset the buffer

  //get the first char of the filename
  while (*command == ' ') {
      command++; 
    }

    /*get the filename*/
    while( *command != '\0' && *command != ' '&& *command != '\n' ){\
      filename[idx] = *command;
      if(idx > FILENAME_MAXLEN)
        return;
      idx++;
      command++;
    }
    //get the first char of the arguments
   while (*command == ' ') {//get the first char 
         command++; 
     }
    /*assign arguments to the buffer*/
    idx = 0;
    while(*command != '\0' && idx < parsing_index_max){
      buf[idx] = *command;
      idx++;
      command++;
    }
    if(idx == parsing_index_max){
      buf_length = -1;
      return;
    }
    if(idx != 0)
    {
      buf[idx] = '\0';
      buf_length = idx + 1;
  }
  return;
}
