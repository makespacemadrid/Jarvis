/*
energy.cpp - Library for energy control in Jarvis. 
Created by Crakernano, May 20, 2015. Released into the public domain.
http://www.trastejant.es
*/
#include "Arduino.h"
#include "energy.h"

energy::energy(int flag, int enable){
   pinMode(flag, INPUT);
   pinMode(enable, OUTPUT);
   _flag = flag;
   _enable = enable;
}

//Devuelve TRUE si hay alimentacion externa y FALSE en caso contrario
boolean energy::externalVcc(){
  if(digitalRead(_flag)==HIGH){return true;}
  else{return false;}
}
void energy::enableExternalVcc(){digitalWrite(_enable,HIGH);}
void energy::disableExternalVcc(){digitalWrite(_enable,LOW);}


