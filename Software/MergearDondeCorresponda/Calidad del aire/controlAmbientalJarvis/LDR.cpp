/*
LDR.cpp - Library for LDR sensor (Light sensor). 
Created by Crakernano, May 20, 2015. Released into the public domain.
http://www.trastejant.es
*/
#include "Arduino.h"
#include "LDR.h"

LDR::LDR(int pin){
   pinMode(pin, INPUT);
   _pin = pin;
}

int LDR::getLight(){return analogRead(_pin);}//Funcion que devuelve la lectura de la intensidad de luz.

//Devuelve el porcentaje de luz etectado
float LDR::getLightPerCent(){
   int light = analogRead(_pin);
   float lightPerCent = light/10.23;//calculamos el tanto por ciento de luz detectada
   return lightPerCent;
}
