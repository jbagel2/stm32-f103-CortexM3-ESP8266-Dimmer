

#ifndef __GLOBALDEFINES_H__
#define __GLOBALDEFINES_H__

/*
 * General variable definitions that need to be available project wide
 * ie..(buffer sizes, timeout values, delay values)
 */


#define ESP_ResponseTimeout_ms 6000
#define ESP_USART USART3
#define DEBUG_CMD_USART USART1


//USART buffers and related
#define RxBuffSize 1000
#define USART_TxComplete_Timeout_ms 1000
#define DMA_Rx_Buff_Poll_Int_ms 500

//Wifi related Variables and declarations
#define WIFI_COMMAND_ERROR "ERROR" // Expected response from ESP8266 on error
#define WIFI_COMMAND_ACCEPTED "OK" // Expected response from ESP8266 on successful command process
#define WIFI_RX_LineComplete = "\r\n"


//WebServer related defines
#define RESPONSE_BUFFER_SIZE 300


//Helpful Macros
#define countof(a)   (sizeof(a) / sizeof(*(a)))
#define ARRAYSIZE(x) (sizeof(x)/sizeof (x)[0]) // Need to test this one.. I think its broken.


#endif //#ifndef __GLOBALDEFINES_H__
