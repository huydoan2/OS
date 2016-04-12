/* IDT.h 
 * - Defines and fills the interrupt descriptor tables, 
 * with all the exceptions, interrupt handlers and system calls
 * 
 */
#ifndef IDT_H
#define IDT_H

#include "lib.h"
#include "x86_desc.h"
#include "keyboard.h"
#include "rtc.h"

/*all the exception handlers*/
extern void Exception_Handler_0();

extern void Exception_Handler_1();

extern void Exception_Handler_2();

extern void Exception_Handler_3();

extern void Exception_Handler_4();

extern void Exception_Handler_5();

extern void Exception_Handler_6();

extern void Exception_Handler_7();

extern void Exception_Handler_8();

extern void Exception_Handler_9();

extern void Exception_Handler_10();

extern void Exception_Handler_11();

extern void Exception_Handler_12();

extern void Exception_Handler_13();

extern void Exception_Handler_14();

extern void Exception_Handler_16();

extern void Exception_Handler_17();

extern void Exception_Handler_18();

extern void Exception_Handler_19();

extern void Exception_Handler_20();

extern void Exception_Handler_30();

/*all the system calls handlers*/
/*
extern int32_t halt (uint8_t status);

extern int32_t excecute (const uint8_t* command);

extern int32_t read(int32_t fd, void* buf, int32_t nbytes);

extern int32_t write (int32_t fd, const void * buf, int32_t nbytes);

extern int32_t open (const uint8_t* filename);

extern int32_t close (int32_t fd);

extern int32_t getargs (uint8_t* buf, int32_t nbytes);

extern int32_t vidmap (uint8_t** scren_start);

extern int32_t set_handler (int32_t signum, void* handler_address);

extern int32_t sigreturn (void);*/

extern void system_calls();

/*functions to fill in the IDT*/
extern  void fill_interrupt_descriptor_table(); //fill in the entire table

//fil in one entry
extern void fill_interrupt_descriptor_table_entry(idt_desc_t * idt_enry, uint16_t seg_selector, uint8_t reserved4, 
  										uint32_t reserved3, uint32_t reserved2, uint32_t reserved1,	uint32_t size,
  									    uint32_t reserved0, uint32_t dpl, uint32_t present);


#endif

