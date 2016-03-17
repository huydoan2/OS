/*file.c 
 * - function for initializing file and functions
 */
#include "file.h"
#include "types.h"
#include "lib.h"


#define max_dentries 62

bootblock_t* bootblock;
dentry_t* dentry_file_system;
inode_t* inode_file_system;


int file_open()
{
	return 0;
}

int file_close()
{
	return 0;
}

void file_handler(void)
{
	
}

int file_read(int * buff, int num_bytes)
{
	return 0;
}

int file_write(int * buff, int num_bytes)
{
	return -1;
}

 
int32_t read_dentry_by_name(const uint8_t* fname, struct dentry_t* dentry)
{
	int i;

	for(i = 0; i < max_dentries; i++)
	{

		//if(condition is true)
		// {
		// 	strcpy(dentry->filename, dentry_file_system[index].filename);
		// 	dentry->file_type = dentry_file_system[index].file_type;
		// 	dentry->inode_num = dentry_file_system[index].inode_num;

		// 	return 0;
		// }
	}

	return -1;
}


int32_t read_dentry_by_index(uint32_t index, struct dentry_t* dentry)
{
	//check if index is valid
	if(index > max_dentries)
	{
		return -1;
	}

	//index is valid, copy information from dentry_file_system
	strcpy(dentry->filename, dentry_file_system[index].filename);
	dentry->file_type = dentry_file_system[index].file_type;
	dentry->inode_num = dentry_file_system[index].inode_num;

	return 0;
}

// int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length)
// {

// 	if(inode >= bootblock.num_inodes || offset >= inode_file_system[inode].length_in_B)
// 		return -1;

// 	return 0;
// }
