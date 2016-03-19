/*file.c 
 * - function for initializing file and functions
 */
#include "file.h"
#include "types.h"
#include "lib.h"



#define max_dentries 63
#define BLOCK_SIZE    4096

 uint8_t * get_block_addr(int32_t block_num);		//get the starting address of the given block number

//data conflict here with header file need to decide
bootblock_t bootblock;
dentry_t directory_entry[max_dentries];
inode_t inode_array[max_dentries];
int test = 0;
uint32_t* inode_startAddr;
uint32_t* datablock_startAddr;
uint32_t* fileSys_startAddr;

void parsing_fileSystem(uint32_t * startAddr){

	int i, j;
	/*starting addresses*/
	fileSys_startAddr = startAddr;	//starting address for the entire system 
	uint32_t* dir_entry_startAddr = fileSys_startAddr + 16; 	//starting address for directory entries
	uint32_t* curr_inode_addr;	//current address for the inode
	/*assign the starting address of the inode array */
	inode_startAddr = curr_inode_addr = fileSys_startAddr + BLOCK_SIZE/4;

	/*fill in the boot block*/
	bootblock.num_dentries = fileSys_startAddr[0];
	bootblock.num_inodes = fileSys_startAddr[1];
	bootblock.num_dblocks = fileSys_startAddr[2];
	/*obtain the starting address of the data block array*/
	datablock_startAddr = inode_startAddr + (BLOCK_SIZE/4) * bootblock.num_inodes;
	//reserved[BOOTBLOCK_RESERVED_SIZE];

	/*parsing all the directory entries*/
	for (i = 0; i < bootblock.num_dentries; i++){

		strcpy((int8_t *)bootblock.directory_entry[i].filename, (int8_t *)dir_entry_startAddr);
		bootblock.directory_entry[i].file_type = dir_entry_startAddr[8];
		bootblock.directory_entry[i].inode_num = dir_entry_startAddr[9];
		dir_entry_startAddr += 16;
		 //printf("i: %d || filename: %s  || file_type: %d || inode_num: %d\n", i, bootblock.directory_entry[i].filename,bootblock.directory_entry[i].file_type,bootblock.directory_entry[i].inode_num);

	}


	/*parsing the inode array*/
	for (i = 0; i < bootblock.num_inodes; i++){
		inode_array[i].length_in_B = curr_inode_addr[0];
		
		for(j = 0 ; j < (inode_array[i].length_in_B/4096)+1; j++){
			inode_array[i].data_block[j] =  curr_inode_addr[j+1];
			test++;
		}
	 	curr_inode_addr += BLOCK_SIZE/4;
	}

	datablock_startAddr = curr_inode_addr;

	/*debug printing, will be removed later */
	//  printf("i: %d, len: %d \n", 0, inode_array[0].length_in_B);
	//  printf("i: %d, len: %d \n", 1, inode_array[13].length_in_B);
	//  printf("i: %d, len: %d \n", 2, inode_array[16].length_in_B);
	//  printf("i: %d, len: %d \n", 3, inode_array[27].length_in_B);
	//  printf("i: %d, len: %d \n", 4, inode_array[25].length_in_B);
	//  printf("i: %d, len: %d \n", 5, inode_array[23].length_in_B);
	//  printf("i: %d, len: %d \n", 6, inode_array[12].length_in_B);
	//  printf("i: %d, len: %d \n", 7, inode_array[9].length_in_B);
	// printf("i: %d, len: %d \n", 8, inode_array[1].length_in_B);
	// printf("i: %d, len: %d \n", 9, inode_array[2].length_in_B);
	// printf("i: %d, len: %d \n", 10, inode_array[7].length_in_B);
	// printf("i: %d, len: %d \n", 11, inode_array[20].length_in_B);
	// printf("i: %d, len: %d \n", 12, inode_array[19].length_in_B);
	// printf("i: %d, len: %d \n", 13, inode_array[0].length_in_B);
	// printf("i: %d, len: %d \n", 14, inode_array[6].length_in_B);
	// printf("i: %d, len: %d \n", 15, inode_array[22].length_in_B);




	// printf("actual data block number: %d \n", test);
	// printf("actual data start addr : %x \n", curr_inode_addr );



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

	for(i = 0; i < bootblock.num_dentries; i++)
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
	if(index > bootblock.num_dentries)
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
	//int32_t i,l;
	//int32_t num_blocks;
	int32_t j , idx = 0;
	uint8_t * read_start_addr;
	int block_offset;
	int block_idx_offset;
	

	//bad inode number
	if(inode >= bootblock.num_inodes)	
		return -1;
	//bad offset
	if(offset > inode_array[inode].length_in_B || offset < 0)
		return -1;

	block_offset = offset/BLOCK_SIZE;
	block_idx_offset = offset % BLOCK_SIZE;

	//if length exceeds the file capacity, read the whole file
	// if(length > inode_array[inode].length_in_B)
	// {
	// 	num_blocks = (inode_array[inode].length_in_B / 4096)+1;
	// 	length = inode_array[inode].length_in_B;
	// 	//remaining_bytes = inode_array[inode].length_in_B; 
	// }
	// else
	// {
	// 	num_blocks = (length / 4096)+1;
	// 	//remaining_bytes = length;
	// }
	
	if(length > inode_array[inode].length_in_B)
	{
		length = inode_array[inode].length_in_B;
	}
	
	while(length > 0)
	{
		read_start_addr = block_idx_offset + get_block_addr(inode_array[inode].data_block[block_offset]);


		if(block_idx_offset != 0){
			if(length > BLOCK_SIZE - block_idx_offset)
			{
				for(j = 0; j < BLOCK_SIZE - block_idx_offset; ++j)
				{
					buf[idx] = read_start_addr[j];
					++idx;
				}
				length -= (BLOCK_SIZE - block_idx_offset);
				block_idx_offset = 0;
				++block_offset;
				continue;
			}
			else
			{
				for(j = 0; j < length; ++j)
				{
					buf[idx] = read_start_addr[j];
					++idx;
				}				
				length = 0;
				break;
			}

		}
		/*determine how many bytes we still have to read*/
		if(length < 4096){
		
		 	for(j = 0; j < length; ++j)
			{
				buf[idx] = (read_start_addr[j]);
				idx++;	
			}
			length = 0;		//finish reading, length is 0
		}
		else{
			for(j = 0; j < 4096; j++)
			{
				buf[idx] = (read_start_addr[j]);
				idx++;
			
			}
			length -= 4096;
		}
		block_offset++;
	
	}
	// 	if(length < 4096){
	// 		 l = (length/4)+1;
	// 	 	for(j = 0; j < l; ++j)
	// 		{
	// 			if(length < 4){
	// 				switch (length){
	// 					case 1:
	// 						buf[idx] = (read_start_addr[j] & 0x00FF);
	// 						idx++;
	// 						break;
	// 					case 2:
	// 						buf[idx] = (read_start_addr[j] & 0x00FF);
	// 						idx++;
	// 						buf[idx] = (read_start_addr[j] & 0xFF00) >> 8;
	// 						idx++;
	// 						break;
	// 					case 3:	
	// 						buf[idx] = (read_start_addr[j] & 0x00FF);
	// 						idx++;
	// 						buf[idx] = (read_start_addr[j] & 0xFF00) >> 8;
	// 						idx++;
	// 						buf[idx] = (read_start_addr[j] & 0x00FF0000) >> 16;
	// 						idx++;
	// 					break;

	// 				}
	// 			}
	// 			else{
	// 			buf[idx] = (read_start_addr[j] & 0x00FF);
	// 			idx++;
	// 			buf[idx] = (read_start_addr[j] & 0xFF00) >> 8;
	// 			idx++;
	// 			buf[idx] = (read_start_addr[j] & 0x00FF0000) >> 16;
	// 			idx++;
	// 			buf[idx] = (read_start_addr[j] & 0xFF000000) >> 24;
	// 			idx++;
	// 			//decrement the length 
	// 			length -= 4;
	// 			}
	// 		}
	// 	}
	// 	else{
	// 		for(j = 0; j < 1024; j++)
	// 		{
	// 			buf[idx] = (read_start_addr[j] & 0x00FF);
	// 			idx++;
	// 			buf[idx] = (read_start_addr[j] & 0xFF00) >> 8;
	// 			idx++;
	// 			buf[idx] = (read_start_addr[j] & 0x00FF0000) >> 16;
	// 			idx++;
	// 			buf[idx] = (read_start_addr[j] & 0xFF000000) >> 24;
	// 			idx++;
	// 		}
	// 		length -= 4096;
	// 	}
	// 	block_offset++;
	
	// }
	
	return idx;

}


uint8_t * get_block_addr(int32_t block_num)
{
	return (uint8_t *)(datablock_startAddr + (BLOCK_SIZE/4) * block_num);
}


