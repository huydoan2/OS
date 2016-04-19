#ifndef IRQ_HANDLER_H
#define IRQ_HANDLER_H


#ifndef ASM

#include "types.h"

extern uint32_t keyboard_irq();
extern uint32_t rtc_irq();
extern uint32_t pit_irq();	

struct process_info{
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;
}process_info_t;

process_info_t active_process_info[3];


#endif /* ASM */
#endif
