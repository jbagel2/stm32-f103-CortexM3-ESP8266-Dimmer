#ifndef _Wifi_H
#define _Wifi_H

#include "USART3_Config.h"
#include "GetCommands.h"
#include <stdio.h>
#include "string.h"
#include "helpers.h"
//#include "RestApi/RestCommands.h"
#include "Server/WebServer.h"



extern volatile uint8_t waitingForReponse;
extern volatile uint8_t OKFound;
extern volatile uint8_t ERRORFound;
extern volatile char RxBuffer[];

uint32_t wi= 0;
//Just testing the ability to send webpages over ESP8266 USART (with AT Commands... :/)
const char siteHeader[] = "HTTP/1.1 200 Ok\r\nContent-Type: application/json;\r\n"
		"Connection: close;\r\n\r\n";

const char DimmingInputPage[] = "  <html><head><meta charset=\"UTF-8\"/><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Dimming Module Control Page</title></head><form action=\"dimming_form.asp\">Dimming Value: <input type=\"text\" name=\"dimvalue\"><br>"
		"<input type=\"submit\" value=\"Submit\">"
		"</form></html>";

/*extern const char *RESTResponse_Headers_Test_OK;  = //Just here for testing as this is just a static OK 200 response
		"HTTP/1.1 200 OK\r\n"
		"Cache-Control: no-cache\r\n"
		"Content-Type: application/json;charset-utf-8\r\n"
		"Connection: close\r\n\r\n\0"; */

		/*extern const char *RESTResponse_Body_TEST_JSON; =
		"{\"Status\":\"\{\"CurrentIP_WAN\":\"0.0.0.0\",\"currentip_lan\":\"192.168.4.1\",\"self_check_result\":\"OK\"}""}\0"; */

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

char *curResponse;

char webResponse[900];
char commandToSend[80];
char closeConnectionBuffer[16];
void Wifi_SendCustomCommand(char *customMessage);

volatile char WIFI_LinkDataReceivedParamsBuffer[8]; // will hold the data after the +IPD stating the amount of data incoming

//Calculate array size
#define ARRAYSIZE(x) (sizeof x/sizeof x[0])


uint8_t outgoingConnections = 0;
void ClearArray(char buffer[])
{
	memset(buffer, '\0', ARRAYSIZE(buffer));
}

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

void Wifi_CloseConnection(uint8_t connectionNum)
{
	sprintf(closeConnectionBuffer, "AT+CIPCLOSE=%d\r\n",connectionNum);
	Wifi_SendCustomCommand(closeConnectionBuffer);
}


void Wifi_SendCustomCommand(char *customMessage)
{
		while(*customMessage)
		{
			while(USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET);
			USART_SendData(USART3,*customMessage++);
		}

		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
			USART_SendData(USART3,'\r');

		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
			Wifi_ReadyWaitForAnswer();
			USART_SendData(USART3,'\n');

			Wifi_WaitForAnswer();
			//for (wi=0;wi<735000;wi++);
}

//Waits to return untill wifi responds (OK or ERROR)
void Wifi_SendCommand(Wifi_Commands command )
{
	const char *commandToSend = ATCommandsArray[command];

	while(*commandToSend)
	{
		while(USART_GetFlagStatus(USART3,USART_FLAG_TXE) == RESET);
		USART_SendData(USART3,*commandToSend++);
	}
	Wifi_ReadyWaitForAnswer();

	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	USART_SendData(USART3,'\r');

	Wifi_ReadyWaitForAnswer();
	while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);

	USART_SendData(USART3,'\n');

	Wifi_WaitForAnswer();
	//for (wi=0;wi<735000;wi++);

}

void Wifi_Init()
{
	Commands_Init();
}

void StartLocalAP(char *SSID, char *password, uint8_t channel, Available_Encyption encypt)
{

	sprintf(commandToSend, "AT+CWSAP=\"\",\"\",\"\",\"\"");

}

