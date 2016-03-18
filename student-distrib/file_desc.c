#include "file_desc.h"
#define RTC 0
#define DIR	1
#define REGULAR 2

file_decs_t PCB[PCB_SIZE];


int32_t open(const uint8_t* filename)
{
	dentry_t dentry;
	file_decs_t fd;

	if (read_dentry_by_name(filename, &dentry) == -1)
		return -1;

	switch(dentry.file_type)
	{
		case RTC:
			fd.fops[OPEN] = (uint32_t)rtc_open;
			fd.fops[CLOSE] = (uint32_t)rtc_close;
			fd.fops[READ] = (uint32_t)rtc_read;
			fd.fops[WRITE] = (uint32_t)rtc_write;

	    //sscase 
			default: return -1;

		break;
	}
	
}


int32_t read(int32_t fd, void * buf, int32_t nbytes)
{
	return 0;
}

int32_t write(int32_t fd, const void * buf, int32_t nbytes)
{
	return 0;

}

int32_t close(int32_t fd)
{
	return 0;

}