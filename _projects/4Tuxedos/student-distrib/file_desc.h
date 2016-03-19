#include "types.h"
#include "file.h"
#include "rtc.h"

#define PCB_SIZE 8
#define NUM_OP 4
#define OPEN 0
#define READ 1
#define WRITE 2
#define CLOSE 3

/*function pointer type for system calls*/
typedef int32_t (*systemcallFunc_ptr)(int32_t*, int32_t);

/*sturcure for one PCB*/
typedef struct file_desc{
	systemcallFunc_ptr fops[NUM_OP];
	uint32_t inode;
	uint32_t file_pos;
	uint32_t flags;
}file_decs_t;

/*initialize the file descriptor array*/
extern void init_PCB();

/*check if the PCB array has vacancy*/
int check_avail();

/*common system calls*/
extern int32_t open(const uint8_t* filename);
extern int32_t read(int32_t fd, void * buf, int32_t nbytes);
extern int32_t write(int32_t fd, const void * buf, int32_t nbytes);
extern int32_t close(int32_t fd);