void ConnectToAP(char *apName, char *password) //Will utilize the arguments later, for now static to Nonya
{
	sprintf(commandToSend,"AT+CWJAP=\"%s\",\"%s\"",apName,password);
	Wifi_SendCustomCommand(commandToSend);
	//Wifi_SendCommand(WIFI_JOIN_NONYA);
}

void StartServer(uint8_t serverNum, uint16_t portNum)
{
	for (wi=0;wi<73500;wi++);
	for (wi=0;wi<73500;wi++);
	for (wi=0;wi<73500;wi++);
	//Wifi_SendCommand(WIFI_SET_MODE_BOTH_AP_ST);
	//for (wi=0;wi<73500;wi++);


	Wifi_SendCommand(WIFI_SET_MULTICONNECTION);

	//for (wi=0;wi<173500;wi++);
	//Wifi_SendCommand(WIFI_JOIN_NONYA);
	//for (wi=0;i<735000;i++);
	//Wifi_SendCommand(WIFI_GET_CURRENT_IP);
	//for (wi=0;i<73500;i++);
	Wifi_SendCommand(WIFI_START_LOCAL_SERVER_PORT_80);

	//for (wi=0;i<73500;i++);
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
	sprintf(webResponse, "AT+CIPSEND=%d,%d\r\n%s", connectionNum, (countof(DimmingInputPage))-1, DimmingInputPage);
	Wifi_SendCustomCommand(webResponse);
	for (wi=0;wi<70500;wi++);
	//Wifi_WaitForAnswer();
	OKFound=0;
	Wifi_CloseConnection(connectionNum);
}


//HTTPResponse newResponse;

uint16_t headLength = 0;
uint16_t bodyLength = 0;
uint8_t bodyLengthLength = 0;
char bodyLengthSizeString[4];

void SendRESTResponse(uint8_t connectionNum, const char *responseHeaders, const char *responseBody)
{
	//newResponse.HttpStatusCode="200";
	//newResponse->Headers=
	//char test[50];
	//buildHeader(test,ContentType,"application/json");

	ClearArray_Size(webResponse,ARRAYSIZE(webResponse));
	headLength = strlen(responseHeaders);
	bodyLength = strlen(responseBody);
	sprintf(bodyLengthSizeString, "%d", bodyLength);
	//Send CIPSEND separate from actual data
	bodyLengthLength = strlen(bodyLengthSizeString);
	//uint16_t countTest = strlen(responseHeaders);
	sprintf(webResponse, "AT+CIPSEND=%d,%d\r\n%s", connectionNum, headLength, responseHeaders);
	Wifi_SendCustomCommand(webResponse);

	//uint16_t testsize1 = ARRAYSIZE(webResponse);
	//uint16_t testsize2 = sizeof webResponse;
	ClearArray_Size(webResponse,strlen(webResponse));
	//ClearArray_Size(webResponse,headLength+12);
	//ClearArray(webResponse);
	sprintf(webResponse, "AT+CIPSEND=%d,%d\r\n%s%d\r\n", connectionNum, ((strlen(RequestHeaders_Array[ContentLength]))+bodyLengthLength)+4, RequestHeaders_Array[ContentLength],bodyLength-1);
	Wifi_SendCustomCommand(webResponse);
	ClearArray_Size(webResponse,strlen(webResponse));
	//ClearArray_Size(webResponse,((strlen(RequestHeaders_Array[ContentLength]))+bodyLengthLength)+15);
	sprintf(webResponse, "AT+CIPSEND=%d,%d\r\n%s\r\n", connectionNum, bodyLength+1, responseBody);
	Wifi_SendCustomCommand(webResponse);
	ClearArray_Size(webResponse,strlen(webResponse));
	//ClearArray_Size(webResponse, bodyLength+13);
	for (wi=0;wi<70500;wi++);
	//Wifi_CloseConnection(connectionNum);

}


void Wifi_SendDataToClient(uint8_t connectionNum)
{
	//(CIPMUX=0) AT+CIPSEND=<length>;
	//(CIPMUX=1) AT+CIPSEND= <id>,<length>
}



#endif // _Wifi_H
