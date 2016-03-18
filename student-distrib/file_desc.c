#include "file_desc.h"
#define RTC 0
#define DIR	1
#define REGULAR 2



file_decs_t PCB[PCB_SIZE];


void init_PCB(){
	int i;
	for (i = 0; i < PCB_SIZE; ++i){
		PCB[i].file_pos = 0;
	    PCB[i].flags = 0;
	}
}

int check_avil(){
	int i;
	for (i = 0; i < PCB_SIZE; ++i){
		
	    if (PCB[i].flags == 0){
	    	return i;
	    }
	}

	return -1;
}

int32_t open(const uint8_t* filename)
{
	dentry_t dentry;
	file_decs_t fd;
	int PCB_idx;

	/*declare function pointers*/
	systemcallFunc_ptr open_ptr;
	systemcallFunc_ptr close_ptr;
	systemcallFunc_ptr read_ptr;
	systemcallFunc_ptr write_ptr;

	if ((PCB_idx = check_avil()) == -1){
		return -1;
	} 

	if (read_dentry_by_name(filename, &dentry) == -1)
		return -1;


	switch(dentry.file_type)
	{
		case RTC:
			open_ptr = &rtc_open;
			close_ptr = &rtc_close;
			read_ptr = &rtc_read;
			write_ptr = &rtc_write;
			fd.fops[OPEN] = open_ptr;
			fd.fops[CLOSE] = close_ptr;
			fd.fops[READ] = read_ptr;
			fd.fops[WRITE] = write_ptr;
		    break;

	    case DIR:
	        open_ptr = &dir_open;
			close_ptr = &dir_close;
			read_ptr = &dir_read;
			write_ptr = &dir_write;
			fd.fops[OPEN] = open_ptr;
			fd.fops[CLOSE] = close_ptr;
			fd.fops[READ] = read_ptr;
			fd.fops[WRITE] = write_ptr;
			break;
 
	    case REGULAR: 
	        open_ptr = &file_open;
			close_ptr = &file_close;
			read_ptr = &file_read;
			write_ptr = &file_write;
			fd.fops[OPEN] = open_ptr;
			fd.fops[CLOSE] = close_ptr;
			fd.fops[READ] = read_ptr;
			fd.fops[WRITE] = write_ptr;	
			break;

		default: 
			return -1;
			break;

		break;
	}

	fd.inode = dentry.inode_num;
	
	fd.flags = 1;


	PCB[PCB_idx] = fd;


	return PCB_idx;

	
}

int32_t read(int32_t fd, void * buf, int32_t nbytes)
{

	file_decs_t cur_fd = PCB[fd];
	*(int32_t*)buf = cur_fd.inode;
	return (*(cur_fd.fops[READ]))((int32_t*)buf, nbytes);
}

int32_t write(int32_t fd, const void * buf, int32_t nbytes)
{
	file_decs_t cur_fd = PCB[fd];
	
	return (*(cur_fd.fops[WRITE]))((int32_t*)buf, nbytes);

	return 0;

}

int32_t close(int32_t fd)
{

	return 0;

}



