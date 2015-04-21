

#include "WebServer.h"


extern char webResponse[RESPONSE_BUFFER_SIZE];
void ClearArray_Size(char buffer[], uint16_t size);

uint32_t wi= 0;


const char *HTTP_Method[4] = {"GET","POST","PUT","DELETE"}; // Array is used for string compare or value of for the Http_Call_Type ENUM
const char *RequestHeaders_Array[6] = {"Content-Type: ","Content-Length: ","Accept: ", "Accept-Encoding: ", "Host: ", "User-Agent: "};

void buildHeader(Header *newHeaderOut, RequestHeaders_Types type, char *headerValue)
{
	Header newHeader;
	newHeader.key = RequestHeaders_Array[type];
	newHeader.value = headerValue;
	//sprintf(newHeaderOut,"%s%s",RequestHeaders_Array[type],headerValue);
}


const char *RESTResponse_Headers_Test_OK = //Just here for testing as this is just a static OK 200 response
		" HTTP/1.1 200 OK\r\n"
		"Content-Type: application/json\r\n"
		"Connection: close\r\n";
		// Needs to include Content-Length:XX
		// Calculated and appended based on body

const char *RESTResponse_Body_TEST_JSON =
		"{\"ID\":\"dim01\",\"Status\":{\"CurrentIP_WAN\":\"0.0.0.0\",\"currentip_lan\":\"192.168.4.1\",\"self_check_result\":\"OK\"}} \0";


const char DimmingInputPage[] = "  <html><head><meta charset=\"UTF-8\"/><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>Dimming Module Control Page</title></head><form action=\"dimming_form.asp\">Dimming Value: <input type=\"text\" name=\"dimvalue\"><br>"
		"<input type=\"submit\" value=\"Submit\">"
		"</form></html>";



uint16_t headLength = 0;
uint16_t bodyLength = 0;
uint8_t bodyLengthLength = 0;
char bodyLengthSizeString[4];

uint16_t fullLength = 0;
uint16_t fullCommandLen = 0;
///Sends a response to the active client (connectionNum)

char *responseBuff;
char data[200];

void SendRESTResponse(uint8_t connectionNum, const char *responseHeaders, const char *responseBody)
{

	//Should check if client is still connected, or connectionNum is a valid connection

	//Wipes the webResponse buffer array to make sure there is no extra data being appended to response
	ClearArray_Size(webResponse,ARRAYSIZE(webResponse));

	//Calculates lengths of strings to be sent (required prior to sending)
	headLength = strlen(responseHeaders);
	bodyLength = strlen(responseBody);
	sprintf(bodyLengthSizeString, "%d", bodyLength);
	bodyLengthLength = strlen(bodyLengthSizeString);

	fullLength = headLength + (((strlen(RequestHeaders_Array[ContentLength]))+bodyLengthLength)+4) + bodyLength+1;
	fullCommandLen = fullLength;
	sprintf(webResponse, "AT+CIPSEND=%d,%d\r\n", connectionNum, fullLength);
	fullCommandLen += strlen(webResponse);

	//void Wifi_ReadyWaitForAnswer();
	//Wifi_SendCustomCommand(webResponse);

	Wifi_SendCustomCommand_External_Wait(webResponse);

	ClearArray_Size(webResponse,strlen(webResponse));
	sprintf(webResponse,"%s%s%d\r\n\r\n%s",responseHeaders,RequestHeaders_Array[ContentLength],bodyLength + 1,responseBody);
	//strcat(webResponse, data);

	Wifi_SendCustomCommand_External_Wait(webResponse);

	Wifi_WaitForAnswer_SEND_OK(fullCommandLen);

	ClearArray_Size(webResponse,strlen(webResponse));

	//for (wi=0;wi<70500;wi++);
}



void StartServer(uint8_t serverNum, uint16_t portNum)
{
	for (wi=0;wi<73500;wi++);
	for (wi=0;wi<73500;wi++);
	for (wi=0;wi<73500;wi++);//TEMP waits to make sure ESP is ready
	//Need to just add check for "ready"

	Wifi_SendCommand(WIFI_SET_MULTICONNECTION); //Enables the multi-connection mode.


	//XXX: Need to build macro or function to append port number to request
	Wifi_SendCommand(WIFI_START_LOCAL_SERVER_PORT_80); // Currently just statically starts the server on 80

	//for (wi=0;i<73500;i++);
	//Wifi_SendCommand(WIFI_GET_CURRENT_IP);

}


void SendWebRequestResponse(uint8_t connectionNum)
{

	//Wifi_SendCustomCommand("AT+CIPSEND=0,36\rGot your web request. I'm responding");
	//Wifi_SendCustomCommand("AT+CIPSEND=1,36\rGot your web request. I'm responding");
	sprintf(webResponse, "AT+CIPSEND=%d,%d\r\n%s", connectionNum, strlen(DimmingInputPage), DimmingInputPage);
	Wifi_SendCustomCommand(webResponse);
	for (wi=0;wi<70500;wi++);
	//Wifi_WaitForAnswer();
	OKFound=0;
	Wifi_CloseConnection(connectionNum);
}
