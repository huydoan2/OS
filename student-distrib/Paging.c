#include "Paging.h"
#define  PT_INCREMENT 0x1000
#define  PYHSADDR_INCREMENT 1000
#define  PHYSADDR_MASK 0xFFC00000
#define  PT_ENTRY_INIT_VAL 0x1B
#define  PD_ENTRY_EMP_VAL 0x00000002
#define  PD_ENTRY_INIT_VAL_0 0x00000003
#define  PD_ENTRY_INIT_VAL_1 0x00000083
void paging_init()
{
	int i;
   // uint32_t val = 3; //set present, supervisor, read/write
    uint32_t physAddr = PHYSMEM_START;

	
	//create the empty page dir first page table
	for(i = 0; i < MAX_SIZE; i++)
	{
	  	physAddr += PT_INCREMENT*i;

		page_directory[i] = PD_ENTRY_EMP_VAL;
		first_page_table[i] = (physAddr | PD_ENTRY_INIT_VAL_0);

	}

	//set the first two enties of the PD
	 page_directory[0] = ((unsigned int)first_page_table) | PD_ENTRY_INIT_VAL_0;


	 physAddr += PT_INCREMENT;
	 page_directory[1] = (physAddr | PD_ENTRY_INIT_VAL_1);


	 //load page dir and enable paging

	 asm volatile("                     \n\
		    	push %%ebp		         \n\
				mov %%esp, %%ebp         \n\
				mov 8(%esp), %%eax	     \n\
				mov %%eax, %%cr3	     \n\
				mov %%ebp, %%esp	     \n\
				pop %%ebp			     \n\
				push %%ebp			     \n\
				mov %%esp, %%ebp		 \n\
				mov %%cr0, %%eax		 \n\
				or $0x80000000, %%eax    \n\
				mov %%eax, %%cr0		 \n\
				mov %%ebp, %%esp		 \n\
				pop %%ebp			     \n\
				ret 				     \n\
		    "
			);
	 printf("Paging enabled!\n");

}


void fill_pd_entry(int index, uint32_t val)
{
	page_directory[index] = val;
}

void fill_pt_entry(pt_entry_t * pt, int index, uint32_t val)
{
	pt[index] = val;
}

/*
uint32_t get_physAddr(uint32_t virtAddr){

	unsigned long pdindex = (unsigned long)virtAddr >> 22;
    unsigned long ptindex = (unsigned long)virtAddr >> 12 & 0x03FF;
 
    unsigned long * pd = (unsigned long *)0xFFFFF000;
    // Here you need to check whether the PD entry is present.
 
    unsigned long * pt = ((unsigned long *)0xFFC00000) + (0x400 * pdindex);
    // Here you need to check whether the PT entry is present.
 
    return ((pt[ptindex] & ~0xFFF) + ((unsigned long)virtualaddr & 0xFFF));

}
*/
/*
void mapping_virt2Phys_Addr(uint32_t* physAddr, uint32_t* virtAddr, uint32_t val){

    first_pt[index] = val;
}
*/

