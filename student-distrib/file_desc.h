
#include "types.h"


#define PCB_SIZE 8
#define NUM_OP 4
#define OPEN 0
#define READ 1
#define WRITE 2
#define CLOSE 3

#ifndef ASM
/*function pointer type for system calls*/
typedef int32_t (*openFunc_ptr)();
typedef int32_t (*readFunc_ptr)(int32_t*, uint32_t, int32_t);
typedef int32_t (*writeFunc_ptr)(int32_t*, int32_t);
typedef int32_t (*closeFunc_ptr)();

/*function operation structure */
typedef struct fop{
	openFunc_ptr open_ptr; 
	readFunc_ptr read_ptr;
	writeFunc_ptr write_ptr;
	closeFunc_ptr close_ptr;
} fop_t;
/*sturcure for one PCB*/
typedef struct file_desc{
	fop_t fops;
	uint32_t inode;
	uint32_t file_pos;
	uint32_t flags;
}file_decs_t;

/*initialize the file descriptor array*/
extern void init_PCB();

/*check if the PCB array has vacancy*/
int check_avail();

/*common system calls*/
extern int32_t open_fd(const uint8_t* filename);
extern int32_t read_fd(int32_t fd, void * buf, int32_t nbytes);
extern int32_t write_fd(int32_t fd, const void * buf, int32_t nbytes);
extern int32_t close_fd(int32_t fd);

#endif
