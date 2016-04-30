#include "file_desc.h"
#include "file.h"
#include "rtc.h"
#include "keyboard.h"

#define RTC 0
#define DIR	1
#define REGULAR 2
#define FD_2 2
#define FD_7 7
#define INUSE  1
#define NOTUSE 0


/*FD array*/
//file_desc_t FD[FD_SIZE];
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
void init_FD(file_desc_t* FD){
	int i;
	/*init in and out*/
	/*stdin*/
	FD[0].file_pos = 0;
    FD[0].flags = INUSE;
	FD[0].fops.open_ptr = &keyboard_open;
	FD[0].fops.close_ptr = &keyboard_close;
	FD[0].fops.read_ptr = &terminal_read;
	FD[0].fops.write_ptr = &terminal_write;

	/*stdout*/
    FD[1].file_pos = 0;
    FD[1].flags = INUSE;
	FD[1].fops.open_ptr = &keyboard_open;
	FD[1].fops.close_ptr = &keyboard_close;
	FD[1].fops.read_ptr = &terminal_read;
	FD[1].fops.write_ptr = &terminal_write;

	/*initialize the rest of the blocks*/
	for (i = REGULAR; i < FD_SIZE; ++i){
		FD[i].file_pos = 0;
	    FD[i].flags = NOTUSE;
	}

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
int check_avail(file_desc_t* FD){
	int i;
	/*traverse through the FD size and for the first empty*/
	//start from 2 to leave space for stdin and stdout
	for (i = FD_2; i < FD_SIZE; ++i){
		
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
int32_t open_fd(file_desc_t* FD, const uint8_t* filename)
{
	dentry_t dentry;
	file_desc_t fd;
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
int32_t read_fd(file_desc_t* FD, int32_t fd, void * buf, int32_t nbytes)
{
	uint32_t offset = 0;
	int32_t var;
	int ret_val;
	if(fd < 0 || fd > FD_7 || fd == 1)
		return -1;

	/*If retunr -1, syserr passes, but fish does not blink
	  If return 0, syserr fails, but fish blinks*/
	if(	FD[fd].flags == NOTUSE)
		return -1;
	file_desc_t cur_fd = FD[fd];
	var = cur_fd.inode;
	offset = cur_fd.file_pos;

	ret_val = cur_fd.fops.read_ptr((int32_t*)buf, offset,nbytes, var);
	if(FD[fd].fops.read_ptr == &dir_read)
		FD[fd].file_pos += 1;

	else
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

int32_t write_fd(file_desc_t* FD, int32_t fd, const void * buf, int32_t nbytes)
{
	if(fd <= 0 || fd > FD_7)
		return -1;
	if(	FD[fd].flags == NOTUSE)
		return -1;
	file_desc_t cur_fd = FD[fd];
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

int32_t close_fd(file_desc_t* FD, int32_t fd)
{
	if(fd < FD_2 || fd > FD_7)
		return -1;
	if(	FD[fd].flags == NOTUSE)
		return -1;
	file_desc_t cur_fd = FD[fd];
	FD[fd].flags = NOTUSE;
	return cur_fd.fops.close_ptr();	
}



