/*file.h 
 * - function for initializing file and functions
 */
#ifndef FILE_H
#define FILE_H

//handler for file
extern void file_handler(void);

//open file driver
extern int file_open();

//close file driver
extern int file_close();

//read function for file
extern int file_read(int * buff, int num_bytes);

//write function for file
extern int file_write(int * buff, int num_bytes);

#endif 
