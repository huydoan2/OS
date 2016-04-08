#include "file_desc.h"
#include "file.h"
#include "rtc.h"
#include "keyboard.h"
#include "PCB.h"

#define RTC 0
#define DIR	1
#define REGULAR 2
#define INUSE  1
#define NOTUSE 0


/*FD array*/
//file_decs_t FD[FD_SIZE];
/* 
 * init_FD
 *   DESCRIPTION: initialize the array of the FDs
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: every FD now has the flag indicates not in use
 *
 */


void init_FD(file_decs_t* FD){
	int i;
	/*initialize file position and flag*/
	for (i = 0; i < FD_SIZE; ++i){
		FD[i].file_pos = 0;
	    FD[i].flags = NOTUSE;
	}
	//initialize the stdin and stdout
	FD[0].fops.open_ptr = &keyboard_open;
	FD[0].fops.close_ptr = &keyboard_close;
	FD[0].fops.read_ptr = &keyboard_read;
	FD[0].fops.write_ptr = &keyboard_write;
	FD[0].inode = -1;
	FD[0].file_pos = 0;
	FD[0].flags = INUSE;

	FD[1].fops.open_ptr = &keyboard_open;
	FD[1].fops.close_ptr = &keyboard_close;
	FD[1].fops.read_ptr = &keyboard_read;
	FD[1].fops.write_ptr = &keyboard_write;
	FD[1].inode = -1;
	FD[1].file_pos = 0;
	FD[1].flags = INUSE;


}

/* 
 * check_avail
 *   DESCRIPTION: find the first availible FD block in the array 
 *-----------------------------------------------------------------------------------
 *   INPUTS: none
 *   OUTPUTS: none
 *   RETURN VALUE: the index of the first availible FD block
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: none
 *
 */
int check_avail(file_decs_t* FD){
	int i;
	/*traverse through the FD size and for the first empty*/
	//start from 2 to leave space for stdin and stdout
	for (i = 2; i < FD_SIZE; ++i){
		
	    if (FD[i].flags == NOTUSE){
	    	return i;
	    }
	}

	return -1;
}


/* 
 * open
 *   DESCRIPTION: create a new FD, call the specific open function
 *-----------------------------------------------------------------------------------
 *   INPUTS: - filename: the name of the file to be opened
 *   OUTPUTS: none
 *   RETURN VALUE: the index of FD block that stores the newly opened fd (fd value)
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: none
 *
 */
int32_t open_fd(const uint8_t* filename)
{

	file_decs_t* FD = find_PCB(current_pid-1)->fd_array;
	dentry_t dentry;
	file_decs_t fd;
	int FD_idx;
	/*check if the FD array has empty spaces*/
	if ((FD_idx = check_avail(FD)) == -1){
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
	FD[FD_idx] = fd;

	//return the index of the fd
	fd.fops.open_ptr();

	return FD_idx;

	
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
int32_t read_fd(int32_t fd, void * buf, int32_t nbytes)
{
	file_decs_t* FD = find_PCB(current_pid-1)->fd_array;
	uint32_t offset = 0;
	int ret_val;
	file_decs_t cur_fd = FD[fd];
	*(int32_t*)buf = cur_fd.inode;
	offset = cur_fd.file_pos;

	ret_val = cur_fd.fops.read_ptr((int32_t*)buf, offset,nbytes);
	FD[fd].file_pos += ret_val;

	return ret_val;
}
/* 
 * write
 *   DESCRIPTION: call the write function associated with the give fd
 *-----------------------------------------------------------------------------------
 *   INPUTS: - fd: index of the FD array
 *			 - nbytes: number of bytes to read from the file
 *			 - buf: extra parameter, may be unused, depending on the type of the file
 *   OUTPUTS: 
 *   RETURN VALUE: 0 for successed and -1 for failed
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: none
 *
 */

int32_t write_fd(int32_t fd, const void * buf, int32_t nbytes)
{
	file_decs_t* FD = find_PCB(current_pid-1)->fd_array;
	file_decs_t cur_fd = FD[fd];

	return cur_fd.fops.write_ptr((int32_t*)buf, nbytes);

	

}

/* 
 * close
 *   DESCRIPTION: close a file, remove the FD from the array (by changing the flag)
 *-----------------------------------------------------------------------------------
 *   INPUTS: - fd: index of the FD array

 *   OUTPUTS: 
 *   RETURN VALUE: 0 for successed and -1 for failed
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: the FD with the index 'fd' is unoccupied now
 *
 */

int32_t close_fd(int32_t fd)
{
	file_decs_t* FD = find_PCB(current_pid-1)->fd_array;
	file_decs_t cur_fd = FD[fd];
	FD[fd].flags = NOTUSE;
	return cur_fd.fops.close_ptr();	
}



