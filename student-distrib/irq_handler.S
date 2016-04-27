 
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
        IRQ_TABLE_SIZE = 16
        FAIL_VALUE = -1


regs:
        .rept 16
        .long 0
        .endr        

.section .text
.globl regs


.extern do_signal
common_interrupt:       
        movl %EBX, EBX(regs)
        movl %ECX, ECX(regs)
        movl %EDX, EDX(regs)
        movl %ESI, ESI(regs)
        movl %EDI, EDI(regs)
        movl %EBP, EBP(regs)
        movl %EAX, EAX(regs)
        XORL %EAX, %EAX
        movw %DS, %AX        
        movl %EAX, DS(regs)
        movw %ES, %AX  
        movl %EAX, ES(regs)
        movw %FS, %AX
        movl %EAX, FS(regs)
        movw %CS, %AX
        movl %EAX, CS(regs)
        pushf
        popf %EAX
        movl %EAX, EFLAGS(regs)
        movl %ESP, ESP(regs)
        XORL %EAX, %EAX
        movw %SS, %AX
        movl %EAX, SS(regs)
        
        movl EAX(regs), %eax            #Recover EAX


        cmpl $IRQ_TABLE_SIZE, %eax
        ja common_interrupt_default
        cmpl $0, %eax
        jb common_interrupt_default                
        movl jumb_table(,%eax,4), %eax        #push the address of the function to eax
        call *%eax			#call the appropriate function     
        # call do_signal                  #send signal   
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

