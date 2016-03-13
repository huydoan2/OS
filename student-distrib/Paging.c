#include "Paging.h"
#define  PT_INCREMENT 0x1000
#define  PHYSADDR_MASK 0xFFC00000
#define  PT_ENTRY_INIT_VAL 0x1B
#define  PD_ENTRY_EMP_VAL 0x00000002
#define  PD_ENTRY_INIT_VAL_0 0x00000103
#define  PD_ENTRY_INIT_VAL_1 0x00000183
#define  TURNON_4MB_PAGE     0x00000010
#define  TURNON_PAGING       0x80000000

void paging_init()
{
	int i;
   // uint32_t val = 3; //set present, supervisor, read/write
    uint32_t physAddr = PHYSMEM_START;

	
	//create the empty page dir first page table
	for(i = 0; i < MAX_SIZE; i++)
	{
	  	physAddr = PT_INCREMENT*i;

		page_directory[i] = PD_ENTRY_EMP_VAL;
		if (i == 0){
			first_page_table[i] = (physAddr | PD_ENTRY_EMP_VAL);
		}
		else
			first_page_table[i] = (physAddr | PD_ENTRY_INIT_VAL_0);

	}

	//set the first two enties of the PD
	 page_directory[0] = ((unsigned int)first_page_table) | PD_ENTRY_INIT_VAL_0;


	 physAddr += PT_INCREMENT;
	 page_directory[1] = ((physAddr )| PD_ENTRY_INIT_VAL_1);

    printf("pd[0]: %x ||pd[1]: %x||pt: %x||pt[1023]: %x",  page_directory[0],page_directory[1],first_page_table, first_page_table[1023]);
	 //load page dir and enable paging

    uint32_t CR0 = 0;
    uint32_t CR3 = 0;
    uint32_t CR4 = 0;
    
    asm volatile("mov %%CR3, %0":"=c"(CR3));
	CR3 = (unsigned int)page_directory;
	asm volatile("mov %0, %%CR3"::"c"(CR3));  

	asm volatile("mov %%CR4, %0":"=c"(CR4));  
	CR4 = CR4 | TURNON_4MB_PAGE;
	asm volatile("mov %0, %%CR4"::"c"(CR4));  

    asm volatile("mov %%CR0, %0":"=c"(CR0));  
	CR0 = CR0 | TURNON_PAGING;
	asm volatile("mov %0, %%CR0"::"c"(CR0)); 

/*
	 asm volatile(
	 		"movl  %0, %%eax            \n"
	 		"movl  %%eax, %%cr3 		\n"
			"movl  %%cr0, %%eax 		\n"
			"orl  $0x80000000, %%eax 	\n"
			"movl %%eax, %%cr0 			\n"
			"movl %%cr4, %%eax 			\n"
			"orl  $0x00000010, %%eax 	\n"
			"movl %%eax, %%cr4 			\n"
		    :
		    : "r" (page_directory)
		    : "eax"
			);
	*/
			
	 printf("Paging enabled!\n");

}


void fill_pd_entry(int index, uint32_t val)
{
	page_directory[index] = val;
}

void fill_pt_entry(uint32_t * pt, int index, uint32_t val)
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

