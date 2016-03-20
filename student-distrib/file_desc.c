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
/* 
 * init_PCB
 *   DESCRIPTION: initialize the array of the PCBs
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: every PCB now has the flag indicates not in use
 *
 */
void init_PCB(){
	int i;
	/*initialize file position and flag*/
	for (i = 0; i < PCB_SIZE; ++i){
		PCB[i].file_pos = 0;
	    PCB[i].flags = NOTUSE;
	}
}

/* 
 * check_avail
 *   DESCRIPTION: find the first availible PCB block in the array 
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: the index of the first availible PCB block
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: none
 *
 */
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

/* 
 * open
 *   DESCRIPTION: create a new PCB, call the specific open function
 *-----------------------------------------------------------------------------------
 *   INPUTS: - filename: the name of the file to be opened
 *   OUTPUTS: none
 *   RETURN VALUE: the index of PCB block that stores the newly opened fd (fd value)
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: none
 *
 */
int32_t open(const uint8_t* filename)
{
	dentry_t dentry;
	file_decs_t fd;
	int PCB_idx;
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
		    break;

	    case DIR:
	        fd.fops.open_ptr = &dir_open;
			fd.fops.close_ptr = &dir_close;
			fd.fops.read_ptr = &dir_read;
			fd.fops.write_ptr = &dir_write;
			break;
 
	    case REGULAR: 
	        fd.fops.open_ptr = &file_open;
			fd.fops.close_ptr = &file_close;
			fd.fops.read_ptr = &file_read;
			fd.fops.write_ptr = &file_write;
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
/* 
 * open
 *   DESCRIPTION: call the read function associated with the give fd
 *-----------------------------------------------------------------------------------
 *   INPUTS: - fd: index of the PCB array
 *			 - nbytes: number of bytes to read from the file
 *   OUTPUTS: - buf: the buffer that holds the data read from the file
 *   RETURN VALUE: number of bytes read
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: none
 *
 */
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
/* 
 * write
 *   DESCRIPTION: call the write function associated with the give fd
 *-----------------------------------------------------------------------------------
 *   INPUTS: - fd: index of the PCB array
 *			 - nbytes: number of bytes to read from the file
 *			 - buf: extra parameter, may be unused, depending on the type of the file
 *   OUTPUTS: 
 *   RETURN VALUE: 0 for successed and -1 for failed
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: none
 *
 */

int32_t write(int32_t fd, const void * buf, int32_t nbytes)
{
	file_decs_t cur_fd = PCB[fd];

	return cur_fd.fops.write_ptr((int32_t*)buf, nbytes);

	

}

/* 
 * close
 *   DESCRIPTION: close a file, remove the PCB from the array (by changing the flag)
 *-----------------------------------------------------------------------------------
 *   INPUTS: - fd: index of the PCB array

 *   OUTPUTS: 
 *   RETURN VALUE: 0 for successed and -1 for failed
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: the PCB with the index 'fd' is unoccupied now
 *
 */

int32_t close(int32_t fd)
{
	file_decs_t cur_fd = PCB[fd];
	PCB[fd].flags = NOTUSE;
	return cur_fd.fops.close_ptr();	
}



