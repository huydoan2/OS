 
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
        EXP_TABLE_SIZE = 20
        FAIL_VALUE = -1

.extern regs

.section .text

.extern do_signal

common_Xinterrupt:  
        pushl %eax
        leal  regs, %eax
        movl %EBX, EBX(%eax)
        movl %ECX, ECX(%eax)
        movl %EDX, EDX(%eax)
        movl %ESI, ESI(%eax)
        movl %EDI, EDI(%eax)
        movl %EBP, EBP(%eax)
        popl EAX(%eax)
        XORL %ecx, %ecx
        movw %DS, %cx     
        movl %ECX, DS(%eax)
        movw %ES, %CX  
        movl %ECX, ES(%eax)
        movw %FS, %CX
        movl %ECX, FS(%eax)
        movw %CS, %CX
        movl %ECX, CS(%eax)
        pushf
        popl %ECX
        movl %ECX, EFLAGS(%eax)
        movl %ESP, ESP(%eax)
        XORL %ECX, %ECX
        movw %SS, %CX
        movl %ECX, SS(%eax)
        
        movl EAX(%eax), %eax            #Recover EAX
              
        movl jumb_table(,%eax,4), %eax        #push the address of the function to eax
        call *%eax                      #call the appropriate function        

        ret                               #return from interrupt






common_Xinterrupt_default:
        movl $FAIL_VALUE, %eax        
        ret     

.extern EX_0
.extern EX_1
.extern EX_2
.extern EX_3
.extern EX_4
.extern EX_5
.extern EX_6
.extern EX_7
.extern EX_8
.extern EX_9
.extern EX_10
.extern EX_11
.extern EX_12
.extern EX_13
.extern EX_14
.extern EX_16
.extern EX_17
.extern EX_18
.extern EX_19
.extern EX_20
.extern EX_30


jumb_table:
        .long EX_0, EX_1, EX_2, EX_3, EX_4, EX_5, EX_6, EX_7, EX_8, EX_9, EX_10, EX_11, EX_12, EX_13, EX_14, EX_16, EX_17, EX_18, EX_19, EX_20, EX_30

#define EXP_HANDLER(name,number)   \
.GLOBL name                   ;\
name:   PUSHA                 ;\
        MOVL    $number,%EAX  ;\
        CALL    common_Xinterrupt ;\
        POPA                  ;\
        IRET

/* the system call library wrappers */
EXP_HANDLER(Exception_Handler_0,0)
EXP_HANDLER(Exception_Handler_1,1)
EXP_HANDLER(Exception_Handler_2,2)
EXP_HANDLER(Exception_Handler_3,3)
EXP_HANDLER(Exception_Handler_4,4)
EXP_HANDLER(Exception_Handler_5,5)
EXP_HANDLER(Exception_Handler_6,6)
EXP_HANDLER(Exception_Handler_7,7)
EXP_HANDLER(Exception_Handler_8,8)
EXP_HANDLER(Exception_Handler_9,9)
EXP_HANDLER(Exception_Handler_10,10)
EXP_HANDLER(Exception_Handler_11,11)
EXP_HANDLER(Exception_Handler_12,12)
EXP_HANDLER(Exception_Handler_13,13)
EXP_HANDLER(Exception_Handler_14,14)
EXP_HANDLER(Exception_Handler_16,15)
EXP_HANDLER(Exception_Handler_17,16)
EXP_HANDLER(Exception_Handler_18,17)
EXP_HANDLER(Exception_Handler_19,18)
EXP_HANDLER(Exception_Handler_20,19)
EXP_HANDLER(Exception_Handler_30,20)
