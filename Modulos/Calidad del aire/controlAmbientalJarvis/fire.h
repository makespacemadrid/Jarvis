/*
fire.h - Library for fire detection.
Created by CrakerNano, Jun 11, 2015. Released into the public domain.
*/
#ifndef fire_h
#define fire_h


#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

class fire{
   public: fire(int pin);
     float analogReadSensor();
     int dangerFire();
     boolean fireNow();

   private:
 
     int _pin;
     int alertLevel;
     boolean flame;
     float sensorRead;
};

#endif
