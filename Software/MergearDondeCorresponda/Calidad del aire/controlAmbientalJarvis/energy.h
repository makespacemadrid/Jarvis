/*
energy.h - Library for Energy control in Jarvis.
Created by CrakerNano, May 20, 2015. Released into the public domain.
*/
#ifndef energy_h
#define energy_h

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

class energy{
   public: energy(int enable, int flag); 
    boolean externalVcc();
    void enableExternalVcc();
    void disableExternalVcc();
   private:
     int _pin;
     int _flag;
     int _enable;
};

#endif
