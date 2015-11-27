/*
MQ07.cpp - Library for MQ-07 sensor (Co sensor). 
Created by Crakernano, May 18, 2015. Released into the public domain.
http://www.trastejant.es
*/
#include "Arduino.h"
#include "MQ07.h"

MQ07::MQ07(int pin){
   pinMode(pin, INPUT);
   _pin = pin;
}

float MQ07::getCO(){
  int val = analogRead(_pin);
  float ppm = map(val, 0, 1023, 0, 2000);//Mapeamos el valor leido a ppm
  return ppm;
}

float getCorrectedCO(float t, float h){}
float getRZero(){}
float getCorrectedRZero(float t, float h){}
