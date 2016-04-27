/*signal.c*/

#include "signal.h"

uint32_t do_signal();
{
	//have to do masking

	setup_frame();
	switch(regs->eax)
	{
		case 0x
	}
}

uint32_t setup_frame()
{
	asm volatile
	(
		start:
		movl $10, %eax
		INT x80
		end:
		leal start, variable_start
		leal end, variable_end
	)
	size = variable_end - variable_start;
	memcpy(esp - size, variable_start, size);
}