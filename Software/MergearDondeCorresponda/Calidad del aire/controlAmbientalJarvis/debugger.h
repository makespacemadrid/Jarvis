/*
debugger.h - Library for debugger air quality module.
Created by CrakerNano, May 28, 2015. Released into the public domain.
*/
#ifndef debugger_h
#define debugger_h

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

class debugger{
   public: debugger(int rx,int tx); 
//Modulo de debugger
void debuggerMode(float temperatura, float humedad, float NH3, float CO, float COO, boolean flame);
void disableDebuggerMode();//Activar el modo de debuggeo
void enableDebuggerMode();//Activa el modo de debuggeo
boolean debuggerStatusMode();//Devuelve el valor de la variable debuggear
   
   private:
     int _rx;
     int _tx;
     boolean _debuggear;
};

#endif
