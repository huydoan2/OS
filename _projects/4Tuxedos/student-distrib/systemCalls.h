/* systemCalls.h
 * - declaration of the function prototype for all system calls
 */

#ifndef SYSTEMCALLS_H
#define SYSTEMCALLS_H 
#include "types.h"

#define MAX_NUM_PCB 6
#define MAX_TERMINAL 3
#ifndef ASM


int32_t exception_flag;

  /*system call 1: halt function*/
extern int32_t syscall_halt(uint8_t status);

  /*system call 2: execute function*/
extern int32_t syscall_execute(const uint8_t* command);

  /*system call 3: read function*/
extern int32_t syscall_read(int32_t fd, void* buf, int32_t nbytes);

  /*system call 4: write function*/
extern int32_t syscall_write(int32_t fd, const void* buf, int32_t nbytes);

  /*system call 5: open function*/
extern int32_t syscall_open(const uint8_t* filename);

  /*system call 6: close*/
extern int32_t syscall_close(int32_t fd);

  /*system call 7: getargs function*/
extern int32_t syscall_getargs(uint8_t* buf, int32_t nbytes);

  /*system call 8: vidmap function*/
extern int32_t syscall_vidmap(uint8_t** screen_start);

  /*system call 9: set_handler function*/
extern int32_t syscall_set_handler(int32_t signum, void* handler_address);

  /*system call 10: sigreturn function*/
extern int32_t syscall_sigreturn();

#endif /* ASM */
#endif
