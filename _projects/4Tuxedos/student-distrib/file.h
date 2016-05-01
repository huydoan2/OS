/*file.h 
 * - function for initializing file and functions
 */
#ifndef FILE_H
#define FILE_H

#include "types.h"


#define FILENAME_MAX_SIZE	32
#define DENTRY_RESERVED_SIZE	6
#define BOOTBLOCK_RESERVED_SIZE	13
#define DENTRY_MAX_NUM	63
#define DATA_BLOCK_MAX_ENTRY	1023




//open file driver
extern int32_t dir_open();

//close file driver
extern int32_t dir_close();

//read function for file
extern int32_t dir_read(int32_t* buff, uint32_t offset,int32_t num_bytes, int32_t var);

//write function for file
extern int32_t dir_write(int32_t* buff,  int32_t num_bytes);

//handler for file
extern void file_handler(void);

//open file driver
extern int32_t file_open();

//close file driver
extern int32_t file_close();

//read function for file
extern int32_t file_read(int32_t* buff, uint32_t offset, int32_t num_bytes, int32_t var);

//write function for file
extern int32_t file_write(int32_t* buff, int32_t num_bytes);

/*helper function to the size of the file*/
extern int32_t get_fileSize(const uint8_t* filename);

/*structure for each entry in a directory*/
typedef struct dentry_t{
	int8_t filename[FILENAME_MAX_SIZE];
	int32_t file_type;
	int32_t inode_num;
	int32_t reserved[DENTRY_RESERVED_SIZE];
} dentry_t;


/*structure for the bootblock*/
typedef struct bootblock_t{
	int32_t num_dentries;
	int32_t num_inodes;
	int32_t num_dblocks;
	int32_t reserved[BOOTBLOCK_RESERVED_SIZE];
	dentry_t directory_entry[DENTRY_MAX_NUM];
} bootblock_t;

/*structure for an inode*/
typedef struct inode_t{
	int32_t length_in_B;
	int32_t data_block[DATA_BLOCK_MAX_ENTRY];
} inode_t;


/*function that parses a filesystem to data structures*/
void parsing_fileSystem();

/*function that finds the directory entry by its name*/
int32_t read_dentry_by_name(const uint8_t* fname, struct dentry_t* dentry);

/*function that finds the directory entry by the index number*/
int32_t read_dentry_by_index(uint32_t index, struct dentry_t* dentry);

/*function that reads data from the data block*/
int32_t read_data(uint32_t inode, uint32_t offset, uint8_t* buf, uint32_t length);

/*load a program image into the virtual address*/
void prog_loader(const uint8_t* filename, uint32_t * virtAddr);
#endif 

