/*
puertaInterior.h - Library tool for door control.
Created by CrakerNano, Jun 9, 2015. Released into the public domain.
*/

#ifndef puertaInterior_h
#define puertaInterior_h

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

class puertaInterior{
  
  public: 
    puertaInterior(int sensor, int cierre); 
    boolean getDoorStatus();
    void openDoor();
    void closeDoor();
    void openDoor(int t);
    
  private:
    int _sensor;
    int _cierre;
    boolean _doorStatus;  
};

#endif
