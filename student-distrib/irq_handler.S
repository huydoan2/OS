
.data
        ESP = 0
        EBP = 4
       
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

#define IRQ_HANDLER(name,number)   \
.GLOBL name                   ;\
name:   PUSHA	              ;\
	MOVL	$number,%EAX  ;\
	CALL    common_interrupt ;\
	POPA	              ;\
	IRET

/* the system call library wrappers */
IRQ_HANDLER(pit_irq,0)
IRQ_HANDLER(keyboard_irq,1)
IRQ_HANDLER(rtc_irq,8)

