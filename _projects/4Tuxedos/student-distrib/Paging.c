#include "Paging.h"
#define  PT_INCREMENT 0x1000
#define  PYHSADDR_INCREMENT 1000
#define  PHYSADDR_MASK 0xFFC00000
#define  PT_ENTRY_INIT_VAL 0x1B
#define  PD_ENTRY_INIT_VAL_0 0x1B
#define  PD_ENTRY_INIT_VAL_1 0x9B
void paging_init()
{
	int i;
	uint32_t val;
	uint32_t virtAddr = VIRMEM_START;
	uint32_t physAddr = 0x00000000;

	
	//set the first page table 
	for(i = 0; i < MAX_SIZE; i++)
	{
		
		virtAddr = PT_INCREMENT*i + VIRMEM_START;

		//get the physical address of current page
		physAddr += PYHSADDR_INCREMENT;
		//set page table flag
        if(i == 0){
        	val  = 0;
        }
        else{
        	val = PT_ENTRY_INIT_VAL | (physAddr & PHYSADDR_MASK);
        }
        //set the page table entry 
		//mapping_virt2Phys_Addr(physAddr, virtAddr, val);
		fill_pt_entry(first_pt, i, val);

	}


  //set the first page directory entry 
	for(i = 0; i < MAX_SIZE; i++){
		if(i == 0){
			val = (PD_ENTRY_INIT_VAL_0);//|(&first_pt[i] & PHYSADDR_MASK);
			fill_pd_entry(i, val);
		}
		else if (i == 1){

			physAddr += PYHSADDR_INCREMENT;

			val = (PD_ENTRY_INIT_VAL_1)|(physAddr & PHYSADDR_MASK);
			fill_pd_entry(i, val);
		}
		else 
			page_directory[i].val = 0;
	}



}


void fill_pd_entry(int index, uint32_t val)
{
	page_directory[index].val = val;
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

