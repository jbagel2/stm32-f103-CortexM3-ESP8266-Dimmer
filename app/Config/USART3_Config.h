





#ifndef _USART3_CONFIG_H
#define _USART3_CONFIG_H

#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "stm32f10x_rcc.h"

#ifdef __cplusplus
 extern "C" {
#endif

void Init_USART3(uint32_t baud, FunctionalState USART3_Interrupts);
void Init_USART3_DMA(uint32_t baud, volatile char DMA_RxBuffer[], uint16_t BufSize);
//void USART3_Send_AT_TEST();//Just send the basic AT command looking for a ready response
//void USART3_SendString(char *MessageToSend);
void USART3_IRQHandler(void);
void DMA_Initialize(volatile char DMA_RxBuffer[], uint16_t BufSize);

#ifdef __cplusplus
}
#endif

#endif /* _USART3_CONFIG_H */
