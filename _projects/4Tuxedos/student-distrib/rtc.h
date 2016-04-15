/*rtc.h 
 * - function for initializing RTC and its PIC connection
 */
#ifndef RTC_H
#define RTC_H



#ifndef ASM 

#include "i8259.h"
#include "lib.h"

//initialize the rtc
extern void rtc_init();

//handler for rtc
extern void rtc_handler(void);

//open rtc driver
extern int32_t rtc_open();

//close rtc driver
extern int32_t rtc_close();

//read function for rtc
extern int32_t rtc_read(int32_t * buff, uint32_t offset, int32_t num_bytes, int32_t var);

//write function for rtc
extern int32_t rtc_write(int32_t * buff, int32_t num_bytes);

#endif /* ASM */
#endif 
