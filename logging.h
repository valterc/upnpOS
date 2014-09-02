/* 
*
* ## upnpOS ##
* Micro Operating System for embedded systems with support for plug and play
* 
* FILE: logging.h -- Logging definitions
* 
* Version 0.1 - 2013
* 
*/

// BOF preprocessor bug prevent
// From: http://www.a-control.de/arduino-fehler/?lang=en
#if 1
__asm volatile ("nop");
#endif

#define ENA_LOG 1      //1 - Enable OS logging via Serial, 0 - Disable OS logging
#define SRL_CLRT 9600  //Serial clock rate

