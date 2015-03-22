
/********************************************************
 * Author: Jacob Pagel
 * Date: 03/05/2015
 * WebServer.h (WebServer library include file)
 ********************************************************/

#ifndef _WEBSERVER_H_
#define _WEBSERVER_H_

#include "CustomStructs/KeyValuePair.h"
#include "WebServerConfig.h"
#include "esp8266.h"
#include "CustomStructs/GeneralMacros.h"

typedef KeyValuePair_StringString Header;

const typedef enum {GET=0,POST,PUT,DELETE}Http_Method_Enum; // The rest call types that we know what to do with
const char *HTTP_Method[4];

#define HttpMethod(methodEnum) (HTTP_Method[(methodEnum)]) // Converts the enum to its corresponding string representation


const typedef enum {ContentType,ContentLength,Accept,AcceptEncoding,Host,UserAgent}RequestHeaders_Types;
const char *RequestHeaders_Array[6];

#define HeaderKey(headerType) (RequestHeaders_Array[(headerType)]) // Testing macro to retrieve headerArray value

typedef struct
{
	char *HttpStatusCode;
	char *Body;
	char *Headers[];

}HTTPResponse;


typedef struct // Response object that will be transmitted back
{
	char *uriAbsolute;
	char *body;
	Header headers[];

}HTTPRequest;



void buildHeader(Header *newHeaderOut, RequestHeaders_Types type, char *headerValue);
void SendRESTResponse(uint8_t connectionNum, const char *responseHeaders, const char *responseBody);
void StartServer(uint8_t serverNum, uint16_t portNum);
void SendWebRequestResponse(uint8_t connectionNum);

/*
 *
 * TEST ARRAYS FOR DEBUGGING ONLY
 *
 *
 */

const char *RESTResponse_Headers_Test_OK; //Just here for testing as this is just a static OK 200 response


const char *RESTResponse_Body_TEST_JSON;






#endif // _WEBSERVER_H_
