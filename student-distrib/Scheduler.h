#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "types.h"
#include "PCB.h"

void pit_handler();
void switch_task(uint32_t curr_pid,uint32_t next_pid);
#endif

