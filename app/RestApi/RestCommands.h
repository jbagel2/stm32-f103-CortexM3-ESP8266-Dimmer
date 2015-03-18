
#ifndef _RESTCOMMANDS_H_
#define _RESTCOMMANDS_H_

typedef enum // The rest call types that we know what to do with
{
	GET,
	POST,
	PUT,
	DELETE

}Http_Call_Type;


typedef enum // The query strings that are accepted by the rest call
{
	Dim,
	Reset,
	Settings,
	Status

}REST_Commands;

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
