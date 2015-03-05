

#ifndef _LOCALSITE_H_
#define _LOCALSITE_H_

//#include "Wifi.h"
//Just testing the ability to send webpages over ESP8266 USART (with AT Commands... :/)
const char siteHeader[] = "<html><head><script type=\"text/javascript\">var r; try {r = new XMLHttpRequest();} catch (e) {try {r = new ActiveXObject('Microsoft.XMLHTTP');} catch (e) {}} function set (c) { r.open('PUT', './led/' + c, false); r.send(null); }</script><style type=\"text/css\">"
		".b {width:112; height:112}.g {color:lightgrey}"
	    "</style>"
	  "</head>"
	  "<body><table height=\"100%\" width=\"100%\">"
	    "<tr><td align=\"center\" valign=\"middle\">"
	      "<p>"
	        "<input type=\"button\" class=\"b\" style=\"background-color:#ff0000\" onclick=\"set('ff0000')\"/>&nbsp;&nbsp;"
	        "<input type=\"button\" class=\"b\" style=\"background-color:#00ff00\" onclick=\"set('00ff00')\"/>&nbsp;&nbsp;"
	        "<input type=\"button\" class=\"b\" style=\"background-color:#0000ff\" onclick=\"set('0000ff')\"/>"
	      "</p>"
	      "<p>Site Hosted on Jacob Pagel's STM32F103 ARM MCU</p>"
	    "</td></tr>"
	  "</table></body></html>";



#endif //"_LOCALSITE_H_"
