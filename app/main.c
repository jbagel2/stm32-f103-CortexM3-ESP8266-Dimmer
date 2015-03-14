#include "stm32f10x.h"
#include "misc.h"
//#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include <stdio.h>
#include "stm32f10x_rcc.h"
#include <stdlib.h>
#include "stm32f10x_adc.h"
#include "stm32f10x_exti.h"
#include "USART3_Config.h"
#include "USART1_Config.h"
#include <string.h>

//#include "helpers.h"
#include "Wifi.h"
#include "GetCommands.h"


/*
 * Possible Rx Command syntax
 * ::CMD:DIM:XXX:|: - :|: = end of transmission
 * ::CMD:DIM:XXX:??SECURITY_KEY??:|: Dim value only 3 chars as last two will just be 0's
 *  - XXXXX = 16bit Dimming value 1-15000 (max value is dependent on LED vs incondecant)
 *  - ??SECURITY_KEY?? = some (rotating if possible) security authentication key.
 *
 *
 *  - Possible Different Commands (CMD)
 *  	- DIM - Dimming command (obviously)
 *  	- RST - Triggers soft reboot (Mainly a restart back to main)
 *  	- HRDRST - Hardware triggered system reboot
 */

/*
 * DO NOT USE THESE PINS!!!!! (SWD DEBUG AND PROGRAM INTERFACE) these can be re-mapped if required.
 * PA14 - SWCLK
 * PA13 - SWDIO
 *
 */





#define countof(a)   (sizeof(a) / sizeof(*(a)))


#define RxBuffSize 400

#define TxBufferSize (countof(USART3_TxBuffer) - 1)
#define USART3_RxBufferSize (countof(USART3_RxBuffer) - 1)
#define USART1_RxBufferSize (countof(USART1_RxBuffer) - 1)

char *testProto = "TCP";
char *testIP = "172.20.112.1";
uint16_t testPort = 80;

//Wifi related Variables and declarations
#define WIFI_COMMAND_ERROR "ERROR" // Expected response from ESP8266 on error
#define WIFI_COMMAND_ACCEPTED "OK" // Expected response from ESP8266 on succesful command process

#define WIFI_RX_LineComplete = "\r\n"
char WIFI_CMD_RECEIVE_COMPLETE[] = ":|:"; // Will be appended to the end of a valid command

Current_CMD parsedCommand;

volatile char CMD_FULL_Incomming[13];
char CMD_FULL_ResponseBuffer[25];
volatile char byteToCommand[2];

volatile char lookForResponseBuffer[10];






uint8_t CMD_Incomming_InProgress = 0;


uint8_t USART3_TxBuffer[10]; //Starting initialization at 10 (for now)
volatile char USART3_RxBuffer[RxBuffSize];
volatile char USART1_RxBuffer[RxBuffSize];
uint8_t TxCounter = 0;
uint8_t RxCounter = 0;
uint8_t USART1_RxCounter = 0;

volatile uint8_t newCommandWaiting = 0;

volatile uint8_t Command_To_Redirect = 0;
volatile uint32_t dimmingValue;

//USART_InitTypeDef ESP8266_Interface_Config;
GPIO_InitTypeDef Button_Config;
GPIO_InitTypeDef POWER_LED_Config;
GPIO_InitTypeDef External_LED_Config;
GPIO_InitTypeDef ZeroCross_Config;
EXTI_InitTypeDef ZeroCross_Interrupt;
NVIC_InitTypeDef ZeroCross_VectorPrior;


void ConfigZeroCrossExternalInt();
void ConfigZeroCross_NVIC();
void LED(void);
volatile uint32_t mi = 0;
volatile uint32_t mj = 0;
volatile uint32_t mdi = 0;
volatile uint8_t waitingForReponse = 0;
volatile uint8_t OKFound = 0;
volatile uint8_t ERRORFound = 0;
volatile uint8_t OCount = 0;
volatile uint8_t LINKFound = 0;
volatile uint8_t indexPageRequestWaiting = 0;
volatile uint8_t activeConnectionNum = 0;

