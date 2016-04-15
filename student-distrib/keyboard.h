/* keyboard.h - keyboard initialization, and keyboard dirver
 * and selectors
 * vim:ts=4 noexpandtab
 */
#ifndef _KEYBORAD_H
#define _KEYBORAD_H

#include "i8259.h"
#include "lib.h"
 
//Define values for Keyboard
#define KEYBOARD_DATA 0x60
#define KEYBOARD_COMMAND 0x64
#define size_of_keys 128

#ifndef ASM




 //int lb_index;
 //char line_buffer [size_of_keys];

//initialize the keyboard
 void keyboard_init();

//handles the keyboard input
 void keyboard_handler();

//get the scancode according to the keypress
 char getScancode();

//access the character from the scancode array
 char getchar();

//reset the line buffer
 void reset_linebuffer();

//open keyboard driver
 int32_t keyboard_open();

//close keyboard driver
 int32_t keyboard_close();

//read function for keyboard
 int32_t keyboard_read(int32_t*buff, uint32_t offset, int32_t num_bytes, int32_t var);

//write function for keyboard
 int32_t keyboard_write(int32_t*buff, int32_t num_bytes);


#endif /* ASM */
#endif

