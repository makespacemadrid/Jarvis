/*
util.h - Library tool for jarvis.
Created by CrakerNano, May 20, 2015. Released into the public domain.
*/
#ifndef util_h
#define util_h
#include "Arduino.h"
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <EEPROM.h>

class util{
   public: util(); 
   void disable(int t);
   void setRefresh(int t);
   void wakeup();
   void goToSleep();
  
};

#endif
