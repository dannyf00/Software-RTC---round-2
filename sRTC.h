#ifndef _sRTC_H
#define _sRTC_H

//implementing a software RTC: https://dannyelectronics.wordpress.com/2023/02/16/implementing-a-softwar-rtc/
//requirement:
//1. compiler support for <time.h>
//2. system to call sRTCISR() at a rate specified by sRTC_CALLRATE
//
//version history:
// 02/16/2023: initial porting
// 02/17/2023: added support for half seconds

#include <time.h>						//we use time.h
#include <stdint.h>						//we use standard data types

//hardware configuration
//pick the software RTC call rate to meet the timing requirement of your chip
//for example, for 16-bit timer, F_CPU / sRTC_CALLRATE < 64K
#define sRTC_CALLRATE	4				//sRTCISR calls per second
//end hardware configuration

//software RTC
//global defines
#define sRTC_TKS		1000000ul		//srtc tick rate per second

//global variables
typedef struct {
	time_t time;						//second counter since 1/1/1970
	uint32_t tick;						//tick counter
	int16_t cal;						//rtc calibration, in ppm. +=faster, -=slower
	uint8_t halfsec;					//0=first half sec, 1=2nd half sec
} sRTC_TypeDef;

void sRTCInit(void);					//initialize software counter //calibration from -128ppm to +128ppm, if sRTC_RATE = 1M
void sRTCSetCal(int16_t cal);			//set sRTC calibration
void sRTCISR(void);						//increment -> called sRTC_CALLRATE times per second
uint32_t sRTCTick(void);				//read rtc tick
time_t sRTC2time(time_t *t);			//read sRTC second counter
time_t time2sRTC(time_t t);				//set sRTC second counter
uint8_t sRTCHalfsec(void);				//return 0: first half of a sec, 1: 2nd half of a sec
//end software RTC
#endif	//srtc_h
