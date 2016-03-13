/* Paging.h
 * - delcaration of page directory and page tables
 * - function to initializing the paging
 * - functions for paging memory manipulation
 * vim:ts=4 noexpandtab
 */
#ifndef PAGING_H
#define PAGING_H

#define MAX_SIZE 1024
#define VIRMEM_START 0x00000000
#define VIRMEM_END   0xFFFFF000

#define PHYSMEM_START 0x00000000

#include "types.h"
#include "lib.h"



/*declare the page directory and the first page table*/
uint32_t page_directory[MAX_SIZE]   __attribute__((aligned( MAX_SIZE*4)));
uint32_t first_page_table[MAX_SIZE] __attribute__((aligned( MAX_SIZE*4)));

/*function that initializes the page dir, page table and enable paging*/
void paging_init();
/*function that fills a singel entry in the page directory*/
void fill_pd_entry(int index, uint32_t val);
/*function that fills a singel entry in the page table*/
void fill_pt_entry(uint32_t * pt, int index, uint32_t val);


/*functions that might be useful in the future for manipulating the paging*/
uint32_t get_physAddr(uint32_t  virtAddr);

//void mapping_virt2Phys_Addr(uint32_t* physAddr, uint32_t* virtAddr, uint32_t val);








#endif

