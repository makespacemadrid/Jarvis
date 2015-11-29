/*
reset.cpp - Library for Arduino reset. 
Created by Crakernano, May 20, 2015. Released into the public domain.
http://www.trastejant.es
*/
#include "Arduino.h"
#include "reboot.h"

reboot::reboot(int pin){
   pinMode(pin, OUTPUT);
   _pin = pin;
}

void reboot::rebootNow(){digitalWrite(_pin,HIGH);}//Funcion para resetear la placa de Arduino
