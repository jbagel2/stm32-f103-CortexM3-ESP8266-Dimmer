
#ifndef _TIME_H_
#define _TIME_H_

#include <stdint-gcc.h>
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"

volatile uint32_t microseconds;

typedef enum{
	SECOND = 1,
	MILLISEC = 1000,
	MICROSEC = 1000000
}Time_Resolution;


void Init_Time(Time_Resolution res);

void Delay(uint16_t milliseconds);

//uint32_t Micros();

uint32_t Millis();


//systick interrupt


#endif // _TIME_H_
