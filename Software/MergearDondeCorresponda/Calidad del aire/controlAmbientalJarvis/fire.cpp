/*
fire.cpp - Library for fire detection. 
Created by Crakernano, Jun 11, 2015. Released into the public domain.
http://www.trastejant.es
*/
#include "Arduino.h"
#include "fire.h"

// lowest and highest sensor readings:
const int sensorMin = 0;     // sensor minimum
const int sensorMax = 1024;  // sensor maximum

fire::fire(int pin){
   pinMode(pin, INPUT);
   _pin = pin;
}


float fire::analogReadSensor(){return analogRead(_pin);}
boolean fire::fireNow(){return digitalRead(_pin);}
int fire::dangerFire(){

  int sensorReading = analogRead(_pin);  // read the sensor on analog A0:
  int alertLevel = map(sensorReading, sensorMin, sensorMax, 0, 2);// map the sensor range (3 options):  
  return alertLevel;

}



