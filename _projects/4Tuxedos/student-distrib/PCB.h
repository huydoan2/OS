#ifndef PCB_H
#define PCH_H

#include "types.h"
#include "file_desc.h"

#define PCB_OFFSET 0x1FFF


typedef struct {
	uint32_t pid;	
	uint32_t esp;
	uint32_t ebp;
    file_desc_t* fd_array;
} parent_info_t;

typedef struct {
	uint32_t pid;	
	uint32_t eip;
	file_desc_t fd_array[FD_SIZE];
	parent_info_t parent;
	uint8_t* arg_buf;
}pcb_struct_t;


void init_PCB(pcb_struct_t* pcb, uint32_t pid, uint32_t eip, const parent_info_t parent );

pcb_struct_t* find_PCB(uint32_t pid);

#endif
