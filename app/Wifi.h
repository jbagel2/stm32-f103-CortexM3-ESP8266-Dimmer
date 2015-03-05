#ifndef _Wifi_H
#define _Wifi_H

#include "USART3_Config.h"
#include "GetCommands.h"
#include <stdio.h>
#include "string.h"
#include "helpers.h"


//Just testing the ability to send webpages over ESP8266 USART (with AT Commands... :/)
const char siteHeader[] = "HTTP/1.1 200 Ok\r\nContent-Type: application/json;\r\n"
		"Connection: close;\r\n\r\n"
		"{\"TestKey\":\"testValue\"}\r\n";


const char WIFI_ClientConnected[] = "Link"; // client just connected if found
const char WIFI_ClientDisconnected[] = "Unlink"; // client disconnected if found
const char WIFI_Command_Successful[] = "OK";
const char WIFI_AlreadyInRequestedState[] = "no change";
const char WIFI_ClientDataFound[] = "+IPD"; // format +IPD,0,4:TEST : +IPD,{connectionNum},{LengthOfData}:{Data}
const char WIFI_BrowserClientConnected[] = ":GET / HTTP/1.1"; // This will follow the +IPD and the client num and char count (+IPD,1,342 + this string)

// Http request headers (Will end up in Enum)
const char WIFI_BrowserClientTypeHeader[] = "User-Agent:";
const char WIFI_BrowserAcceptEncodingHeader[] = "Accept-Encoding:";
const char WIFI_BrowserAcceptLangHeader[] = "Accept-Language:";

char webResponse[900];


volatile char WIFI_LinkDataReceivedParamsBuffer[8]; // will hold the data after the +IPD stating the amount of data incoming

uint8_t outgoingConnections = 0;

void Wifi_SendCustomCommand(char *customMessage)
{
		int i= 0;

		while(*customMessage)
		{
			while(USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET);
			USART_SendData(USART3,*customMessage++);
		}

		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
			USART_SendData(USART3,'\r');

		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
			USART_SendData(USART3,'\n');

			for (i=0;i<735000;i++);
}


void Wifi_SendCommand(Wifi_Commands command )
{
	int i= 0;
	const char *commandToSend = ATCommandsArray[command];

	while(*commandToSend)
	{
		while(USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET);
		USART_SendData(USART3,*commandToSend++);
	}

	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	USART_SendData(USART3,'\r');

	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	USART_SendData(USART3,'\n');

	for (i=0;i<735000;i++);

}

void Wifi_Init()
{
	Commands_Init();
}

void ConnectToAP(char *apName, char *password) //Will utilize the arguments later, for now static to Nonya
{
	Wifi_SendCommand(WIFI_JOIN_NONYA);
}

void StartServer(uint8_t serverNum, uint16_t portNum)
{
	int i =0;
	for (i=0;i<73500;i++);
	for (i=0;i<73500;i++);
	//Wifi_SendCommand(WIFI_SET_MODE_BOTH_AP_ST);
	//for (i=0;i<73500;i++);
	Wifi_SendCommand(WIFI_SET_MULTICONNECTION);
	for (i=0;i<73500;i++);
	//Wifi_SendCommand(WIFI_JOIN_NONYA);
	//for (i=0;i<735000;i++);
	//Wifi_SendCommand(WIFI_GET_CURRENT_IP);
	//for (i=0;i<73500;i++);
	Wifi_SendCommand(WIFI_START_LOCAL_SERVER_PORT_80);
	//for (i=0;i<73500;i++);
	//Wifi_SendCommand(WIFI_GET_CURRENT_IP);

}

void ConnectToRemoteServer(char *Protocol, char *IPAddress, uint16_t port)
{
	char newMessage[40];

	sprintf(newMessage, "AT+CIPSTART=\"%s\",\"%s\",%d",Protocol,IPAddress,port);

	Wifi_SendCustomCommand(newMessage);
}

void SendWebRequestResponse(uint8_t connectionNum)
{

	//Wifi_SendCustomCommand("AT+CIPSEND=0,36\rGot your web request. I'm responding");
	//Wifi_SendCustomCommand("AT+CIPSEND=1,36\rGot your web request. I'm responding");
	sprintf(webResponse, "AT+CIPSEND=%d,%d\r\n%s",connectionNum,countof(siteHeader), siteHeader);
	Wifi_SendCustomCommand(webResponse);
}


void Wifi_SendDataToClient()
{
	//(CIPMUX=0) AT+CIPSEND=<length>;
	//(CIPMUX=1) AT+CIPSEND= <id>,<length>
}




#endif // _Wifi_H
