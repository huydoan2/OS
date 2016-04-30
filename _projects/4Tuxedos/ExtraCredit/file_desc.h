


#ifndef FILE_DESC_H
#define FILE_DESC_H

#include "types.h"


#define FD_SIZE 8
#define NUM_OP 4
#define OPEN 0
#define READ 1
#define WRITE 2
#define CLOSE 3

#ifndef ASM


/*function pointer type for system calls*/
typedef int32_t (*openFunc_ptr)();
typedef int32_t (*readFunc_ptr)(int32_t*, uint32_t, int32_t, int32_t);
typedef int32_t (*writeFunc_ptr)(int32_t*, int32_t);
typedef int32_t (*closeFunc_ptr)();

/*function operation structure */
typedef struct fop{
	openFunc_ptr open_ptr; 
	readFunc_ptr read_ptr;
	writeFunc_ptr write_ptr;
	closeFunc_ptr close_ptr;
} fop_t;
/*sturcure for one FD*/
typedef struct file_desc{
	fop_t fops;
	uint32_t inode;
	uint32_t file_pos;
	uint32_t flags;
}file_desc_t;

/*initialize the file descriptor array*/
extern void init_FD(file_desc_t* FD);

/*check if the FD array has vacancy*/
int check_avail(file_desc_t* FD);

/*common system calls*/
int32_t open_fd(file_desc_t* FD, const uint8_t* filename);
int32_t read_fd(file_desc_t* FD, int32_t fd, void * buf, int32_t nbytes);
int32_t write_fd(file_desc_t* FD, int32_t fd, const void * buf, int32_t nbytes);
int32_t close_fd(file_desc_t* FD, int32_t fd);


#endif

#endif
