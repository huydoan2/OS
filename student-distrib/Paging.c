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
#define  PD_ENTRY_INIT_VAL_2 0x00000087
#define  PD_ENTRY_INIT_VAL_3 0x00000007
/*entry value for page table*/
#define  PT_ENTRY_EMP_VAL 0x00000002
#define  PT_ENTRY_INIT_VAL_0 0x00000003
#define  PT_ENTRY_INIT_VAL_1 0x00000183
#define  PT_ENTRY_INIT_VAL_2 0x00000007
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

#define FIRST_PROG 0x00800000 //8MB
#define PROG_VIRTADDR 0x08000000
#define FOUR_MB 0x0400000
#define INDEX 184
//uint32_t vid_mem_array[3] = {0x08400000, 0x08401000, 0x08402000};
uint32_t vid_mem_array[3] = {0x000B9000, 0x000BA000, 0x000BB000};
extern int current_terminal;

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
			page_table[i] = (physAddr | PT_ENTRY_EMP_VAL);	//reserve the first entry in pt as NULL
		}
		else
			page_table[i] = (physAddr | PT_ENTRY_INIT_VAL_0);

	}


	/*assign the first and second entry of the page directory*/
	page_directory[0] = ((unsigned int)page_table) | PD_ENTRY_INIT_VAL_0;
	//increment the physical address to the next segement 
	physAddr += PT_INCREMENT;
	page_directory[1] = ((physAddr )| PD_ENTRY_INIT_VAL_1);
	page_directory[2] =  ((unsigned int)vid_page_table) | PD_ENTRY_INIT_VAL_3;

	
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

	/* obtain the page table entry index and page directory entry index */
	unsigned long pd_index = (unsigned long)virtAddr >> PD_IDX_SHIFT;
    unsigned long pt_index = (unsigned long)virtAddr >> PT_IDX_SHIFT & PT_IDX_MASK;
    unsigned long phys_offset_0 = (unsigned long)(virtAddr & PHYS_ADDR_OFFSET_MASK_0);
    unsigned long phys_offset_1 = (unsigned long)(virtAddr & PHYS_ADDR_OFFSET_MASK_1);
 	/*obtain the page direcotry entry */
    uint32_t pd_entry = page_directory[pd_index];
    //determine the size of the page 
    uint32_t page_size = pd_entry & PAGE_SIZE_MASK;

    /*if the page to be created is a small page*/
    if(page_size == 0){ //4kB page
     	pt_addr = pd_entry & PHYSADDR_MASK_0;
     	pt_entry = ((uint32_t *)pt_addr)[pt_index];

     	phys_addr = pt_entry & PHYSADDR_MASK_0;
     	phys_addr += phys_offset_0;

     	return phys_addr;
    }
    /* if the page to be created is a large page*/
    else{ // 4MB page

    	phys_addr = pd_entry & PHYSADDR_MASK_1;
    	phys_addr += phys_offset_1;

    	return phys_addr;

    }
}

/* 
 * process_switch_mem_map
 *   DESCRIPTION: switch the video memory mapping when task switching 
 *-----------------------------------------------------------------------------------
 *   INPUTS: 
 *		- next_pid_terminal: the terminal to switch to 
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS:  video memory changed in the page table
 *
 */
