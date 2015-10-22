/*
LDR.h - Library for LDR sensor(light sensor).
Created by CrakerNano, May 20, 2015. Released into the public domain.
*/
#ifndef LDR_h
#define LDR_h

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

class LDR{
   public: LDR(int pin); 
     int getLight();
     float getLightPerCent();
   
   private:
     int _pin;
     int light;
     float lightPerCent;
};

#endif
