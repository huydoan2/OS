#include "../syscalls/ece391sysnum"
#include "systemCalls.h"

.data
	TABLE_SIZE = 10
	FAIL_VALUE = -1

.text

syscalls:        
        cmpl $TABLE_SIZE, %eax
        ja ioctl_default        
        pushl %edx			#push all 3 argument to KERNEL STACK
        pushl %ecx			#for c calling convention
        pushl %ebx
        leal jumb_table(,%eax,4), %eax        #push the address of the function to eax
        call %eax			#call the appropriate function
        /*tear off the KERNEL STACK*/
        popl %ebx
        popl %ecx
        popl %edx
        iret		#return from interrupt
syscalls_default:
        /* return -1 */
        movl $FAIL_VALUE, %eax
        iret     

jumb_table:
        .long halt, execute, read, write, open,
        		close, getargs, vidmap, set_handler, sigreturn
