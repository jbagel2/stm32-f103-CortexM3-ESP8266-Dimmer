
/********************************************************
 * Author: Jacob Pagel
 * Date: 03/05/2015
 * esp8266.c (esp8266 wifi library source file)
 ********************************************************/

#include "esp8266.h"


char *ESP_IPD_Data_Buffer_Pntr;
char ESP_IPD_DataBuffer[RxBuffSize];


char commandToSend[70];
volatile uint8_t waitingForReponse = 0;
volatile uint8_t OKFound = 0;
volatile uint8_t ERRORFound = 0;
volatile uint32_t TxWaitForResponse_TimeStmp = 0;
//extern volatile char USART3_RxBuffer_Buffer[RxBuffSize];
extern volatile char USART3_RxBuffer[RxBuffSize];

uint32_t dimmingValueToValidate = 30000; //Just a starting value that is outside the allowed

extern volatile uint32_t dimmingValue;
extern uint32_t lastDMABuffPoll;


uint16_t incommingDimmingValue = 0;
char *dimmingString;
char *URI;
char *queryString1;
char *queryValue1;
char *queryString2;
char *queryValue2;

IPD_Data currentIPD;

//METHOD DECLARATIONS
IPD_Data ProcessIPD_Data(char *IPD_Buffer);



const char *ATCommandsArray[18] = {"AT",
	"AT+CIPSTATUS",
	"AT+CWLAP",
	"AT+GMR",
	"AT+CWMODE?",
	"AT+CWMODE=3",
	"AT+CWJAP=\"SSID\",\"PASSWORD\"",
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

void SetArray_Size(char buffer[], uint16_t size)
{
	memset(buffer, '1', size);
}

void Wifi_ReadyWaitForAnswer()
{
	TxWaitForResponse_TimeStmp = Millis();
	waitingForReponse = 1;

}

void Wifi_WaitForAnswer()
{
	while(waitingForReponse == 1 && (Millis() - TxWaitForResponse_TimeStmp) < ESP_ResponseTimeout_ms);
	OKFound=0;
	ERRORFound=0;
}


char *WaitForAnswer_cmd_Buffer;
char *WaitForAnswer_ans_Buffer;
///Will parse the USART buffer periodically (based on #defined poll interval) for the echo of cmdToWaitFor
///in the response from the ESP8266 module.
void Wifi_WaitForAnswerCMD(char *cmdToWaitFor, uint16_t cmdSize)
{

	while(waitingForReponse == 1 && (Millis() - TxWaitForResponse_TimeStmp) < ESP_ResponseTimeout_ms)
		{
		WaitForAnswer_cmd_Buffer = memmem(USART3_RxBuffer,RxBuffSize,cmdToWaitFor,cmdSize);
		if(strlen(WaitForAnswer_cmd_Buffer)>0)
		{
			if(WaitForAnswer_ans_Buffer = memmem(WaitForAnswer_cmd_Buffer,strlen(WaitForAnswer_cmd_Buffer),"OK\r\n",4))
			{
				ClearArray_Size(WaitForAnswer_cmd_Buffer, strlen(WaitForAnswer_cmd_Buffer));
				OKFound=1;
				waitingForReponse = 0;
			}
			//Check for OK or Error Message

		}

		};
	//OKFound=0;
	//ERRORFound=0;
}

uint32_t pointerRange = 0;
void Wifi_WaitForAnswer_SEND_OK(uint16_t cmdSize)
{

	while(waitingForReponse == 1 && (Millis() - TxWaitForResponse_TimeStmp) < ESP_ResponseTimeout_ms)
	{
		WaitForAnswer_cmd_Buffer = memmem(USART3_RxBuffer,RxBuffSize,"AT+CIPSEND",10);
		if(strlen(WaitForAnswer_cmd_Buffer)>0)
		{
			while(waitingForReponse == 1 && (Millis() - TxWaitForResponse_TimeStmp) < ESP_ResponseTimeout_ms)
				{
				if(WaitForAnswer_ans_Buffer = memmem(USART3_RxBuffer,strlen(USART3_RxBuffer),"SEND OK\r\n",9))
				{
					pointerRange = WaitForAnswer_cmd_Buffer - WaitForAnswer_ans_Buffer;
					ClearArray_Size(WaitForAnswer_cmd_Buffer, cmdSize + 9);
					OKFound=1;
					waitingForReponse = 0;
				}
				}
			//Check for OK or Error Message
		}
	}

}


char closeConnectionBuffer[15];
void Wifi_CloseConnection(uint8_t connectionNum)
{
	sprintf(closeConnectionBuffer, "AT+CIPCLOSE=%d\r\n",connectionNum);
	Wifi_SendCustomCommand(closeConnectionBuffer);
}


void Wifi_SendCustomCommand(char *customMessage)
{
			Wifi_SendCustomCommand_External_Wait(customMessage);

			Wifi_WaitForAnswer();
			//for (wi=0;wi<735000;wi++);
}

void Wifi_SendCustomCommand_External_Wait(char *customMessage)
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

			//Wifi_WaitForAnswer();
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

	//Wifi_ReadyWaitForAnswer();
	while(USART_GetFlagStatus(ESP_USART, USART_FLAG_TXE) == RESET);

	USART_SendData(ESP_USART,'\n');

	Wifi_WaitForAnswerCMD(ATCommandsArray[command], strlen(ATCommandsArray[command]));
	//for (wi=0;wi<735000;wi++);

}

