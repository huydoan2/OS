#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "types.h"
#include "PCB.h"

int32_t  pit_handler();
void switch_task(const uint32_t curr_pid, const uint32_t next_pid);
void switch_task_from_pit(const uint32_t curr_pid, const uint32_t next_pid);

#endif

