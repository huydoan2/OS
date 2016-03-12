#ifndef PAGING_H
#define PAGING_H

#define MAX_SIZE 1024

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

static pt_entry_t first_pt[MAX_SIZE];
static pd_entry_t page_directory[MAX_SIZE] __attribute__((aligned (4)));
static uint32_t pd_size;

void paging_init();

int32_t fill_pt_entry(page_table_t * pt, uint32_t index, uint32_t val);

int32_t fill_pd_entry(index, uint32_t val);








#endif