char *IPD_Processing_buf;
char *ConnectNum;
//Breaks the IPD message into a proper request object
IPD_Data ProcessIPD_Data(char *IPD_Buffer)
{
	//IPD_Processing_buf = strdupa(IPD_Buffer);
	//IPD_Processing_buf = &IPD_Buffer + 5;
	IPD_Data thisIPDMessage;

	strtok(IPD_Buffer,",");

	ConnectNum = strtok(NULL,",");
	thisIPDMessage.ConnectionNum = atoi(ConnectNum);

	thisIPDMessage.DataSize = strtok(NULL,":");
	//TODO: Probably need to add a check to make sure actual datasize matches expected..

	thisIPDMessage.RequestType = strtok(NULL," ");

	thisIPDMessage.URI = strtok(NULL," ");

	strtok(NULL,"\r\n");

	thisIPDMessage.Headers = strtok(NULL,"{");

	thisIPDMessage.Body = strtok(NULL,"}");
	return thisIPDMessage;

}

IPD_Data Wifi_CheckDMABuff_ForIPDData()
{
	currentIPD.Valid = 0;
	//if((Millis() - lastDMABuffPoll) >= DMA_Rx_Buff_Poll_Int_ms)
	//		{
				//Probably need to check for new client ({clientNum},CONNECT)
				lastDMABuffPoll = Millis();
				ESP_IPD_Data_Buffer_Pntr = memmem(USART3_RxBuffer,RxBuffSize,"+IPD",4);
				if(ESP_IPD_Data_Buffer_Pntr)
				{
					//position = DMA_GetCurrDataCounter(DMA1_Channel3);
					//position = strlen(USART3_RxBuffer);
					//Copy IPD message and data to its own buffer so DMA can go about its business
					strcpy(ESP_IPD_DataBuffer,ESP_IPD_Data_Buffer_Pntr);
					DMA_Cmd(DMA1_Channel3,DISABLE);

					//Wipes the received message from the DMA buffer (using the pointer to the data)
					//This makes sure the data doesn't get mistaken for a new request, on the next buffer polling.
					ClearArray_Size(ESP_IPD_Data_Buffer_Pntr,strlen(ESP_IPD_Data_Buffer_Pntr));
					DMA_Initialize(USART3_RxBuffer, RxBuffSize);


					//now we process since DMA isn't going to stomp on us.
					currentIPD = ProcessIPD_Data(ESP_IPD_DataBuffer);

						//TODO: Need to add a level of error detection/correction as data may be missing the
					if(strstr(currentIPD.RequestType, "POST"))
					{
						//if URI contains dimming (the test for now)
						if(strstr(currentIPD.URI, "dimming"))
						{
							if(strstr(currentIPD.URI, "?"))//If query String is found
							{
								URI = strtok(currentIPD.URI, "?");
								if(strstr(URI,"="))//If URI was sent prepended with a '/' this will be true
								{
									queryString1 = strtok(URI, "=");

									queryValue1 = strtok(NULL, "\0");
								}
								else
								{
								queryString1 = strtok(NULL, "=");
								if(strstr(currentIPD.URI, "&"))
								{
									queryValue1 = strtok(NULL, "&");
								}
								else
								{
									queryValue1 = strtok(NULL, "\0");
								}
								}
								currentIPD.Valid = 1;
							}

							dimmingValueToValidate = atoi(queryValue1);
							if(dimmingValueToValidate <= 13000)
							{
								dimmingValue = dimmingValueToValidate;

								RefreshCustomRESTResponseDimmer("172.20.112.136", "192.168.4.1", dimmingValue);
								//SendRESTResponse(currentIPD.ConnectionNum, RESTResponse_Headers_Test_OK, customRESTResponse);
							}
							else {
								RefreshCustomRESTResponse("172.20.112.136", "192.168.4.1", "dimmingValue", "InvalidValue");
							}
								currentIPD.Valid = 1;
						}
					}
					//printf("Incoming webrequest\r\n");
				}
				//DMA_Rx_Buff_Index = strlen(USART3_RxBuffer);
				//tstBuff = mempcpy(USART3_RxBuffer_Buffer, USART3_RxBuffer, RxBuffSize);
				//DMA_Rx_Buff_Index = tstBuff - &USART3_RxBuffer_Buffer[0];
				//tstBuff = memmem(USART3_RxBuffer,sizeof(USART3_RxBuffer),"OK\r\n",4);
				//ClearArray_Size(USART3_RxBuffer, sizeof(USART3_RxBuffer));

		//	}

				return currentIPD;
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
