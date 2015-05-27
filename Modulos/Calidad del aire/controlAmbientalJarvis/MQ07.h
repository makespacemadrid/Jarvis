/*
MQ07.h - Library for MQ-07 sensor(Co sensor).
Created by CrakerNano, May 18, 2015. Released into the public domain.
*/
#ifndef MQ07_h
#define MQ07_h
#include "Arduino.h"

class MQ07{
   public: MQ07(int pin); 
   float getCO();

   private:
   int _pin;
   int val;
   float ppm;
};

#endif
