/* systemCalls.c
 * - definition of system call functions 
 */
#include "systemCalls.h"
#include "file_desc.h" //file that have sys


  /*system call 1: halt function*/
 int32_t halt(uint8_t status){
 	return 0;
 }

  /*system call 2: execute function*/
 int32_t execute(const uint8_t* command){
 	return 0;

 }

  /*system call 3: read function*/
 int32_t read(int32_t fd, void* buf, int32_t nbytes){
 	return read_fd(fd, buf, nbytes);

 }

  /*system call 4: write function*/
 int32_t write(int32_t fd, const void* buf, int32_t nbytes){
 	return write_fd(fd, buf, nbytes);
 }

  /*system call 5: open function*/
 int32_t open(const uint8_t* filename){
 	return open_fd(filename);
 }

  /*system call 6: close*/
 int32_t close(int32_t fd){
 	return close_fd(fd);

 }

  /*system call 7: getargs function*/
 int32_t getargs(uint8_t* buf, int32_t nbytes){
 	return 0;

 }

  /*system call 8: vidmap function*/
 int32_t vidmap(uint8_t** screen_start){
 	return 0;

 }

  /*system call 9: set_handler function*/
 int32_t set_handler(int32_t signum, void* handler_address){
 	return 0;

 }

  /*system call 10: sigreturn function*/
 int32_t sigreturn(){
 	return 0;

 }
