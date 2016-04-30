#ifndef IRQ_HANDLER_H
#define IRQ_HANDLER_H


#ifndef ASM

#include "types.h"

extern uint32_t keyboard_irq();
extern uint32_t rtc_irq();
extern uint32_t pit_irq();	



#endif /* ASM */
#endif
