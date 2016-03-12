#ifndef PAGING_H
#define PAGING_H

#define MAX_SIZE 1024
#define VIRMEM_START 0x00000000
#define VIRMEM_END   0xFFFFF000

#include "types.h"


typedef struct pt_entry{
	union{
		uint32_t val;
		struct{
			uint32_t phys_page_addr : 20;
			uint8_t avail: 3;
			uint8_t global	:1;
			uint8_t zero	:1;
			uint8_t dirty	:1;
			uint8_t access	:1;
			uint8_t cache_disable	:1;
			uint8_t write_through	:1;
			uint8_t privillege	:1;
			uint8_t rw	:1;
			uint8_t present	:1;
			} __attribute__((packed));
		};

}pt_entry_t;

typedef struct pd_entry{
	union{
		uint32_t val;
		struct{
			uint32_t pt_addr : 20;
			uint8_t avail: 3;
			uint8_t global	:1;
			uint8_t size	:1;
			uint8_t zero	:1;
			uint8_t access	:1;
			uint8_t cache_disable	:1;
			uint8_t write_through	:1;
			uint8_t privillege	:1;
			uint8_t rw	:1;
			uint8_t present	:1;
			} __attribute__((packed));
		};
}pd_entry_t;

typedef struct page_table_t{
	pt_entry_t page_table[MAX_SIZE] __attribute__((aligned (4)));
	uint32_t pt_size;	
} page_table_t;

/**/
static uint32_t first_pt[MAX_SIZE];
static pd_entry_t page_directory[MAX_SIZE] __attribute__((aligned (4)));
static uint32_t pd_size;

void paging_init();


void fill_pd_entry(int index, uint32_t val);

void fill_pt_entry(uint32_t * pt, int index, uint32_t val);

//void get_physAddr(uint32_t * virtAddr);


//void mapping_virt2Phys_Addr(uint32_t* physAddr, uint32_t* virtAddr, uint32_t val);








#endif

