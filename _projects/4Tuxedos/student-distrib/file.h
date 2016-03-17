/*file.h 
 * - function for initializing file and functions
 */
#ifndef FILE_H
#define FILE_H

#include "types.h"

//handler for file
extern void file_handler(void);

//open file driver
extern int file_open();

//close file driver
extern int file_close();

//read function for file
extern int file_read(int* buff, int num_bytes);

//write function for file
extern int file_write(int* buff, int num_bytes);

//bootblock starting address
/*
printf("Module %d loaded at address: 0x%#x\n", mod_count, (unsigned int)mod->mod_start);
printf("Module %d ends at address: 0x%#x\n", mod_count, (unsigned int)mod->mod_end);
*/


typedef struct dentry_t{
	int8_t filename[32];
	int32_t file_type;
	int32_t inode_num;
	int32_t reserved[6];
} dentry_t;

typedef struct bootblock_t{
	int32_t num_dentries;
	int32_t num_inodes;
	int32_t num_dblocks;
	int32_t reserved[13];
	//need dir entries here???
	dentry_t directory[62];
} bootblock_t;

typedef struct inode_t{
	int8_t length_in_B;
	int32_t data_block[1023];
} inode_t;

int32_t read_dentry_by_name(const uint8_t* fname, struct dentry_t* dentry);

int32_t read_dentry_by_index(uint32_t index, struct dentry_t* dentry);

int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

#endif 
