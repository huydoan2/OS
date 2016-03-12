#ifndef _KEYBORAD_H
#define _KEYBORAD_H

#include "i8259.h"
#include "lib.h"

//Define values for Keyboard
#define KEYBOARD_DATA 0x60
#define KEYBOARD_COMMAND 0x64

//initialize the keyboard
extern void keyboard_init();

//handles the keyboard input
extern void keyboard_handler();

//get the scancode according to the keypress
extern char getScancode();

//access the character from the scancode array
extern char getchar();

#endif

