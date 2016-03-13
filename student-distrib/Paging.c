#include "Paging.h"

#define  PT_INCREMENT 0x1000  		//increment value for the physical address for each entry in the page table
#define  PHYSADDR_MASK 0xFFC00000	//mask to get the physical address from a table enry
/*entry values for page directory*/
#define  PD_ENTRY_EMP_VAL 0x00000002
#define  PD_ENTRY_INIT_VAL_0 0x00000103
#define  PD_ENTRY_INIT_VAL_1 0x00000183
/*entry value for page table*/
#define  PT_ENTRY_EMP_VAL 0x00000002
#define  PT_ENTRY_INIT_VAL_0 0x00000103
#define  PT_ENTRY_INIT_VAL_1 0x00000183
/*mask for turn on CR0 and CR4*/
#define  TURNON_PAGING 0x80000000
#define  TURNON_4MB_PAGE 0x00000010


/* 
 * paging_init
 *   DESCRIPTION: function that initializes the page dir, page table and enable paging
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: - page directory and the first page table are allocated 
 *				   - paging is enabled
 *
 */
void paging_init()
{
	int i;	
	//assign the physical address to the starting address
    uint32_t physAddr = PHYSMEM_START;

	
	/*initialize page directory and the first page table*/
	for(i = 0; i < MAX_SIZE; i++)
	{

	  	physAddr = PT_INCREMENT*i;
		page_directory[i] = PD_ENTRY_EMP_VAL;
		if (i == 0){
			first_page_table[i] = (physAddr | PT_ENTRY_EMP_VAL);	//reserve the first entry in pt as NULL
		}
		else
			first_page_table[i] = (physAddr | PT_ENTRY_INIT_VAL_0);

	}


	/*assign the first and second entry of the page directory*/
	//set the first two enties of the PD
	 page_directory[0] = ((unsigned int)first_page_table) | PD_ENTRY_INIT_VAL_0;
	 //increment the physical address to the next segement 
	 physAddr += PT_INCREMENT;
	 page_directory[1] = ((physAddr )| PD_ENTRY_INIT_VAL_1);

	
	/*load page dir and enable paging*/
    uint32_t CR0 = 0;
    uint32_t CR3 = 0;
    uint32_t CR4 = 0;

	//pass the address of page directory     
    asm volatile("mov %%CR3, %0":"=c"(CR3));
	CR3 = (unsigned int)page_directory;
	asm volatile("mov %0, %%CR3"::"c"(CR3));  

	//turn on the 4 MB page
	asm volatile("mov %%CR4, %0":"=c"(CR4));  
	CR4 = CR4 | TURNON_4MB_PAGE;
	asm volatile("mov %0, %%CR4"::"c"(CR4)); 
	//enable paging 
	asm volatile("mov %%CR0, %0":"=c"(CR0));  
	CR0 = CR0 | TURNON_PAGING;
	asm volatile("mov %0, %%CR0"::"c"(CR0)); 


}
/* 
 * fill_pd_entry
 *   DESCRIPTION: fill a single entry in the page directory
 *-----------------------------------------------------------------------------------
 *   INPUTS: - index: the index of the page directory
 * 			 - val: the value of the entry
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: the page directoy entry is set
 *
 */

void fill_pd_entry(int index, uint32_t val)
{
	page_directory[index] = val;
}


/* 
 * fill_pd_entry
 *   DESCRIPTION: fill a single entry in the page table
 *-----------------------------------------------------------------------------------
 *   INPUTS: - index: the index of the page directory
 * 			 - val: the value of the entry
 *           - pt: specify the page table
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: the page table entry is set
 *
 */
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

