
/********************************************************
 * Author: Jacob Pagel
 * Date: 03/05/2015
 * esp8266.c (esp8266 wifi library source file)
 ********************************************************/

#include "esp8266.h"


char commandToSend[80];
volatile uint8_t waitingForReponse = 0;
volatile uint8_t OKFound = 0;
volatile uint8_t ERRORFound = 0;


const char *ATCommandsArray[18] = {"AT",
	"AT+CIPSTATUS",
	"AT+CWLAP",
	"AT+GMR",
	"AT+CWMODE?",
	"AT+CWMODE=3",
	"AT+CWJAP=\"YOUR_SSID\",\"PASSWORD\"",
	"AT+CWJAP?",
	"AT+RST",
	"AT+CIPMUX=1",
	"AT+CIOBAUD=115200",
	"AT+CIPSERVER=1,80",
	"AT+CIFSR",
	"AT+CIPSTART=?",
	"AT+CWLIF",
	"AT+CWQAP",
	"AT+CWSAP=",
	"ATE0"};


const char *ESP_Responses[10] =
{
		"ready",
		"Link",
		"Unlink",
		"OK",
		"SEND OK",
		"+IPD",
		"ERROR",
		"wrong syntax",
		"busy p...",
		"busy inet..."
};

void ClearArray_Size(char buffer[], uint16_t size)
{
	memset(buffer, '\0', size);
}
void Wifi_ReadyWaitForAnswer()
{
	waitingForReponse = 1;

}

void Wifi_WaitForAnswer()
{
	while(waitingForReponse == 1);
	OKFound=0;
	ERRORFound=0;
}


char closeConnectionBuffer[15];
void Wifi_CloseConnection(uint8_t connectionNum)
{
	sprintf(closeConnectionBuffer, "AT+CIPCLOSE=%d\r\n",connectionNum);
	Wifi_SendCustomCommand(closeConnectionBuffer);
}


void Wifi_SendCustomCommand(char *customMessage)
{
		while(*customMessage)
		{
			while(USART_GetFlagStatus(ESP_USART,USART_FLAG_TXE) == RESET);
			USART_SendData(ESP_USART,*customMessage++);
		}

		while(USART_GetFlagStatus(ESP_USART, USART_FLAG_TXE) == RESET);
			USART_SendData(ESP_USART,'\r');

		while(USART_GetFlagStatus(ESP_USART, USART_FLAG_TXE) == RESET);
			Wifi_ReadyWaitForAnswer();
			USART_SendData(ESP_USART,'\n');

			Wifi_WaitForAnswer();
			//for (wi=0;wi<735000;wi++);
}

//Waits to return untill wifi responds (OK or ERROR)
void Wifi_SendCommand(Wifi_Commands command )
{
	const char *commandToSend = ATCommandsArray[command];

	while(*commandToSend)
	{
		while(USART_GetFlagStatus(ESP_USART,USART_FLAG_TXE) == RESET);
		USART_SendData(ESP_USART,*commandToSend++);
	}
	Wifi_ReadyWaitForAnswer();

	while(USART_GetFlagStatus(ESP_USART, USART_FLAG_TXE) == RESET);
	USART_SendData(ESP_USART,'\r');

	Wifi_ReadyWaitForAnswer();
	while(USART_GetFlagStatus(ESP_USART, USART_FLAG_TXE) == RESET);

	USART_SendData(ESP_USART,'\n');

	Wifi_WaitForAnswer();
	//for (wi=0;wi<735000;wi++);

}


///Pretty self explanatory
void ConnectToAP(char *apName, char *password) //Will utilize the arguments later, for now static to Nonya
{
	//TODO: Need to add check that ESP is in a compatible client mode
	sprintf(commandToSend,"AT+CWJAP=\"%s\",\"%s\"",apName,password);
	Wifi_SendCustomCommand(commandToSend);
}

//Configures ESP82667 Access Point with given parameters.
void StartLocalAP(char *SSID, char *password, uint8_t channel, Available_Encyption encypt)
{

	sprintf(commandToSend, "AT+CWSAP=\"\",\"\",\"\",\"\"");

}
