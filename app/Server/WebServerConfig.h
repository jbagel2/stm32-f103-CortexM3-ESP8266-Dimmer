

#ifndef _WEBSERVERCONFIG_H_
#define _WEBSERVERCONFIG_H_

#include "globalDefines.h"
//#define RESPONSE_BUFFER_SIZE 1000

//Modify this section to reflect your valid rest query strings

const typedef enum {Dim,Reset,Settings,Status}REST_ValidQueryStrings; // The query strings that are accepted by the rest call
const char *ValidQueryStrings[4]; // Query strings that are valid




#endif // _WEBSERVERCONFIG_H_
