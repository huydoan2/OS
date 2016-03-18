/*file.c 
 * - function for initializing file and functions
 */
#include "file.h"
#include "types.h"
#include "lib.h"



#define max_dentries 63
#define BLOCK_SIZE    4096

 uint32_t * get_block_addr(int32_t block_num);		//get the starting address of the given block number

//data conflict here with header file need to decide
bootblock_t bootblock;
dentry_t directory_entry[max_dentries];
inode_t inode_array[max_dentries];

uint32_t* inode_startAddr;
uint32_t* datablock_startAddr;
uint32_t* fileSys_startAddr;

void parsing_fileSystem(uint32_t * startAddr){

	int i, j;
	/*starting addresses*/
	fileSys_startAddr = startAddr;
	uint32_t* dir_entry_startAddr = fileSys_startAddr + 16;
	uint32_t* curr_inode_addr;

	

	inode_startAddr = curr_inode_addr = fileSys_startAddr + (BLOCK_SIZE/4);

	/*fill in the boot block*/
	bootblock.num_dentries = fileSys_startAddr[0];
	bootblock.num_inodes = fileSys_startAddr[1];
	bootblock.num_dblocks = fileSys_startAddr[2];

	datablock_startAddr = inode_startAddr + (BLOCK_SIZE/4) * bootblock.num_inodes;
	//reserved[BOOTBLOCK_RESERVED_SIZE];

	for (i = 0; i < bootblock.num_dentries; i++){

		strcpy((int8_t *)bootblock.directory_entry[i].filename, (int8_t *)dir_entry_startAddr);
		bootblock.directory_entry[i].file_type = dir_entry_startAddr[8];
		bootblock.directory_entry[i].inode_num = dir_entry_startAddr[9];
		dir_entry_startAddr += 16;
	//ved[DENTRY_RESERVED_SIZE];

	}

	/*fill in the inode array*/
	for (i = 0; i < bootblock.num_inodes; i++){
		inode_array[i].length_in_B = curr_inode_addr[0];
		for(j = 0 ; j < (inode_array[i].length_in_B/1024)+1; ++j){
			inode_array[i].data_block[j] =  curr_inode_addr[j+1];
		}
	 	curr_inode_addr += BLOCK_SIZE/4;
	}
	

}

//open file driver
int32_t dir_open(int32_t* buff, int32_t num_bytes){
 	return 0;
}

//close file driver
int32_t dir_close(int32_t* buff, int32_t num_bytes){
	return 0;
}

//read function for file
int32_t dir_read(int32_t* buff, int32_t num_bytes){

	return 0;
}

//write function for file
int32_t dir_write(int32_t* buff, int32_t num_bytes){
	return 0;
}


int32_t file_open(int32_t* buff, int32_t num_bytes)
{
	return 0;
}

int32_t file_close(int32_t* buff, int32_t num_bytes)
{
	return 0;
}

void file_handler(void)
{
	
}


int32_t file_read(int32_t * buff, int32_t num_bytes)
{
	
	return 0;
}

int32_t file_write(int32_t * buff, int32_t num_bytes)
{
	return -1;
}

 
int32_t read_dentry_by_name(const uint8_t* fname, struct dentry_t* dentry)
{
	int i;

	for(i = 0; i < max_dentries; i++)
	{

		if(strncmp((int8_t *)bootblock.directory_entry[i].filename, (int8_t *)fname, strlen((int8_t *)fname)))
		{
			strcpy((int8_t *)dentry->filename, (int8_t *)bootblock.directory_entry[i].filename);
			dentry->file_type = bootblock.directory_entry[i].file_type;
			dentry->inode_num = bootblock.directory_entry[i].inode_num;

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
	strcpy((int8_t *)dentry->filename, (int8_t *)bootblock.directory_entry[index].filename);
	dentry->file_type = bootblock.directory_entry[index].file_type;
	dentry->inode_num = bootblock.directory_entry[index].inode_num;

	return 0;
}

int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length)
{
	int32_t i,j, idx =0;
	int32_t num_blocks;
	uint32_t * read_start_addr;
	//int32_t read_start_block = 0;			//the starting block to read
	//int32_t read_start_block_offset = 0;	//the offset in the starting block
	//int32_t remaining_bytes;
	

	//bad inode number
	if(inode >= bootblock.num_inodes)	
		return -1;
	//bad offset
	if(offset > (inode_array[inode].length_in_B/ 1024)+1 || offset < 0)
		return -1;

	//read_start_block = offset / (BLOCK_SIZE/4);
	//read_start_block_offset = offset % BLOCK_SIZE;

	//if length exceeds the file capacity, read the whole file
	if(length > inode_array[inode].length_in_B)
	{
		num_blocks = (inode_array[inode].length_in_B / 1024)+1;
		//remaining_bytes = inode_array[inode].length_in_B; 
	}
	else
	{
		num_blocks = (length / 1024)+1;
		//remaining_bytes = length;
	}

	for(i = 0; i < num_blocks; i++)
	{
		read_start_addr = get_block_addr(inode_array[inode].data_block[offset]/4);

		/*determine how many bytes we still have to read*/
		if(length < 1024){
		 	for(j = 0; j < length; j++)
			{
				buf[idx] = read_start_addr[j];
				idx++;
			}
		}
		else{
			for(j = 0; j < 1024; j++)
			{
				buf[idx] = read_start_addr[j];
				idx++;
			}
			length -= 1024;
		}
		offset++;
	}
	/*
	if(remaining_bytes > BLOCK_SIZE - read_start_block_offset)
	{
		memcpy(buf, (void*)read_start_addr, BLOCK_SIZE - read_start_block_offset);
		remaining_bytes -= BLOCK_SIZE;	//get the remaining bytes to read
		read_start_block += 1;			//get to the next block
		read_start_block_offset = 0;	//offset is 0 after reading the whole block chunk
		num_byte_read += BLOCK_SIZE;
	}
	else
	{
		memcpy(buf, (void*)read_start_addr, remaining_bytes);
		remaining_bytes = 0;			//done reading. No bytes left to read			
		num_byte_read += remaining_bytes;
		break; 							//Do we need this since i will be num_blocks - 1 now 
	}
	*/
	return length;
}


uint32_t * get_block_addr(int32_t block_num)
{
	return datablock_startAddr + (BLOCK_SIZE/4) * block_num;
}


