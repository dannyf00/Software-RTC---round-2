#include "sRTC.h"					//we use software rtc

//hardware configuration
//end hardware configuration

//software RTC
//global defines

//global variables

volatile sRTC_TypeDef sRTC={-1, 0, 0, 0};		//software RTC

//initialize software counter
void sRTCInit(void) {					//calibration from -128ppm to +128ppm, if sRTC_RATE = 1M
	//sRTC.tick_rate=sRTC_RATE;
	sRTC.time=-1;						//reset counter: -1=uninitiated timer
	sRTC.tick=0;						//reset the tick counter
	sRTC.cal=0;							//initialize calibration
	sRTC.halfsec=0;						//0->1st half sec, 1->2nd half sec
}

//initialize the calibration
void sRTCSetCal(int16_t cal) {
	sRTC.cal=cal;
}

//increment sRTC time - called sRTC_CALLRATE per second
void sRTCISR(void) {
	sRTC.tick += sRTC_TKS / sRTC_CALLRATE;		//increment tick, with noerror correction
	sRTC.halfsec=(sRTC.tick<sRTC_TKS/2)?0:1;	//half sec
	if (sRTC.tick>=sRTC_TKS) {
		sRTC.tick-=sRTC_TKS;			//reset tick
		sRTC.tick+=sRTC.cal;			//correct tick for calibration term
		sRTC.time+=1;					//increment sec
	}
}

//read sRTC second counter
time_t sRTC2time(time_t *t) {
	return *t=sRTC.time;
}

//set sRTC second counter - used for initialization
//returns -1 if uninitialized
time_t time2sRTC(time_t t) {
	return sRTC.time=t;
}

//read rtc tick
uint32_t sRTCTick(void) {
	return sRTC.tick;
}

//return half sec
//0: first half of a sec
//1: 2nd half of a sec
uint8_t sRTCHalfsec(void) {
	return sRTC.halfsec;
}

//end software rtc
