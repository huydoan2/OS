/*file.c 
 * - function for initializing file and functions
 */
#include "file.h"
#include "types.h"
#include "lib.h"



#define max_dentries  63
#define BLOCK_SIZE    4096
#define ADDRPERBLOCK  1024
#define SKIPENTRY     16
#define NAME_MAX_LEN  32		//one character left for '\0'

 uint8_t * get_block_addr(int32_t block_num);		//get the starting address of the given block number

/*variables that hold the file system structure*/
bootblock_t bootblock;	//boot block structure
dentry_t directory_entry[max_dentries];	//data entry structure
inode_t inode_array[max_dentries];	//inode array
int test = 0;

/*starting addresses for the file system*/
uint32_t* inode_startAddr;
uint32_t* datablock_startAddr;
uint32_t* fileSys_startAddr;

/* 
 * parsing_fileSystem
 *   DESCRIPTION: parsing a given file system into data structures for further use
 *-----------------------------------------------------------------------------------
 *   INPUTS: - startAddr: the starting address for the file system
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: - store the file system in to data structures 
 */
void parsing_fileSystem(uint32_t * startAddr){

	int i, j;
	/*starting addresses*/
	fileSys_startAddr = startAddr;	//starting address for the entire system 
	uint32_t* dir_entry_startAddr = fileSys_startAddr + 16; 	//starting address for directory entries
	uint32_t* curr_inode_addr;	//current address for the inode
	/*assign the starting address of the inode array */
	inode_startAddr = curr_inode_addr = fileSys_startAddr + ADDRPERBLOCK;

	/*fill in the boot block - statistic information */
	bootblock.num_dentries = fileSys_startAddr[0];	
	bootblock.num_inodes = fileSys_startAddr[1];
	bootblock.num_dblocks = fileSys_startAddr[2];

	/*obtain the starting address of the data block array*/
	datablock_startAddr = inode_startAddr + (ADDRPERBLOCK) * bootblock.num_inodes;

	/*parsing all the directory entries*/
	for (i = 0; i < bootblock.num_dentries; i++){
		//fill in the current directory entry 
		strcpy((int8_t *)bootblock.directory_entry[i].filename, (int8_t *)dir_entry_startAddr);
		bootblock.directory_entry[i].file_type = dir_entry_startAddr[8];
		bootblock.directory_entry[i].inode_num = dir_entry_startAddr[9];
		//go to the next directory entry
		dir_entry_startAddr += SKIPENTRY; 
		 //printf("i: %d || filename: %s  || file_type: %d || inode_num: %d\n", i, bootblock.directory_entry[i].filename,bootblock.directory_entry[i].file_type,bootblock.directory_entry[i].inode_num);

	}


	/*parsing the inode array*/
	for (i = 0; i < bootblock.num_inodes; i++){
		inode_array[i].length_in_B = curr_inode_addr[0];
		//fill in the data block entry (data block numbers)
		for(j = 0 ; j < (inode_array[i].length_in_B/BLOCK_SIZE)+1; j++){
			inode_array[i].data_block[j] =  curr_inode_addr[j+1];
			test++;
		}
		//go to the next inode block 
	 	curr_inode_addr += ADDRPERBLOCK;
	}

	datablock_startAddr = curr_inode_addr;




}

//open file driver
int32_t dir_open(){	
 	return 0;
}

//close file driver
int32_t dir_close(){
	return 0;
}

/* 
 * dir_read
 *   DESCRIPTION: reads a directory file 
 *-----------------------------------------------------------------------------------
 *   INPUTS: - offset: the index for the directory entry
 *           
 *   OUTPUTS: - buff: the filename of the next directory entry
 *   RETURN VALUE: - 1: if a directory entry is read
 *				   - 0: is the reach the end of the directory 
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: - store the file system in to data structures 
 */int32_t index_temp = 0;
int32_t dir_read(int32_t* buff, uint32_t offset, int32_t num_bytes, int32_t var){
	dentry_t dentry;
	

	//check if the input offset is valid
	if(offset >= bootblock.num_dentries)
		return 0;

	cli();
printf("idx: %d|| ", ++index_temp);
	//obtain the directory entry
	read_dentry_by_index(offset, &dentry);
	//obtian the file name of that entry 
	strncpy((int8_t*)buff,dentry.filename, num_bytes);
	printf("==>from read dir: %s || ",buff);
	sti();

	return strlen(dentry.filename);
}

