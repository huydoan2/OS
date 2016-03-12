#include "Paging.h"

void paging_init()
{
	int i,j;
	for(i = 0; i < MAX_SIZE; ++i)
	{
		page_directory[i] = 0;
		for(j = 0; j < MAX_SIZE; ++j)
			page_table[j] = 0;
	}

	
}



int32_t fill_pt_entry(page_table_t * pt, uint32_t index, uint32_t val)
{

}



int32_t fill_pd_entry(index, uint32_t val)
{


}