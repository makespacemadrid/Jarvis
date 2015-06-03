/*
jarvisTool.h - Library tool for jarvis.
Created by CrakerNano, May 20, 2015. Released into the public domain.
*/
#ifndef jarvisTool_h
#define jarvisTool_h
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <EEPROM.h>

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

class jarvisTool{
   public: jarvisTool(); 
   void disable(int t);
   void setRefresh(int t);   
   void goToSleep();
  
};

#endif
