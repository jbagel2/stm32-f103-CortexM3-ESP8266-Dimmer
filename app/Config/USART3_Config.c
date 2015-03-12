

/*
 * @file		USART3_Config.c
 * @author		Jacob Pagel
 * @version		0.01 (Alpha)
 * @date		02/17/15
 * @brief		All configuration required for USART3 periph placed in this file.
 * 	On my current dev board this USART is configured for the ESP8266 - Wifi
 *
 */

#include "USART3_Config.h"
#include "misc.h"
#include "helpers.h"
#include "stm32f10x_dma.h"
//#include "Wifi.h"

//extern uint8_t USART3_RxBufferSize;



extern uint8_t USART3_TxBuffer[];
extern volatile char USART3_RxBuffer[];
extern uint8_t TxCounter;
extern uint8_t RxCounter;
extern uint8_t NbrOfDataToTransfer;
extern uint8_t NbrOfDataToRead;

#define USART3_RxBufferSize (countof(USART3_RxBuffer) - 1)


USART_InitTypeDef USART3_Config;
void Init_USART3_RCC();
void Init_USART3_GPIO();
void Init_USART3_Interrupt();
void Init_USART_DMA();

//int i = 0;

void Init_USART3(uint32_t baud, FunctionalState USART3_Interrupts)
{
	//Init_USART3_RCC();
	Init_USART3_GPIO();

	USART3_Config.USART_BaudRate = baud;
	USART3_Config.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART3_Config.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART3_Config.USART_Parity = USART_Parity_No;
	USART3_Config.USART_StopBits = USART_StopBits_1;
	USART3_Config.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART3, &USART3_Config);

	USART_Cmd(USART3,ENABLE);

	Init_USART3_Interrupt();

	//Wifi_Init();
}

void Init_USART3_RCC()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); // (Starts and attaches Peripheral clock for USART3)
}

GPIO_InitTypeDef GPIOB_Config_USART3;

void Init_USART3_GPIO()
{

	//For RX Pin ---------------------------------
	GPIOB_Config_USART3.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOB_Config_USART3.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIOB_Config_USART3.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIOB_Config_USART3); // Saves above configuration to associated registers

	//--------------------------------------------

	//For TX Pin ---------------------------------
	GPIOB_Config_USART3.GPIO_Mode = GPIO_Mode_AF_PP; // Alternate Function Push-Pull
	GPIOB_Config_USART3.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOB, &GPIOB_Config_USART3); // Saves above configuration to associated registers

	//--------------------------------------------
}

NVIC_InitTypeDef USART3_Interrupt_Config;
void Init_USART3_Interrupt()
{
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	USART3_Interrupt_Config.NVIC_IRQChannel = USART3_IRQn;
	USART3_Interrupt_Config.NVIC_IRQChannelPreemptionPriority = 1;
	USART3_Interrupt_Config.NVIC_IRQChannelSubPriority = 0;
	USART3_Interrupt_Config.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&USART3_Interrupt_Config);

	USART_ITConfig(USART3,USART_IT_RXNE, ENABLE); // Enable USART Interrupts
	//USART_ITConfig(USART3,USART_IT_TXE, ENABLE);
}


void Init_USART_DMA()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_InitTypeDef USART_DMA_Config;

	//Just incase there is lingering config
	DMA_DeInit(DMA1_Channel1);

	USART_DMA_Config.DMA_DIR = DMA_DIR_PeripheralSRC;
	USART_DMA_Config.DMA_M2M = DMA_M2M_Enable;
	USART_DMA_Config.DMA_Mode = DMA_Mode_Circular;
	USART_DMA_Config.

}


void USART3_SendString(char *MessageToSend)
{
	while(MessageToSend++)
	{

		USART_SendData(USART3, MessageToSend);
	}
}

// Send "AT" to verify if ESP8266 is ready
void USART3_Send_AT_TEST()
{
	int i = 0;
	//AT+CWLAP
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);

	USART_SendData(USART3,'A');

	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);

	USART_SendData(USART3,'T');

	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);

	USART_SendData(USART3,'+');

	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);

	USART_SendData(USART3,'C');

	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);

	USART_SendData(USART3,'W');

	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);

	USART_SendData(USART3,'L');

	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);

	USART_SendData(USART3,'A');

	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);

	USART_SendData(USART3,'P');

	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	//USART_Send;
	USART_SendData(USART3,'\n');
	for (i=0;i<500000;i++);
}


void USART3_SendNextChar()
{

}

// - Interrupt routines






