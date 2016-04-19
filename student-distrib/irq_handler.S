
.data
        EAX = 0
        ECX = 4
        EDX = 8
        EBX = 12
        ESP = 16
        EBP = 20
        ESI = 24
        EDI = 28
        EFLAGS = 32
        IRQ_TABLE_SIZE = 16
        FAIL_VALUE = -1

.section .text


common_interrupt:        
        cmpl $IRQ_TABLE_SIZE, %eax
        ja common_interrupt_default
        cmpl $0, %eax
        jb common_interrupt_default                
        movl jumb_table(,%eax,4), %eax        #push the address of the function to eax
        call *%eax			#call the appropriate function        
        ret		                  #return from interrupt

common_interrupt_default:        
        ret     

.extern pit_handler
.extern rtc_handler
.extern keyboard_handler


jumb_table:
        .long pit_handler, keyboard_handler, 0, 0, 0, 0, 0, 0, rtc_handler, 0, 0, 0, 0, 0, 0, 0

.GLOBL process_info
process_info:
        .rept 9
        .long 0
        .endr

.GLOBL pit_irq
pit_irq:
        pushl %eax
        movl %eax, process_info
        leal process_info, %eax
        popl EAX(%eax)
        movl %ecx, ECX(%eax)
        movl %edx, EDX(%eax)
        movl %ebx, EBX(%eax)
        movl %esp, ESP(%eax)
        movl %ebp, EBP(%eax)
        movl %esi, ESI(%eax)
        movl %edi, EDI(%eax)
        pushf
        pop EFLAGS(%eax)              
        #MOVL pit_handler, %eax
        PUSHL process_info
        CALL pit_handler
        popl %eax
        LEAL process_info, %eax
        MOVL ECX(%eax), %ecx
        MOVL EDX(%eax), %edx
        MOVL EBX(%eax), %ebx
        MOVL ESP(%eax), %esp
        MOVL EBP(%eax), %ebp
        MOVL ESI(%eax), %esi
        MOVL EDI(%eax), %edi
        push EFLAGS(%eax)
        popf

        movl process_info, %eax
        IRET

#define IRQ_HANDLER(name,number)   \
.GLOBL name                   ;\
name:   PUSHA	              ;\
	MOVL	$number,%EAX  ;\
	CALL    common_interrupt ;\
	POPA	              ;\
	IRET

/* the system call library wrappers */
/*IRQ_HANDLER(pit_irq,0)*/
IRQ_HANDLER(keyboard_irq,1)
IRQ_HANDLER(rtc_irq,8)

