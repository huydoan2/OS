/*rtc.h 
 * - function for initializing RTC and its PIC connection
 */
#ifndef RTC_H
#define RTC_H

#include "i8259.h"
#include "lib.h"

//initialize the rtc
extern void rtc_init();
extern void rtc_handler(void);

#endif 
