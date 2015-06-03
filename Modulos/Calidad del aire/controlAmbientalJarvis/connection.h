/*
connection.h - Library for connection Jarvis to Wifi.
Created by CrakerNano, May 18, 2015. Released into the public domain.
*/
#ifndef connection_h
#define connection_h
#include <EEPROM.h>

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

class connection{
   public: connection(); 
     void setMAC(uint8_t mac);
     uint8_t getMAC();   
   
   
};

#endif
