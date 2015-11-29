/*
MG811.cpp - Library for MG811 sensor (COO sensor). 
Created by Crakernano, May 20, 2015. Released into the public domain.
http://www.trastejant.es
*/
#include "Arduino.h"
#include "MG811.h"

MG811::MG811(int pin){
   pinMode(pin, INPUT);
   _pin = pin;
}

float MG811::getCOO(){
  int val = analogRead(_pin);
  float ppm = map(val,0,1023,0,10000);
  return ppm;
}


