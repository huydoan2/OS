#ifndef PAGING_H
#define PAGING_H

#define MAX_SIZE 1024
#define VIRMEM_START 0x00000000
#define VIRMEM_END   0xFFFFF000

#define PHYSMEM_START 0x00000000

#include "types.h"




uint32_t page_directory[MAX_SIZE] __attribute__((aligned( MAX_SIZE*4)));
uint32_t first_page_table[MAX_SIZE] __attribute__((aligned( MAX_SIZE*4)));

void paging_init();

void fill_pd_entry(int index, uint32_t val);

void fill_pt_entry(pt_entry_t * pt, int index, uint32_t val);

//void get_physAddr(uint32_t * virtAddr);


//void mapping_virt2Phys_Addr(uint32_t* physAddr, uint32_t* virtAddr, uint32_t val);








#endif

