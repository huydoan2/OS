/* systemCalls.h
 * - declaration of the function prototype for all system calls
 */
#include "types.h"

  /*system call 1: halt function*/
 extern int32_t halt(uint8_t status);

  /*system call 2: execute function*/
 extern int32_t execute(const uint8_t* command);

  /*system call 3: read function*/
 extern int32_t read(int32_t fd, void* buf, int32_t nbytes);

  /*system call 4: write function*/
 extern int32_t write(int32_t fd, const void* buf, int32_t nbytes);

  /*system call 5: open function*/
 extern int32_t open(const uint8_t* filename);

  /*system call 6: close*/
 extern int32_t close(int32_t fd);

  /*system call 7: getargs function*/
 extern int32_t getargs(uint8_t* buf, int32_t nbytes);

  /*system call 8: vidmap function*/
 extern int32_t vidmap(uint8_t** screen_start);

  /*system call 9: set_handler function*/
 extern int32_t set_handler(int32_t signum, void* handler_address);

  /*system call 10: sigreturn function*/
 extern int32_t sigreturn();
