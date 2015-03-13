# stm32-f103-CortexM3-ESP8266-Dimmer

###--- CoIDE (v1.7.8) Project. ---

AC mains triac controlled dimmer using STM32F103 for brains. ESP8266 Wifi for communication and mesh networking. 

I welcome comments on things you think can be improved along the way. CONSTRUCTIVE ONLY PLEASE!!

THIS IS A SUB PROJECT OF A LARGER SOON TO BE KICKSTARTED OPEN SOURCED HOME AUTOMATION PROJECT
 --- I have been working on this project alone for over 2 years (long before all these other poser kits showed up.. LOL (no offense to the posers reading this... ;). hence the slow progress. I have a wife, daughter, and full time job. Liek alot of you. )) ---


Current capabilities as software and hardware currently exist.
  -connection to existing wifi network
  -starting local TCP/UDP server
  -echo and CMD RECVD response back to connected wifi client if properly formated CMD received over DEBUG connection (USART1)
  -USART buffer parsing in interrupt for USART1 and 3. (being done inside inturrupt for now to simply code base and development) Keeping close eye on active AC dimming as logic for USART expands inside interrupt (no flicker AT ALL on connected AC Cree 60W (equiv.) LED bulb).
  -Lots of dimming code tweaks (while monitoring with high frequency bench oscilloscope) (still in progress), to get the most dimming possible without ANY flicker on LED bulds (so sick of the crap out there, that makes LED bulbs hum or cut out before they even get DIM. This project already does better at dimming than any comercial "LED compatible" dimmer I have found.(not saying a better one doesn't exist)).
  -dimming function is 360 degree of SINE (FULL WAVE), unlike a lot of dimmers that only work on the positive portion of the wave.
  ----- I'm sure there is more i'm not thinking of.



Project under heavy development. 
-(DevBoard is a Leaf Labs - Maple Mini (socketed on a custom protoboard (used as a connections breakout for testing)))
-This will be implemented on a custom built multi-layer PCB for production.
-EAGLE CAD files will be included soon for V0.1 hardware.

-- Basic COMS/Commands for ESP8266 working
-- USART1 - currently used for debuging and sending commands while testing (will come from parsed Wifi received data eventually)
-- USART3 - used for serial connection to ESP8266


As you can see below, there is still a lot to do. 

On the To-Do list:

(This item (#1) is currently being actively developed in the branch


1. Create basic http based response for a port 80 request to module. (Active on branch "browser_response_return_testing")
    * Report current status (to include)
      * Assigned IP
      * Currently connected Wifi AP SSID
      * Current Dimming level
      * Current, current throughput..
      * Current angle of triac in AC SINE
      * Current frequency of AC mains
      * Current Triac, MCU, WIFI module tempurature
      * Flag for if recent errors (last hour or so, not sure how long I want it to hold on to them)
    * Basic Textbox for direct input of dimming value
    * Manual MCU/Module reset/reboot
    * Option to enable Mesh network or Wifi network extention features (802.11n)
     
2. Make ESP8266 library more universal (more of a library, less of a specific implimentation).
3. Build stm32 hosted website library for ESP8266.
4. Test realworld throughput limits of USART data throughput to ESP8266 module. (current tested max without tweaking, 2Mbaud)
5. Build motion control/occupation sensor support (So user can add motion sensor if desired)
6. Build small current monitor library for AC and DC connection support (obviously hardware will also be required)
7. Build security and authentication library/layer for any write level connection requests.
8. Make dimming code auto adjust timing for 50Hz vs 60Hz mains (or any random freq fluctuation above or below) without code treaking required.
9. Lots more I'm not thinking of. I want the little STM32 to really have to work for its place in this project. So more functions and options will be added.
