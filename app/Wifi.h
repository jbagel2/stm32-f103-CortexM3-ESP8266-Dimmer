#ifndef _Wifi_H
#define _Wifi_H

#include "USART3_Config.h"
#include "GetCommands.h"
#include <stdio.h>


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

	Wifi_SendCommand(WIFI_SET_MODE_BOTH_AP_ST);
	for (i=0;i<73500;i++);
	Wifi_SendCommand(WIFI_SET_MULTICONNECTION);
	for (i=0;i<73500;i++);
	Wifi_SendCommand(WIFI_JOIN_NONYA);
	for (i=0;i<735000;i++);
	Wifi_SendCommand(WIFI_GET_CURRENT_IP);
	for (i=0;i<73500;i++);
	Wifi_SendCommand(WIFI_START_LOCAL_SERVER_PORT_80);
	for (i=0;i<73500;i++);
	Wifi_SendCommand(WIFI_GET_CURRENT_IP);



}

void ConnectToRemoteServer(char *Protocol, char *IPAddress, uint16_t port)
{
	char newMessage[40];

	sprintf(newMessage, "AT+CIPSTART=\"%s\",\"%s\",%d",Protocol,IPAddress,port);

	Wifi_SendCustomCommand(newMessage);
}

void Wifi_SendDataToClient()
{
	//(CIPMUX=0) AT+CIPSEND=<length>;
	//(CIPMUX=1) AT+CIPSEND= <id>,<length>
}


#endif // _Wifi_H
