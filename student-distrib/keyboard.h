#ifndef _KEYBORAD_H
#define _KEYBORAD_H

#include "i8259.h"
#include "lib.h"

#define KEYBOARD_DATA 0x60
#define KEYBOARD_COMMAND 0x64


extern void keyboard_init();

extern void keyboard_handler();

extern char getScancode();

extern char getchar();

#endif

