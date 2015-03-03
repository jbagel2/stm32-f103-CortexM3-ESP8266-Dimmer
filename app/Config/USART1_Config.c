

#include "USART1_Config.h"
#include "misc.h"
#include "helpers.h"


USART_InitTypeDef USART1_Config;
void Init_USART1_RCC();
void Init_USART1_GPIO();
void Init_USART1_Interrupt();


void Init_USART1(uint32_t baud, FunctionalState USART1_Interrupts)
{

	Init_USART1_GPIO();

	USART1_Config.USART_BaudRate = baud;
	USART1_Config.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART1_Config.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART1_Config.USART_Parity = USART_Parity_No;
	USART1_Config.USART_StopBits = USART_StopBits_1;
	USART1_Config.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART1_Config);

	USART_Cmd(USART1,ENABLE);

	Init_USART1_Interrupt();

}

void Init_USART1_RCC()
{

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); // (Starts and attaches Peripheral clock for USART1)
}

GPIO_InitTypeDef GPIOA_Config_USART1;

void Init_USART1_GPIO()
{

	//For RX Pin ---------------------------------
	GPIOA_Config_USART1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOA_Config_USART1.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIOA_Config_USART1.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIOA_Config_USART1); // Saves above configuration to associated registers

	//--------------------------------------------

	//For TX Pin ---------------------------------
	GPIOA_Config_USART1.GPIO_Mode = GPIO_Mode_AF_PP; // Alternate Function Push-Pull
	GPIOA_Config_USART1.GPIO_Pin = GPIO_Pin_9;
	GPIO_Init(GPIOA, &GPIOA_Config_USART1); // Saves above configuration to associated registers

	//--------------------------------------------
}

NVIC_InitTypeDef USART1_Interrupt_Config;
void Init_USART1_Interrupt()
{
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	USART1_Interrupt_Config.NVIC_IRQChannel = USART1_IRQn;
	USART1_Interrupt_Config.NVIC_IRQChannelPreemptionPriority = 1;
	USART1_Interrupt_Config.NVIC_IRQChannelSubPriority = 0;
	USART1_Interrupt_Config.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&USART1_Interrupt_Config);

	USART_ITConfig(USART1,USART_IT_RXNE, ENABLE); // Enable USART Interrupts
	//USART_ITConfig(USART1,USART_IT_TXE, ENABLE);
}
