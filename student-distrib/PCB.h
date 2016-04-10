/*PCB.h
 * - structures and functions for process control blocks 
 */

#ifndef PCB_H
#define PCH_H

#include "types.h"
#include "file_desc.h"

#define PCB_OFFSET 0x1FFF

/*structure for the parent process information */
typedef struct {
	uint32_t pid;	
	uint32_t esp;
	uint32_t esp0;
	uint32_t ss0;
	uint32_t ebp;
    file_desc_t* fd_array;
} parent_info_t;

/*structure for PCB*/
typedef struct {
	uint32_t pid;	
	uint32_t eip;
	file_desc_t fd_array[FD_SIZE];
	parent_info_t parent;
	uint8_t* arg_buf;
}pcb_struct_t;

/*function that initializes a PCB for the new process */
void init_PCB(pcb_struct_t* pcb, uint32_t pid, uint32_t eip, const parent_info_t parent );
/*function that finds the corresponding PCB given a pid*/
pcb_struct_t* find_PCB(uint32_t pid);

#endif
