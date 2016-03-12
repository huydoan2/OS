#ifndef PAGING_H
#define PAGING_H

typedef struct page_table_t{
	pt_entry_t page_table[1024] __attribute__((aligned (4)));
	uint32_t pt_size;	
} page_table_t;


static pd_entry_t page_directory[1024] __attribute__((aligned (4)));
static uint32_t pd_size;

typedef struct pt_entry{
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

}pt_entry_t;

typedef struct pd_entry{
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
}pd_entry_t;

void paging_init();

int32_t fill_pt_entry(page_table_t * pt, uint32_t index);

int32_t fill_pd_entry(index);








#endif