//write function for file
int32_t dir_write(int32_t* buff, int32_t num_bytes){
	return 0;
}


int32_t file_open()
{
	return 0;
}

int32_t file_close()
{
	return 0;
}

void file_handler(void)
{
	
}

/* 
 * file_read
 *   DESCRIPTION: reads a regular file with given inode
 *-----------------------------------------------------------------------------------
 *   INPUTS: - offset: the index for the directory entry
 *           
 *   OUTPUTS: - buff: the data read from the data blocks
 *   RETURN VALUE: - the number of bytes that has been read
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: none 
 */
int32_t file_read(int32_t * buff, uint32_t offset,int32_t num_bytes, int32_t var)
{
	uint32_t inode = var;

	return read_data(inode, offset, (uint8_t*)buff, num_bytes);
	
}

int32_t file_write(int32_t * buff, int32_t num_bytes)
{
	return -1;
}

 
/* 
 * read_dentry_by_name
 *   DESCRIPTION: obtain a directory entry with given entry name
 *-----------------------------------------------------------------------------------
 *   INPUTS: - name: the name of the file 
 *			 
 *   OUTPUTS: - dentry: returned directory entry
 *   RETURN VALUE: -1: failed 
 *					0: successed
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: - store target directory entry into dentry
 */
int32_t read_dentry_by_name(const uint8_t* fname, struct dentry_t* dentry)
{
	int i, len= strlen((int8_t *)fname);

	/*truncate the length to 31 if the length is longer*/
	if(len > NAME_MAX_LEN)
		len = NAME_MAX_LEN;

	/*traverse the directory entries to match with input filename*/
	for(i = 0; i < bootblock.num_dentries; i++)
	{
		if( len != strlen((int8_t *)bootblock.directory_entry[i].filename))
			continue;
		if(strncmp((int8_t *)bootblock.directory_entry[i].filename, (int8_t *)fname, len ) == 0)
		{
			/*if match has been found, then copy the directory entry*/
			strcpy((int8_t *)dentry->filename, (int8_t *)bootblock.directory_entry[i].filename);
			dentry->file_type = bootblock.directory_entry[i].file_type;
			dentry->inode_num = bootblock.directory_entry[i].inode_num;
			return 0;
		}
	}

	return -1;
}

/* 
 * read_dentry_by_index
 *   DESCRIPTION: obtain a directory entry with given entry index
 *-----------------------------------------------------------------------------------
 *   INPUTS: - index: entry for the directory entry array
 			 - dentry: returned directory entry
 *   OUTPUTS: - dentry: dentry that contains the desired directory entry
 *   RETURN VALUE: -1: failed 
 					0: successed
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: - store target directory entry into dentry
 */

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


