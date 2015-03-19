

#include "WebServer.h"

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
		"Cache-Control: no-cache\r\n"
		"Content-Type: application/json;charset-utf-8\r\n"
		"Connection: close\r\n\r";
		// Needs to include Content-Length:

const char *RESTResponse_Body_TEST_JSON =
		"{\"Status\":\"\{\"CurrentIP_WAN\":\"0.0.0.0\",\"currentip_lan\":\"192.168.4.1\",\"self_check_result\":\"OK\"}""}\0";

