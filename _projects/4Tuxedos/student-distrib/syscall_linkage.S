
#define ASM 1
#include "../syscalls/ece391sysnum.h"
#include "file_desc.h"
#include "syscall_linkage.h"


.data
	TABLE_SIZE = 10
	FAIL_VALUE = -1
        RETURN_VALUE: .long
.section .text

.globl syscalls     
.type syscalls, @function
syscalls:        
        cmpl $TABLE_SIZE, %eax
        ja syscalls_default
        cmpl $0, %eax
        jbe syscalls_default
        pusha                           #save all registers
        pushl %edx			#push all 3 argument to KERNEL STACK
        pushl %ecx			#for c calling convention
        pushl %ebx
        dec %eax
        movl jumb_table(,%eax,4), %eax        #push the address of the function to eax
        call *%eax			#call the appropriate function
        movl %eax, RETURN_VALUE         #save return value
        /*tear off the KERNEL STACK*/
        popl %ebx
        popl %ecx
        popl %edx        
        popa                            #restore all registers
        movl RETURN_VALUE, %eax           #put return value back to EAX 
        iret		                  #return from interrupt

syscalls_default:
        /* return -1 */
        movl $FAIL_VALUE, %eax
        iret     

jumb_table:
        .long syscall_halt, syscall_execute, syscall_read, syscall_write, syscall_open, syscall_close, syscall_getargs, syscall_vidmap, syscall_set_handler, syscall_sigreturn

#define DO_CALL(name,number)   \
.GLOBL name                   ;\
name:   PUSHL	%EBX          ;\
	MOVL	$number,%EAX  ;\
	MOVL	8(%ESP),%EBX  ;\
	MOVL	12(%ESP),%ECX ;\
	MOVL	16(%ESP),%EDX ;\
	INT	$0x80         ;\
	POPL	%EBX          ;\
	RET

/* the system call library wrappers */
DO_CALL(halt,SYS_HALT)
DO_CALL(execute,SYS_EXECUTE)
DO_CALL(read,SYS_READ)
DO_CALL(write,SYS_WRITE)
DO_CALL(open,SYS_OPEN)
DO_CALL(close,SYS_CLOSE)
DO_CALL(getargs,SYS_GETARGS)
DO_CALL(vidmap,SYS_VIDMAP)
DO_CALL(set_handler,SYS_SET_HANDLER)
DO_CALL(sigreturn,SYS_SIGRETURN)