/* 
 * read_data
 *   DESCRIPTION: read data of given inode within valid range
 *-----------------------------------------------------------------------------------
 *   INPUTS: - inode: location of data to be read
 			 - offset: offset of where to start reading
 			 - buf: stores data being read
 			 -length: length of how many bytes to read
 *   OUTPUTS: - buf: data read from inode
 *   RETURN VALUE: -1: failed 
 					success: number of bytes read
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: - store target directory entry into dentry
 */
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length)
{
	
	int32_t j , idx = 0;	//idx holds the number bytes read
	uint8_t * read_start_addr;	//the starting address of the file system 
	int block_offset;		//data block offset
	int block_idx_offset;   //starting position of the data block
	
	/*check if the inputs are valid*/
	//bad inode number
	if(inode >= bootblock.num_inodes)	
		return 0;
	//bad offset
	if(offset >= inode_array[inode].length_in_B || offset < 0)
		return 0;

	/*calculate block offset adn block entry offset*/
	block_offset = offset/BLOCK_SIZE;
	block_idx_offset = offset % BLOCK_SIZE;


	/*check if the requested length exceed the maximum length */
	if(length > inode_array[inode].length_in_B)
	{
		length = inode_array[inode].length_in_B - offset;
	}
	
	/*read data with the given length*/
	while(length > 0)
	{
		//find the starting address of the data block 
		read_start_addr = block_idx_offset + get_block_addr(inode_array[inode].data_block[block_offset]);

		//handle the offset
		if(block_idx_offset != 0){
			//if the length is larger than the remaining blcok
			if(length > BLOCK_SIZE - block_idx_offset)
			{	//read everything remains in this block
				for(j = 0; j < BLOCK_SIZE - block_idx_offset; ++j)
				{
					buf[idx] = read_start_addr[j];
					++idx;
				}
				length -= (BLOCK_SIZE - block_idx_offset);	//decremen the length
				block_idx_offset = 0;	
				++block_offset;	//increment block index 
				continue;
			}
			else	//if the length is smaller than the size of the remaining block
			{	
				//read data of the size of length
				for(j = 0; j < length; ++j)
				{
					buf[idx] = read_start_addr[j];
					++idx;
				}				
				length = 0;		//clear the length, the read is finished 
				break;
			}

		}
		/*if the length is smaller the size of one data block*/
		if(length < BLOCK_SIZE){
		
		 	for(j = 0; j < length; ++j)	//read the size of the length
			{
				buf[idx] = (read_start_addr[j]);
				idx++;	
			}
			length = 0;		//finish reading, length is 0
		}
		else{ //if the length is larger thanthe lenght of a block
			for(j = 0; j < BLOCK_SIZE; j++)
			{
				buf[idx] = (read_start_addr[j]);
				idx++;
			
			}
			length -= BLOCK_SIZE;	//decrement the length 
		}
		block_offset++;	//increment the index for the data block array 
	
	}
	
	
	return idx;	//return the number of bytes read 

}

/* 
 * get_block_addr
 *   DESCRIPTION: obtain the starting address of a data block
 *-----------------------------------------------------------------------------------
 *   INPUTS: - block_num: index for the data block array in the inode structure

 *   OUTPUTS: none
 *   RETURN VALUE:the starting address of the data block
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: none
 */
uint8_t * get_block_addr(int32_t block_num)
{
	return (uint8_t *)(datablock_startAddr + (BLOCK_SIZE/4) * block_num);
}

/* 
 * get_fileSize
 *   DESCRIPTION: obtain the size of the file given the file name 
 *-----------------------------------------------------------------------------------
 *   INPUTS: - filename: the name of the file 

 *   OUTPUTS: none
 *   RETURN VALUE:the size of the file with the given filename
 *   - if there is no match, the return value is -1
 *-----------------------------------------------------------------------------------
 *   SIDE EFFECTS: none
 */
 int32_t get_fileSize(const uint8_t* filename){
	dentry_t dentry;
	int32_t inode_num;
	
	/*obtain the directory entry by filename*/
	if (read_dentry_by_name(filename, &dentry) == -1)
		return -1;
	//obtain the inode number
	inode_num = dentry.inode_num;
	return inode_array[inode_num].length_in_B;

}

/*load a program image into the virtual memory*/

void prog_loader(const uint8_t* filename, uint32_t * virtAddr){

	//get the data blocks for the program file 
	dentry_t dentry;
	int32_t inode_num;
	uint8_t* vitr_startAddr = (uint8_t*) virtAddr;
	uint8_t * datablk_startAddr;	
	int32_t file_length = 0;
	uint32_t i = 0;
	uint32_t j;
	uint32_t data_blk_idx = 0;

	read_dentry_by_name(filename, &dentry);
	inode_num = dentry.inode_num;
	file_length = inode_array[inode_num].length_in_B;


	//copy data from the data blocks
    while(file_length > 0){
    	
    	datablk_startAddr = get_block_addr(inode_array[inode_num].data_block[data_blk_idx]);


    	if(file_length < BLOCK_SIZE){
		
		 	for(j = 0; j < file_length; ++j)	//read the size of the length
			{
				vitr_startAddr[i] = (datablk_startAddr[j]);
				i++;	
			}
			file_length = 0;		//finish reading, length is 0
		}
		else{ //if the length is larger thanthe lenght of a block
			for(j = 0; j < BLOCK_SIZE; j++)
			{
				vitr_startAddr[i] = (datablk_startAddr[j]);
				i++;
			
			}
			file_length -= BLOCK_SIZE;	//decrement the length 
		}
       data_blk_idx++;


    }

}