int main(void)
{
	printf("Entering Main()\r\n"); //SEMIHOSTING DEBUG OUT
	// LED lamp 12800 MAX (before no response, wont turn on) But a few second delay before Diode saturation (light comes on)
	// 12400 - min for reasonable saturation delay
	dimmingValue = 12600; // 12600

	printf("Starting RCC clocks\r\n"); //SEMIHOSTING DEBUG OUT
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); // ESP8266 - Wifi
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	printf("Finished Starting RCC clocks\r\n"); //SEMIHOSTING DEBUG OUT


	POWER_LED_Config.GPIO_Speed = GPIO_Speed_50MHz;
	POWER_LED_Config.GPIO_Mode = GPIO_Mode_Out_PP;
	POWER_LED_Config.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6; // PB1 - Maple On-board LED | PB6 - Maple Pin 16 | PB0 - CH_PD (Power ON) Pin for ESP8266 Wifi
	GPIO_Init(GPIOB,&POWER_LED_Config);
	printf("Finished GPIO Init for GPIO Port: B Pins: 0,1,6\r\n"); //SEMIHOSTING DEBUG OUT

		GPIOB->BRR = GPIO_Pin_0; // Power OFF for ESP8266
		printf("ESP8266 Powered OFF (CH01 Pin Disabled (Pulled Low))\r\n"); //SEMIHOSTING DEBUG OUT
		GPIOB->BSRR = GPIO_Pin_1; // PB1 - Maple On-board LED
		GPIOB->BRR = GPIO_Pin_6; // PB6 - Maple Pin 16
		GPIOB->BSRR = GPIO_Pin_0; // Power On for ESP8266
		printf("ESP8266 Powered ON\r\n"); //SEMIHOSTING DEBUG OUT
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);

	ZeroCross_Config.GPIO_Mode = GPIO_Speed_50MHz;
	ZeroCross_Config.GPIO_Mode = GPIO_Mode_IPD;
	ZeroCross_Config.GPIO_Pin = GPIO_Pin_14; // PB14 - Maple Pin 29
	GPIO_Init(GPIOB,&ZeroCross_Config);
	printf("GPIOB Pin 14 configured for Zero-Crossing detection (Maple Pin 29)\r\n"); //SEMIHOSTING DEBUG OUT

	Button_Config.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	Button_Config.GPIO_Speed = GPIO_Speed_50MHz;
	Button_Config.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_7;
	GPIO_Init(GPIOB,&Button_Config);
	printf("GPIOB Pin 14 configured for Zero-Crossing detection (Maple Pin 29)\r\n"); //SEMIHOSTING DEBUG OUT


	Init_USART3(115200,ENABLE);
	Init_USART1(115200,ENABLE);

	for (mj=0;mj<500000;mj++);// FOR TESTING

	//for (mj=0;mj<5000;mj++);// FOR TESTING
	//Need to wait for a sec before transmitting data. Let ESP8266 power on
	//for (mj=0;mj<500000;mj++); // FOR TESTING


	//USART_SendData(USART3,"AT/r/n");

	ConfigZeroCrossExternalInt();
	ConfigZeroCross_NVIC();

	//for (mj=0;mj<20500;mj++);
	Wifi_Init();
	printf("Wifi_Init() Complete\r\n"); //SEMIHOSTING DEBUG OUT
	//for (mj=0;mj<20500;mj++);
	for (mj=0;mj<130500;mj++);
	//ConnectToAP("Nonya","porsche911");
	//for (mj=0;mj<70500;mj++);
	printf("Preparing to start local ESP8266 server at ID: 1 Port: 80\r\n"); //SEMIHOSTING DEBUG OUT
	StartServer(1,80);

	for(;;)
    {

		if(indexPageRequestWaiting == 1)
		{
			printf("WebResquest found!\r\n"); //SEMIHOSTING DEBUG OUT
			for (mdi=0;mdi<80170;mdi++);// Wait for buffer. (need to replace with check for OK)
			indexPageRequestWaiting = 0;
			printf("Preparing to send web response to connection %d\r\n",activeConnectionNum); //SEMIHOSTING DEBUG OUT
			SendWebRequestResponse(activeConnectionNum);
		}
		//Check for data to transmit USART3

		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)) // Power cycle the ESP8266
		{
			Wifi_SendCommand(Command_To_Redirect);
			for (mj=0;mj<130500;mj++);//debounce
		}
		//check for command to send
		//check for connection to close

		if(newCommandWaiting == 1)
		{
			printf("New Command waiting!\r\n"); //SEMIHOSTING DEBUG OUT
			if(CMD_FULL_Incomming[0] != NULL)
			{
				CMD_FULL_Incomming[11] = '\r';
				CMD_FULL_Incomming[12] = '\0';
				strcpy(CMD_FULL_ResponseBuffer,CMD_FULL_Incomming);
				strcat(CMD_FULL_ResponseBuffer,"CMD RECVD\r");
				//CMD_FULL_ResponseBuffer = CMD_FULL_Incomming;
				//CMD_FULL_ResponseBuffer[13] = "CMD RECVD\r";
				//Wifi_SendCustomCommand("AT+CIPSEND=0,23");
				//Wifi_SendCustomCommand("AT+CIPSEND=1,36");
				//Wifi_SendCustomCommand(CMD_FULL_ResponseBuffer);
				SendWebRequestResponse(0);
				SendWebRequestResponse(1);

			}
			//Wifi_SendCustomCommand("AT+CIPSEND=0,11");
			//Wifi_SendCustomCommand("CMD RECVD\r");
			newCommandWaiting = 0;
		}

    }
}

