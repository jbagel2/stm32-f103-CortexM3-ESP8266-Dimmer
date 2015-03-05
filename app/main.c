#include "stm32f10x.h"
#include "misc.h"
//#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include <stdlib.h>
#include "stm32f10x_adc.h"
#include "stm32f10x_exti.h"
#include "USART3_Config.h"
#include "USART1_Config.h"

//#include "helpers.h"
#include "Wifi.h"
#include "GetCommands.h"





/*
 *  ATCommandsArray[0] = "AT+CIPSTATUS";
	ATCommandsArray[1] = "AT+CWLAP";
	ATCommandsArray[2] = "AT+GMR";
	ATCommandsArray[3] = "AT+CWMODE?";
	ATCommandsArray[4] = "AT+CWMODE=2";
	ATCommandsArray[5] = "AT+CWMODE=1";
	ATCommandsArray[6] = "AT+CWMODE=3";
 */


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

#define countof(a)   (sizeof(a) / sizeof(*(a)))


#define RxBuffSize 800

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
int i;
int main(void)
{
	// LED lamp 12800 MAX (before no response, wont turn on) But a few second delay before Diode saturation (light comes on)
	// 12400 - min for reasonable saturation delay
	dimmingValue = 12600; // 12600
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE); // ESP8266 - Wifi
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);



	POWER_LED_Config.GPIO_Speed = GPIO_Speed_50MHz;
	POWER_LED_Config.GPIO_Mode = GPIO_Mode_Out_PP;
	POWER_LED_Config.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_6; // PB1 - Maple On-board LED | PB6 - Maple Pin 16 | PB0 - CH_PD (Power ON) Pin for ESP8266 Wifi
	GPIO_Init(GPIOB,&POWER_LED_Config);

		GPIOB->BRR = GPIO_Pin_0; // Power OFF for ESP8266
		GPIOB->BSRR = GPIO_Pin_1; // PB1 - Maple On-board LED
		GPIOB->BRR = GPIO_Pin_6; // PB6 - Maple Pin 16
		GPIOB->BSRR = GPIO_Pin_0; // Power On for ESP8266
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);

	ZeroCross_Config.GPIO_Mode = GPIO_Speed_50MHz;
	ZeroCross_Config.GPIO_Mode = GPIO_Mode_IPD;
	ZeroCross_Config.GPIO_Pin = GPIO_Pin_14; // PB14 - Maple Pin 29
	GPIO_Init(GPIOB,&ZeroCross_Config);

	Button_Config.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	Button_Config.GPIO_Speed = GPIO_Speed_50MHz;
	Button_Config.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_7;
	GPIO_Init(GPIOB,&Button_Config);


	Init_USART3(115200,ENABLE);
	Init_USART1(115200,ENABLE);

	for (i=0;i<500000;i++);// FOR TESTING

	//for (i=0;i<5000;i++);// FOR TESTING
	//Need to wait for a sec before transmitting data. Let ESP8266 power on
	//for (i=0;i<500000;i++); // FOR TESTING


	//USART_SendData(USART3,"AT/r/n");

	ConfigZeroCrossExternalInt();
	ConfigZeroCross_NVIC();

	//for (i=0;i<20500;i++);
	Wifi_Init();
	//for (i=0;i<20500;i++);
	for (i=0;i<70500;i++);
	//ConnectToAP("AncestryGuest","shakyleaf");
	for (i=0;i<70500;i++);
	StartServer(1,80);

	for(;;)
    {

		//Check for data to transmit USART3

		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)) // Power cycle the ESP8266
		{
			Wifi_SendCommand(Command_To_Redirect);
		}
		/*if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)) // Power cycle the ESP8266
		{
			//ConnectToRemoteServer(testProto,testIP,testPort);
			StartServer(0,80);
		}*/

		if(newCommandWaiting == 1)
		{

			if(CMD_FULL_Incomming[0] != NULL)
			{
				CMD_FULL_Incomming[11] = '\r';
				CMD_FULL_Incomming[12] = '\0';
				strcpy(CMD_FULL_ResponseBuffer,CMD_FULL_Incomming);
				strcat(CMD_FULL_ResponseBuffer,"CMD RECVD\r");
				//CMD_FULL_ResponseBuffer = CMD_FULL_Incomming;
				//CMD_FULL_ResponseBuffer[13] = "CMD RECVD\r";
				Wifi_SendCustomCommand("AT+CIPSEND=0,23");
				//Wifi_SendCustomCommand("AT+CIPSEND=1,36");
				Wifi_SendCustomCommand(CMD_FULL_ResponseBuffer);
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
	for (i=0;i<1170;i++);// delay to allow 0 dimming value to be full brightness
	for (i=0;i<dimmingValue;i++); // NUMBER FOR ADJUSTMENT- 14750 = lowest found full wave
	GPIOB->BSRR = (1<<6);
	int i;
	for (i=0;i<70;i++); // 70 = 11.8us
	GPIOB->BRR = (1<<6);
	EXTI_ClearITPendingBit(EXTI_Line14);

}

void USART3_IRQHandler(void) //USART3 - ESP8266 Wifi Module
{
  if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
  {
    /* Read one byte from the receive data register */
	USART3_RxBuffer[RxCounter++] = USART_ReceiveData(USART3);

	if(USART3_RxBuffer[(RxCounter - 1)] == '\r')
	{
		//newCommandWaiting = 1;
	}

	if(RxCounter >= USART3_RxBufferSize)
	{
		RxCounter = 0;
	}

	USART_ClearITPendingBit(USART3,USART_IT_RXNE);
  }

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
