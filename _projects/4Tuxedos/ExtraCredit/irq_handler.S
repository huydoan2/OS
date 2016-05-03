 
.data
        EBX = 0
        ECX = 4
        EDX = 8
        ESI = 12
        EDI = 16        
        EBP = 20
        EAX = 24
        DS  = 28
        ES  = 32
        FS  = 36
        IRQ_NUM = 40
        ERRNO = 44
        RET_ADDR = 48
        CS = 52
        EFLAGS = 56
        ESP = 60
        SS = 64       
        IRQ_TABLE_SIZE = 17
        FAIL_VALUE = -1

      

.section .text



.extern regs_x
.extern do_signal_int
common_interrupt:       
        pushl %EAX
        leal regs_x, %EAX
        movl %EBX, EBX(%EAX)
        movl %ECX, ECX(%EAX)
        movl %EDX, EDX(%EAX)
        movl %ESI, ESI(%EAX)
        movl %EDI, EDI(%EAX)
        movl %EBP, EBP(%EAX)
        popl EAX(%EAX)
        XORL %ECX, %ECX
        movw %DS, %CX     
        movl %ECX, DS(%EAX)
        movw %ES, %CX  
        movl %ECX, ES(%EAX)
        movw %FS, %CX
        movl %ECX, FS(%EAX)
        movl $0, IRQ_NUM(%EAX)
        movl $0, ERRNO(%EAX)
        movl 8(%ESP), %ECX
        movl %ECX, RET_ADDR(%EAX)
        XORL %ECX, %ECX
        movl 12(%ESP), %ECX
        movl %ECX, CS(%EAX)
        XORL %ECX, %ECX
        movl 16(%ESP), %ECX
        movl %ECX, EFLAGS(%eax)
        XORL %ECX, %ECX
        movl 20(%ESP), %ECX
        movl %ECX, ESP(%eax)
        XORL %ECX, %ECX
        movl 24(%ESP), %ECX
        movl %ECX, SS(%EAX)        
        movl ECX(%EAX), %ECX            #Recover ECX     
        movl EAX(%EAX), %EAX            #Recover EAX

        cmpl $IRQ_TABLE_SIZE, %eax
        ja common_interrupt_default
        cmpl $0, %eax
        jb common_interrupt_default                
        movl jumb_table(,%eax,4), %eax        #push the address of the function to eax
        call *%eax                      #call the appropriate function     
        call do_signal_int

common_interrupt_default:
        movl $FAIL_VALUE, %eax        
        ret     

.extern pit_handler
.extern rtc_handler
.extern keyboard_handler


jumb_table:
        .long pit_handler, keyboard_handler, 0, 0, 0, 0, 0, 0, rtc_handler, 0, 0, 0, 0, 0, 0, 0

#define IRQ_HANDLER(name,number)   \
.GLOBL name                   ;\
name:   PUSHA                 ;\
        MOVL    $number,%EAX  ;\
        CALL    common_interrupt ;\
        POPA                  ;\
        IRET

/* the system call library wrappers */
IRQ_HANDLER(pit_irq,0)
IRQ_HANDLER(keyboard_irq,1)
IRQ_HANDLER(rtc_irq,8)

