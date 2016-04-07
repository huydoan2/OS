/* systemCalls.c
 * - definition of system call functions 
 */
#include "systemCalls.h"
#include "file_desc.h" //file that have sys
#include "file.h"
#include "lib.h"
#include "paging.h"
#include "PCB.h"

#define FILENAME_MAXLEN   32


  /*system call 1: halt function*/
 int32_t syscall_halt(uint8_t status){
 	return 0;
 }

  /*system call 2: execute function*/
 int32_t syscall_execute(const uint8_t* command){
 	/*local variable declaration*/
 	uint32_t idx = 0;
 	uint8_t filename[FILENAME_MAXLEN];
 	uint8_t* arg_buf;
 	uint8_t  read_buf[4];
 	uint8_t  ELF[4];
 	dentry_t dentry;

 	/*parse the input string*/
 	filename = systcall_exec_parse(command, arg_buf);

 	/*check the file type*/
 	EFL[0] = 0x46;
 	EFL[1] = 0x4C;
 	EFL[2] = 0x45;
 	EFL[3] = 0x7F;
    read_dentry_by_name(filename, &dentry);
    read_data(dentry.inode_num, 0, read_buf, 4);
    if(strncmp(ELF, read_buf, 4) != 0){
    	return -1;//not an executable
    } 

    /*Set up paging*/
    map_page(current_pid);
     
    /*Load the progrma file*/
    prog_loader(filename, 0x08048000);

    /*Create PCB*/
    

    /*Context switching*/

    /*IRET*/
 	return 0;

 }

  /*system call 3: read function*/
 int32_t syscall_read(int32_t fd, void* buf, int32_t nbytes){
 	return read_fd(fd, buf, nbytes);

 }

  /*system call 4: write function*/
 int32_t syscall_write(int32_t fd, const void* buf, int32_t nbytes){
 	return write_fd(fd, buf, nbytes);
 }

  /*system call 5: open function*/
 int32_t syscall_open(const uint8_t* filename){
 	return open_fd(filename);
 }

  /*system call 6: close*/
 int32_t syscall_close(int32_t fd){
 	return close_fd(fd);

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
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////
 /*  helper functions for system calls  */

/* systcall_exec_parse: parse input for execute call
 *reutrn the filename and store the command parameters in the buffer
 */
uint8_t* systcall_exec_parse(const uint8_t* command, uint8_t* buf){

	uint32_t idx = 0;
	uint8_t filename[FILENAME_MAXLEN];

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

	return filename;



}

while (*line != '\0') {       /* if not the end of line ....... */ 
          while (*line == ' ' || *line == '\t' || *line == '\n')
               *line++ = '\0';     /* replace white spaces with 0    */
          *argv++ = line;          /* save the argument position     */
          while (*line != '\0' && *line != ' ' && 
                 *line != '\t' && *line != '\n') 
               line++;             /* skip the argument until ...    */
     }
     *argv = '\0';                 /* mark the end of argument list  */
}
