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


extern int lb_index;
extern char line_buffer [size_of_keys];


//initialize the keyboard
extern void keyboard_init();

//handles the keyboard input
extern void keyboard_handler();

//get the scancode according to the keypress
extern char getScancode();

//access the character from the scancode array
extern char getchar();

//reset the line buffer
extern void reset_linebuffer();

//open keyboard driver
extern void keyboard_open();

//close keyboard driver
extern int keyboard_close();

//read function for keyboard
extern int keyboard_read(char * buff, int num_bytes);

//write function for keyboard
extern int keyboard_write(char * buff, int num_bytes);

#endif

