#ifndef PCB_H
#define PCH_H

#include "types.h"
#include "file_desc.h"

#define NEV_8KB 0xFFFFE000


typedef struct parent_info{
	uint32_t pid;
	/*uint32_t*/ file_decs_t* fd_array;
} parent_info_t;

typedef struct pcb_struct{
	uint32_t pid;
	uint32_t esp;
	uint32_t ebp;
	uint32_t eip;
	file_decs_t* fd_array;
	parent_info_t parent;
	uint8_t* arg_buf;
}pcb_struct_t;


void init_PCB(pcb_struct_t* pcb, uint32_t pid, uint32_t esp, uint32_t ebp, uint32_t eip, const parent_info_t parent);



#endif
