

#include "time.h"

volatile uint32_t microseconds = 0;

void Init_Time()
{
SysTick_Config((SystemCoreClock/10000));
}

void Delay(uint16_t milliseconds)
{

}

uint32_t Micros()
{

}

uint32_t Millis()
{


}

void SysTick_Handler()
{

	microseconds++;
}

//
