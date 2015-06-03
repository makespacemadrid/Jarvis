/*
reboot.h - Library for Arduino reset.
Created by CrakerNano, May 20, 2015. Released into the public domain.
*/
#ifndef reboot_h
#define reboot_h

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

class reboot{
   public: reboot(int pin); 
     void rebootNow();
   
   private:
     int _pin;
};

#endif
