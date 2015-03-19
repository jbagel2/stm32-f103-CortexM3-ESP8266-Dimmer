
#ifndef _RESTCOMMANDS_H_
#define _RESTCOMMANDS_H_

/**********************************************************
 * Most everything in this section is just here to
 * simplify the parsing of HTTP calls for the REST
 * service.
 *
 **********************************************************/

//#include "CustomStructs/CustomTypes.h"

/* Moved to WebServer.h
#define HttpMethod(methodEnum) (HTTP_Method[(methodEnum)]) // Converts the enum to its corresponding string representation
*/
//typedef KeyValuePair_StringString Header;

//char *RequestMethod;
//char *ResponseMethod;

/* Moved to WebServer.h
const char RESTResponse_Headers_Test_OK[] = //Just here for testing as this is just a static OK 200 response
		" HTTP/1.1 200 OK\r\n"
		"Cache-Control: no-cache\r\n"
		"Content-Type: application/json;charset-utf-8\r\n"
		"Connection: close\r\n\r";
		// Needs to include Content-Length:

const char RESTResponse_Body_TEST_JSON[] =
		"{\"Status\":\"\{\"CurrentIP_WAN\":\"0.0.0.0\",\"currentip_lan\":\"192.168.4.1\",\"self_check_result\":\"OK\"}""}\0";
*/


/* Moved to WebServer.h
const typedef enum {GET=0,POST,PUT,DELETE}Http_Method_Enum; // The rest call types that we know what to do with
const char *HTTP_Method[4] = {"GET","POST","PUT","DELETE"}; // Array is used for string compare or value of for the Http_Call_Type ENUM
*/

/* Moved to WebServer.h
const typedef enum {Dim,Reset,Settings,Status}REST_ValidQueryStrings; // The query strings that are accepted by the rest call
const char *ValidQueryStrings[4] = {"Dim","Reset","Settings","Status"}; // Query strings that are valid
*/

/* moved to WebServer.h
typedef struct
{
	char *HttpStatusCode;
	char *Body;
	char *Headers[];

}Request_Type;
*/

/*
const typedef enum {ContentType,ContentLength,Accept,AcceptEncoding,Host,UserAgent}RequestHeaders_Types;
const char *RequestHeaders_Array[6] = {"Content-Type: ","Content-Length: ","Accept: ", "Accept-Encoding: ", "Host: ", "User-Agent: "};
*/


/* moved to WebServer
Header buildHeader(Header *newHeaderOut, RequestHeaders_Types type, char *headerValue)
{
	Header newHeader;
	newHeader.key = RequestHeaders_Array[type];
	newHeader.value = headerValue;
	//sprintf(newHeaderOut,"%s%s",RequestHeaders_Array[type],headerValue);

}
*/


/* moved to WebServer.h
typedef struct // Response object that will be transmitted back
{
	char *uriAbsolute;
	char *body;
	Header headers[];

}APIresponse;
*/

void ProcessRestRequest()
{

	//breakdown REST uri and find query strings

}

void Authorize()
{

}


//void buildResponseHeaders(ResponseHeaders headerValues);

//void buildResponse();

//void processRequestHeaders(RequestHeaders headerValues);

#endif //_RESTCOMMANDS_H_
