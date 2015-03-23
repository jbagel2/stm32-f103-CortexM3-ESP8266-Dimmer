
#ifndef _TIME_H_
#define _TIME_H_

#include <stdint-gcc.h>
#include "stm32f10x.h"

volatile uint32_t microseconds;

void Init_Time();

void Delay(uint16_t milliseconds);

uint32_t Micros();

uint32_t Millis();


//systick interrupt


#endif // _TIME_H_
