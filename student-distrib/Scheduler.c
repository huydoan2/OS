#include "Scheduler.h"
#include "lib.h"
#include "i8259.h"
#define pit_irq_num 0

void pit_handler()
{
	send_eoi(pit_irq_num);

	

}
