
#ifndef _RESTCOMMANDS_H_
#define _RESTCOMMANDS_H_

/**********************************************************
 * Most everything in this section is just here to
 * simplify the parsing of HTTP calls for the REST
 * service.
 *
 **********************************************************/
typedef enum {GET,POST,PUT,DELETE}Http_Method; // The rest call types that we know what to do with
char *HTTP_Method[4] = {"GET","POST","PUT","DELETE"}; // Array is used for string compare or value of for the Http_Call_Type ENUM


typedef enum {Dim,Reset,Settings,Status}REST_ValidQueryStrings; // The query strings that are accepted by the rest call
char *ValidQueryStrings[4] = {"Dim","Reset","Settings","Status"}; // Query strings that are valid

typedef struct
{
	char *key;
	char *value;

}Header;

typedef struct
{
	char *ContentTypeValue;
	char *ContentLengthValue;
	char *AcceptValue;

}RequestHeaders;

typedef struct
{
	char *ContentTypeValue;
	char *ContentLengthValue;
	char *Authorization;

}ResponseHeaders;

typedef struct // Response object that will be transmitted back
{
	char *uriAbsolute;
	char *body;
	Header headers[];

}APIresponse;

void buildResponseHeaders(ResponseHeaders headerValues);

void buildResponse();

void processRequestHeaders(RequestHeaders headerValues);

#endif //_RESTCOMMANDS_H_
