
#ifndef _GetCommands_H
#define _GetCommands_H

#ifdef __cplusplus
 extern "C" {
#endif

/*
const char *ATCommandsArray[18];
const char *SystemCommandsArray[4];

typedef struct
{
	uint16_t Current_Dim_Level;
	char *Current_Wifi_AP_Name;
	char *Current_Wifi_Strength;
	char *Current_Wifi_IP;
	char *Current_Wifi_MAC;
	char *Time_Since_Boot;
}SYSTEM_STATUS;

typedef struct
{
	char *Request_Type;
	char *Request_Key;
	char *Request_Value;
}Current_CMD;


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
	//WIFI_SELF_UPDATE,
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
	CMD_DIM = 0, //Get current status
	CMD_SOFT_RESET,
	CMD_HARD_RESET,
	CMD_STATUS

}Available_Commands;

void Build_SystemCommands()
{
	SystemCommandsArray[0] = "DIM";
	SystemCommandsArray[1] = "SRT";
	SystemCommandsArray[2] = "HRT";
	SystemCommandsArray[3] = "STA";
}

typedef enum
{
	OPEN,
	WEP,
	WPA_PSK,
	WPA2_PSK,
	WPA_WPA2_PSK
}Available_Encyption;


void Build_WifiCommands()
{
	ATCommandsArray[0] = "AT"; // Used to validate connection to device only
	ATCommandsArray[1] = "AT+CIPSTATUS";
	ATCommandsArray[2] = "AT+CWLAP"; // List available Access Points
	ATCommandsArray[3] = "AT+GMR"; // Get current installed Firmware version
	ATCommandsArray[4] = "AT+CWMODE?";
	ATCommandsArray[5] = "AT+CWMODE=3";
	ATCommandsArray[6] = "AT+CWJAP=\"YOUR_SSID\",\"PASSWORD\"";
	ATCommandsArray[7] = "AT+CWJAP?"; // Reports connected AP (if any)
	ATCommandsArray[8] = "AT+RST";
	ATCommandsArray[9] = "AT+CIPMUX=1";
	//ATCommandsArray[10] = "AT+CIUPDATE"; // Dangerous, borked last unit I used this on
	ATCommandsArray[10] = "AT+CIOBAUD=115200";
	ATCommandsArray[11] = "AT+CIPSERVER=1,80"; // Start local server on port 80
	ATCommandsArray[12] = "AT+CIFSR"; // Get assigned IP
	ATCommandsArray[13] = "AT+CIPSTART=?"; // Get assigned IP
	ATCommandsArray[14] = "AT+CWLIF"; // AP Mode check joined devices
	ATCommandsArray[15] = "AT+CWQAP";
	ATCommandsArray[16] = "AT+CWSAP="; //Parameters are required for this <ssid>,<password>,<channel>,<encryption>
	ATCommandsArray[17] = "ATE0";
}


void Commands_Init()
{

	Build_WifiCommands();
	Build_SystemCommands();

}

#ifdef __cplusplus
}
#endif

*/
#endif // _GetCommands_H
