#ifndef _USART1_CONFIG_H
#define _USART1_CONFIG_H

#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "misc.h"
#include "stm32f10x_rcc.h"


#ifdef __cplusplus
 extern "C" {
#endif

 void Init_USART1(uint32_t baud, FunctionalState USART1_Interrupts);
 void USART1_SendString(char *MessageToSend);
 void USART1_IRQHandler(void);


#ifdef __cplusplus
}
#endif

#endif /* _USART1_CONFIG_H */