void ClearRxBuffer(char buffer[])
{
	memset(&buffer[0], 0, sizeof(buffer));
}

void SetRedirectCommand(uint8_t commandNum)
{

}

void ConfigZeroCrossExternalInt() // Configured for Maple Mini Pin 29
{
	ZeroCross_Interrupt.EXTI_Line = EXTI_Line14;
	ZeroCross_Interrupt.EXTI_Mode = EXTI_Mode_Interrupt;
	ZeroCross_Interrupt.EXTI_Trigger = EXTI_Trigger_Rising;
	ZeroCross_Interrupt.EXTI_LineCmd = ENABLE;

	EXTI_Init(&ZeroCross_Interrupt);
}

void ConfigZeroCross_NVIC()
{
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	ZeroCross_VectorPrior.NVIC_IRQChannel = EXTI15_10_IRQn;
	ZeroCross_VectorPrior.NVIC_IRQChannelPreemptionPriority = 0;
	ZeroCross_VectorPrior.NVIC_IRQChannelSubPriority = 0;
	ZeroCross_VectorPrior.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&ZeroCross_VectorPrior);
}

void EXTI15_10_IRQHandler(void)
{
	for (mdi=0;mdi<1170;mdi++);// delay to allow 0 dimming value to be full brightness
	for (mdi=0;mdi<dimmingValue;mdi++); // NUMBER FOR ADJUSTMENT- 14750 = lowest found full wave
	GPIOB->BSRR = (1<<6);
	for (mdi=0;mdi<70;mdi++); // 70 = 11.8us
	GPIOB->BRR = (1<<6);
	EXTI_ClearITPendingBit(EXTI_Line14);

}

volatile uint8_t activeDataTrap = 0;
volatile char IPDDataBuffer[1000];
volatile uint16_t IPDDataIndex = 0;
volatile uint16_t bytesToGet = 462; //a count of the number of bytes to expect from an incoming +IPD data transmission DEBUG SET AT 400 need to get this from the IPD metadata
volatile uint8_t activeIPDTrap = 0;
volatile uint8_t IPDMetaIndex = 0;
volatile char IPDMetaBuffer[15]; // contains data after +IPD ie +IPD[,0,394:] - (data inside brackets), denoting the connection number sending the data and the byte count of expected data




