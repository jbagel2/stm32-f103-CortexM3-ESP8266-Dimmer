
/********************************************************
 * Author: Jacob Pagel
 * Date: 03/05/2015
 * esp8266.h (esp8266 wifi library include file)
 ********************************************************/

#ifndef _ESP8266_H_
#define _ESP8266_H_

#include "stm32f10x_usart.h"
#include <stdio.h>
#include "string.h"

volatile uint8_t waitingForReponse;
volatile uint8_t OKFound;
volatile uint8_t ERRORFound;



#define ESP_USART USART3


const typedef enum {ESP_RESPONSE_READY,ESP_RESPONSE_Link,ESP_RESPONSE_Unlink,ESP_RESPONSE_OK,
					ESP_RESPONSE_SEND_OK,ESP_RESPONSE_IPD,ESP_RESPONSE_ERROR,
					ESP_RESPONSE_Wrong_Syntax,ESP_RESPONSE_BUSY_P,ESP_RESPONSE_BUSY_INET}ESP_Messages;

typedef enum
{
	WIFI_CHECK_MODULE_CONNECTION = 0,
	WIFI_CURRENT_STATUS, //Get current status
	WIFI_AP_LIST, // Get available AP's
	WIFI_FIRMWARE_VERSION, // Get current firmware version on ESP8266
	WIFI_GET_CURRENT_MODE, // Gets the current mode (Access Point, Station, both)
	WIFI_SET_MODE_BOTH_AP_ST, // Set Mode as both, Access Point, Station
	WIFI_JOIN_NONYA,
	WIFI_SHOW_CURRENT_CONNECTED_AP,
	WIFI_MODULE_RESET,
	WIFI_SET_MULTICONNECTION,
	WIFI_SET_BAUD_115200,
	WIFI_START_LOCAL_SERVER_PORT_80,
	WIFI_GET_CURRENT_IP,
	WIFI_SHOW_ACTIVE_CONNECTIONS,
	WIFI_LIST_CONNECTED_DEVICES_inAPModeOnly,
	WIFI_QUIT_CURRENT_AP,
	WIFI_START_ACCESS_POINT,
	WIFI_DISABLE_ECHO
}Wifi_Commands;

typedef enum
{
	OPEN,
	WEP,
	WPA_PSK,
	WPA2_PSK,
	WPA_WPA2_PSK
}Available_Encyption;

extern const char *ATCommandsArray[18];

#define WIFI_COMMAND(commandEnum) (ATCommandsArray[(commandEnum)])




void Wifi_ReadyWaitForAnswer();
void Wifi_WaitForAnswer();
void Wifi_CloseConnection(uint8_t connectionNum);
void Wifi_SendCustomCommand(char *customMessage);
void Wifi_SendCommand(Wifi_Commands command );
void ConnectToAP(char *apName, char *password);
void StartLocalAP(char *SSID, char *password, uint8_t channel, Available_Encyption encypt);


#endif //_ESP8266_H_
