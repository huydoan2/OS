#include "file_desc.h"
#include "file.h"
#include "rtc.h"

#define RTC 0
#define DIR	1
#define REGULAR 2
#define INUSE  1
#define NOTUSE 0


/*PCB array*/
file_decs_t PCB[PCB_SIZE];


void init_PCB(){
	int i;
	/*initialize file position and flag*/
	for (i = 0; i < PCB_SIZE; ++i){
		PCB[i].file_pos = 0;
	    PCB[i].flags = NOTUSE;
	}
}

int check_avail(){
	int i;
	/*traverse through the PCB size and for the first empty*/
	//start from 2 to leave space for stdin and stdout
	for (i = 2; i < PCB_SIZE; ++i){
		
	    if (PCB[i].flags == NOTUSE){
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
	// systemcallFunc_ptr open_ptr;
	// systemcallFunc_ptr close_ptr;
	// systemcallFunc_ptr read_ptr;
	// systemcallFunc_ptr write_ptr;

	/*check if the PCB array has empty spaces*/
	if ((PCB_idx = check_avail()) == -1){
		return -1;
	} 
	/*obtain the directory entry by filename*/
	if (read_dentry_by_name(filename, &dentry) == -1)
		return -1;

	/*file in the file operation table by the file type*/
	switch(dentry.file_type)
	{
		case RTC:
			fd.fops.open_ptr = &rtc_open;
			fd.fops.close_ptr = &rtc_close;
			fd.fops.read_ptr = &rtc_read;
			fd.fops.write_ptr = &rtc_write;
			// fd.fops[OPEN] = open_ptr;
			// fd.fops[CLOSE] = close_ptr;
			// fd.fops[READ] = read_ptr;
			// fd.fops[WRITE] = write_ptr;
		    break;

	    case DIR:
	        fd.fops.open_ptr = &dir_open;
			fd.fops.close_ptr = &dir_close;
			fd.fops.read_ptr = &dir_read;
			fd.fops.write_ptr = &dir_write;
			// fd.fops[OPEN] = open_ptr;
			// fd.fops[CLOSE] = close_ptr;
			// fd.fops[READ] = read_ptr;
			// fd.fops[WRITE] = write_ptr;
			break;
 
	    case REGULAR: 
	        fd.fops.open_ptr = &file_open;
			fd.fops.close_ptr = &file_close;
			fd.fops.read_ptr = &file_read;
			fd.fops.write_ptr = &file_write;
			// fd.fops[OPEN] = open_ptr;
			// fd.fops[CLOSE] = close_ptr;
			// fd.fops[READ] = read_ptr;
			// fd.fops[WRITE] = write_ptr;	
			break;

		default: 
			return -1;
			break;

		break;
	}

	/*store the inode number*/
	fd.inode = dentry.inode_num;
	fd.file_pos = 0;
	
	//change the flag to indicate the occupied
	fd.flags = INUSE;

	//put the file descriptor to the fd array
	PCB[PCB_idx] = fd;

	//return the index of the fd
	fd.fops.open_ptr();
	return PCB_idx;

	
}

int32_t read(int32_t fd, void * buf, int32_t nbytes)
{
	uint32_t offset = 0;
	int ret_val;
	file_decs_t cur_fd = PCB[fd];
	*(int32_t*)buf = cur_fd.inode;
	offset = cur_fd.file_pos;

	ret_val = cur_fd.fops.read_ptr((int32_t*)buf, offset,nbytes);
	PCB[fd].file_pos += ret_val;

	return ret_val;
}

int32_t write(int32_t fd, const void * buf, int32_t nbytes)
{
	file_decs_t cur_fd = PCB[fd];

	printf("fd from write func: %d \n", fd);
	return cur_fd.fops.write_ptr((int32_t*)buf, nbytes);

	

}

int32_t close(int32_t fd)
{
	file_decs_t cur_fd = PCB[fd];
	PCB[fd].flags = NOTUSE;
	return cur_fd.fops.close_ptr();	
}



