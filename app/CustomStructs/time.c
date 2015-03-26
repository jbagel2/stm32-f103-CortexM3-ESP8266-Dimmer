

#include "time.h"

volatile uint32_t millis = 0;

void Init_Time(Time_Resolution res)
{
SysTick_Config((SystemCoreClock/res));

//Just making sure that the Systick Interrupt is top priority
//Or the timer wont be accurate
NVIC_SetPriority(SysTick_IRQn, 0);

}

/*
void Delay(uint16_t milliseconds)
{

}
*/
/*
uint32_t Micros()
{

}
*/
uint32_t Millis()
{
return millis;
}

void SysTick_Handler()
{

	millis++;
}

//
