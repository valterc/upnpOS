/* 
*
* ## upnpOS ##
* Micro Operating System for embedded systems with support for plug and play
* 
* FILE: upnpos.ino -- OS Entry Point
* 
* Version 0.1 - 2013
* 
*/

#include <Wire.h>
#include "logging.h"

void setup() {   
  
  #if ENA_LOG
    Serial.begin(SRL_CLRT);  
  #endif
  
  init_os();
  uinit();
  usetup();
}

void loop() {
  
  #if ENA_LOG
    Serial.print("l");
  #endif
  
  uloop();
}