void USART3_IRQHandler(void) //USART3 - ESP8266 Wifi Module
{
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */
	USART3_RxBuffer[RxCounter++] = (char)USART_ReceiveData(USART3);


	if(activeDataTrap == 1)
	{
		if(IPDDataIndex < bytesToGet)
		{
		IPDDataBuffer[IPDDataIndex++] = USART3_RxBuffer[RxCounter - 1];
		}
		else
		{
			IPDDataIndex = 0;
			activeDataTrap = 0; //Data collection should be complete
			indexPageRequestWaiting = 1;
		}

	}
	else if(activeIPDTrap == 1)
	{
		if(USART3_RxBuffer[RxCounter - 1] != ':')
		{
			IPDMetaBuffer[IPDMetaIndex++] = USART3_RxBuffer[RxCounter - 1];
			if(IPDMetaIndex >= sizeof(IPDMetaBuffer))
			{
				IPDMetaIndex = 0;
				activeIPDTrap = 0; //This data section should never be even close to this big. but this is here to keep it getting stuck in case of data corruption or something.
			}
		}
		else {
			activeIPDTrap = 0;
			activeDataTrap = 1;
			IPDMetaIndex = 0;
			//activeConnectionNum = IPDMetaBuffer[1] - '0'; // converts ascii to int
		}

	}
	else
	{


		if((USART3_RxBuffer[(RxCounter - 1)] == 'D')&&(USART3_RxBuffer[(RxCounter - 2)] == 'P')&&(USART3_RxBuffer[(RxCounter - 3)] == 'I')&&(USART3_RxBuffer[(RxCounter - 4)] == '+'))
			{
				activeIPDTrap = 1;
			}
		if((waitingForReponse==1)&&(USART3_RxBuffer[(RxCounter - 1)] == 'K')&&(USART3_RxBuffer[(RxCounter - 2)] == 'O'))
			{
				OKFound = 1;
				waitingForReponse =0;
			}
		if((waitingForReponse==1)&&(USART3_RxBuffer[(RxCounter - 2)] == 'R')&&(USART3_RxBuffer[(RxCounter - 3)] == 'O')&&(USART3_RxBuffer[(RxCounter - 5)] == 'R'))
			{
				ERRORFound=1;
				waitingForReponse =0;
			}

		if(RxCounter >= USART3_RxBufferSize)
			{
				RxCounter = 0;
			}
	}


	USART_ClearITPendingBit(USART3,USART_IT_RXNE);
  }
  USART_ClearITPendingBit(USART3,USART_IT_RXNE);
}

void USART1_IRQHandler(void) //USART1 - User Command recieve (DEBUG ONLY, commands from user will come from wifi)
{
  if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)
  {
	
    /* Read one byte from the receive data register */
	//byteToCommand[0] = ;
	USART1_RxBuffer[USART1_RxCounter++] = USART_ReceiveData(USART1);
	//char byteToCommand = USART1_RxBuffer[(USART1_RxCounter - 1)];
	if(USART1_RxBuffer[USART1_RxCounter - 1] == '\r')
	{
		// validate message receive string (if CMD termination string found)
		if(((USART1_RxBuffer[USART1_RxCounter - 2] == WIFI_CMD_RECEIVE_COMPLETE[2]) && (USART1_RxBuffer[USART1_RxCounter - 3] == WIFI_CMD_RECEIVE_COMPLETE[1]) && (USART1_RxBuffer[USART1_RxCounter - 4] == WIFI_CMD_RECEIVE_COMPLETE[0])))
		{

			strncpy(CMD_FULL_Incomming,&USART1_RxBuffer[USART1_RxCounter - 15],11); // pulls command from buffer into CMD_FULL_Incomming Array
			newCommandWaiting = 1;

		}
		else { // if CR is found but no CMD termination string (This is mainly for debugging, and this just references the 2 digit array index for the ATCommands array)
			byteToCommand[0] = USART1_RxBuffer[USART1_RxCounter - 3];
			byteToCommand[1] = USART1_RxBuffer[USART1_RxCounter - 2];

			Command_To_Redirect = atoi(byteToCommand);

			if(Command_To_Redirect >= sizeof(ATCommandsArray)) // Should probably run this outside the interrupt
				{
							//Command error has occured, as requested command is outside the bounds of the array
				Command_To_Redirect = 0;
				}
		}
	}

	if(USART1_RxCounter >= USART1_RxBufferSize) // if counter has hit the end of the buffer, reset counter to 0 (hack circular buffer) Will move this to DMA
	{
		USART1_RxCounter = 0;
	}

	USART_ClearITPendingBit(USART1,USART_IT_RXNE);
  }

}
