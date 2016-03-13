/* Paging.c
 * - delcaration of page directory and page tables
 * - function to initializing the paging
 * - functions for paging memory manipulation*/

#include "Paging.h"

#define  PT_INCREMENT 0x1000  		//increment value for the physical address for each entry in the page table
#define  PHYSADDR_MASK_1 0xFFC00000	//mask to get the physical address from a table enry (4KB page)
#define  PHYSADDR_MASK_0 0xFFFFF000
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
/*masks for mapping from the virtual address to the physical address*/
#define PD_IDX_SHIFT 22
#define PT_IDX_SHIFT 12
#define PT_IDX_MASK 0x03FF
#define PHYS_ADDR_OFFSET_MASK_0 0x00000FFF//the mask for 4kB page
#define PHYS_ADDR_OFFSET_MASK_1 0x003FFFFF//the mask for 4MB page
#define PAGE_SIZE_MASK 0x0000080 //mask to extract the page size bit from the pd 


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
 * get_physAddr
 *   DESCRIPTION: get the physical address from the given a virtual address (linear address)
 *-----------------------------------------------------------------------------------
 *   INPUTS: - virtAddr: the virtual (linear) address to be translated
 *   OUTPUTS: none
 *   RETURN VALUE: return the value of the physical address corresponding to the virtual address
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: none
 *
 */
uint32_t get_physAddr(uint32_t virtAddr){

	uint32_t pt_addr = 0;
	uint32_t pt_entry;
	uint32_t phys_addr;


	unsigned long pd_index = (unsigned long)virtAddr >> PD_IDX_SHIFT;
    unsigned long pt_index = (unsigned long)virtAddr >> PT_IDX_SHIFT & PT_IDX_MASK;
    unsigned long phys_offset_0 = (unsigned long)(virtAddr & PHYS_ADDR_OFFSET_MASK_0);
    unsigned long phys_offset_1 = (unsigned long)(virtAddr & PHYS_ADDR_OFFSET_MASK_1);
 
    uint32_t pd_entry = page_directory[pd_index];
    //determine the size of the page 
    uint32_t page_size = pd_entry & PAGE_SIZE_MASK;

    if(page_size == 0){ //4kB page
     	pt_addr = pd_entry & PHYSADDR_MASK_0;
     	pt_entry = ((uint32_t *)pt_addr)[pt_index];

     	phys_addr = pt_entry & PHYSADDR_MASK_0;
     	phys_addr += phys_offset_0;

     	return phys_addr;
    }
    else{ // 4MB page

    	phys_addr = pd_entry & PHYSADDR_MASK_1;
    	phys_addr += phys_offset_1;

    	return phys_addr;

    }
    

}

/*
void mapping_virt2Phys_Addr(uint32_t* physAddr, uint32_t* virtAddr, uint32_t val){

    first_pt[index] = val;
}

*/
