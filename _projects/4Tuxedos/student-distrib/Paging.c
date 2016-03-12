#include "Paging.h"

void paging_init()
{
	int i;
	int val;

	for(i = 2; i < MAX_SIZE; ++i)
	{
		page_directory[i] = 0;		
	}


}



int32_t fill_pt_entry(page_table_t * pt, uint32_t index, uint32_t val)
{

}



int32_t fill_pd_entry(index, uint32_t val)
{


}