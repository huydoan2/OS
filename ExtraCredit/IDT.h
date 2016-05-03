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
#include "exp_handler.h"
#include "rtc.h"

/*all the exception handlers*/
extern void EX_0();

extern void EX_1();

extern void EX_2();

extern void EX_3();

extern void EX_4();

extern void EX_5();

extern void EX_6();

extern void EX_7();

extern void EX_8();

extern void EX_9();

extern void EX_10();

extern void EX_11();

extern void EX_12();

extern void EX_13();

extern void EX_14();

extern void EX_16();

extern void EX_17();

extern void EX_18();

extern void EX_19();

extern void EX_20();

extern void EX_30();

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
void update_siginfo_exp(uint32_t sig_num, uint32_t err_code );
void update_siginfo_int(uint32_t sig_num, uint32_t err_code );



#endif

