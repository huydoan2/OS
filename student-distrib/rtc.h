/*rtc.h 
 * - function for initializing RTC and its PIC connection
 */
#ifndef RTC_H
#define RTC_H

#include "i8259.h"
#include "lib.h"

//initialize the rtc
extern void rtc_init();

//handler for rtc
extern void rtc_handler(void);

//open rtc driver
extern int rtc_open();

//close rtc driver
extern int rtc_close();

//read function for rtc
extern int rtc_read(int * buff, int num_bytes);

//write function for rtc
extern int rtc_write(int * buff, int num_bytes);
#endif 