void process_switch_mem_map(uint32_t next_pid_terminal){
	   uint32_t CR3 = 0;    

	/*change the video memory mapping*/
	/*if the next terminal is the terminal is currently on display*/
	if(next_pid_terminal == current_terminal){
		page_table[INDEX] = VIDEO | PT_ENTRY_INIT_VAL_2;
		vid_page_table[0] = (VIDEO|PT_ENTRY_INIT_VAL_2);
	}
	else{
		/* map the video memory based on the next terminal*/
		switch (next_pid_terminal){
			case 0:
				page_table[INDEX] =  VIDEO_0 | PT_ENTRY_INIT_VAL_2;
				vid_page_table[0] = (VIDEO_0|PT_ENTRY_INIT_VAL_2);
				break;
			case 1:
				page_table[INDEX] =  VIDEO_1 | PT_ENTRY_INIT_VAL_2;
				vid_page_table[0] = (VIDEO_1|PT_ENTRY_INIT_VAL_2);
				break;
			case 2:
				page_table[INDEX] =  VIDEO_2 | PT_ENTRY_INIT_VAL_2;
				vid_page_table[0] = (VIDEO_2|PT_ENTRY_INIT_VAL_2);	
				break;
		}
	}
	/* Flush TLB */
	asm volatile("mov %%CR3, %0":"=c"(CR3));
	CR3 = (unsigned int)page_directory;
	asm volatile("mov %0, %%CR3"::"c"(CR3));  
}



/* 
 * map_page
 *   DESCRIPTION: map a program image to the 128MB in virtual address space
 *-----------------------------------------------------------------------------------
 *   INPUTS: 
 *		- pid: the pid of the program the function is trying to map
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: changed page table entry
 *
 */
void map_page(uint32_t pid)
{
	/* get the starting address of the prgoram image*/
	uint32_t prog_startAddr = FIRST_PROG + FOUR_MB* (pid -1);
	/* map the program image to 128 MB */
	mapping_virt2Phys_Addr(prog_startAddr, PROG_VIRTADDR);
}


/* 
 * mapping_virt2Phys_Addr
 *   DESCRIPTION: mapping a given phsical address to a given virtual address
 *-----------------------------------------------------------------------------------
 *   INPUTS: 
 *		- physAddr: the physical address 
 *  	- virtAddr: the virtual address
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: page table entry changed
 *
 */
void mapping_virt2Phys_Addr(uint32_t physAddr, uint32_t virtAddr)
{
    unsigned long pdindex = (unsigned long)virtAddr >> PD_IDX_SHIFT;
    uint32_t CR3 = 0;    

    // Create a large page 
     page_directory[pdindex] = (physAddr | PD_ENTRY_INIT_VAL_2);

    // flush TLB
    asm volatile("mov %%CR3, %0":"=c"(CR3));
	CR3 = (unsigned int)page_directory;
	asm volatile("mov %0, %%CR3"::"c"(CR3));  	
}


/* 
 * vidmap_mapping
 *   DESCRIPTION: helper function for the vid_map system call, given user program 
 *                access to change the video memory 
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: page table entry changed
 *
 */
void vidmap_mapping()
{
    uint32_t CR3 = 0;
    uint32_t video_addr = VIDEO;
    // Create a vidmap mapping
	vid_page_table[0] = ((video_addr)|PT_ENTRY_INIT_VAL_2);
    // flush the TLB 
    asm volatile("mov %%CR3, %0":"=c"(CR3));
	CR3 = (unsigned int)page_directory;
	asm volatile("mov %0, %%CR3"::"c"(CR3));
}


/* 
 * set_vid_mem
 *   DESCRIPTION: function that changes the display when switching terminal
 *-----------------------------------------------------------------------------------
 *   INPUTS: 
 *		- cur_terminal_id: terminal is currently on dispaly 
 *      - next_terminal_id: the destination terminal to switch to 
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: 
 *
 */
void set_vid_mem(uint32_t cur_terminal_id, uint32_t next_terminal_id){
		/* map the virtual video memory to the physical video memory */
		int32_t CR3 = 0;   
		page_table[INDEX] = VIDEO | PT_ENTRY_INIT_VAL_2;
		asm volatile("mov %%CR3, %0":"=c"(CR3));
		CR3 = (unsigned int)page_directory;
		asm volatile("mov %0, %%CR3"::"c"(CR3));  

		/* save the current display to the background video memory*/
		memcpy((void*)vid_mem_array[cur_terminal_id], (void*)VIDEO, RESOLUTION*2);
		/* resotre the display to the next terminal's video memory*/
		memcpy((void*)VIDEO,(void*)vid_mem_array[next_terminal_id], RESOLUTION*2); 
}



