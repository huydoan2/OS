/*file.c 
 * - function for initializing file and functions
 */
#include "file.h"
#include "types.h"
#include "lib.h"



#define max_dentries 63
#define BLOCK_SIZE    4096
//data conflict here with header file need to decide
bootblock_t * bootblock;
dentry_t directory_entry[max_dentries];
inode_t inode_array[max_dentries];

uint32_t inode_startAddr;
uint32_t datablock_startAddr;
uint32_t fileSys_startAddr;

void parsing_fileSystem(uint32_t * startAddr){

	int i, j, k;
	int32_t dir_entry_starAddr = fileSys_startAddr + 64;
	int32_t curr_inode_addr;

	/*starting addresses*/
	fileSys_startAddr = startAddr;

	inode_startAddr = curr_inode_addr = fileSys_startAddr + BLOCK_SIZE;

/*fill in the boot block*/
	bootblock->num_dentries = fileSys_startAddr[0];
	bootblock->num_inodes = fileSys_startAddr[1];
	bootblock->num_dblocks = fileSys_startAddr[2];

	datablock_startAddr = inode_startAddr + BLOCK_SIZE * bootblock->num_inodes;
	//reserved[BOOTBLOCK_RESERVED_SIZE];

	for (i = 0; i < max_dentries; ++i){

		strcpy(bootblock->directory_entry[i].filename, dir_entry_starAddr);
		bootblock->directory_entry[i].file_type = dir_entry_starAddr[4];
		bootblock->directory_entry[i].inode_num = dir_entry_starAddr[5];
		dir_entry_starAddr += 64;
	//ved[DENTRY_RESERVED_SIZE];

	}

/*fill in the inode array*/
	for (i = 0; i < bootblock->num_inodes, ++i){
		inode_array[i].length_in_B = curr_inode_addr[0];
		for(j = 0 ; j < inode_array[i].length_in_B; ++j){
			inode_array[i].data_block[j] =  curr_inode_addr[j+1];
		}
	 	curr_inode_addr += BLOCK_SIZE;
	}
	

}



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

		if(strcmp(bootblock->directory_entry[i].filename, fname, strlen(fname))
		{
			strcpy(dentry->filename, bootblock->directory_entry[i].filename);
			dentry->file_type = bootblock->directory_entry[i].file_type;
			dentry->inode_num = bootblock->directory_entry[i].inode_num;

			return 0;
		}
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
	strcpy(dentry->filename, bootblock->directory_entry[index].filename);
	dentry->file_type = bootblock->directory_entry[index].file_type;
	dentry->inode_num = bootblock->directory_entry[index].inode_num;

	return 0;
}

// int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length)
// {

// 	if(inode >= bootblock->num_inodes || offset >= inode_file_system[inode].length_in_B)
// 		return -1;

// 	return 0;
// }
