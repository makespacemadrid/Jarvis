/*
puertaIngerior.cpp - Library for door control. 
Created by Crakernano, Jun 09, 2015. Released into the public domain.
http://www.trastejant.es
*/
#include "Arduino.h"
#include "puertaInterior.h"

puertaInterior::puertaInterior(int sensor, int cierre){
   pinMode(sensor, INPUT);
   pinMode(cierre, OUTPUT);
   _sensor = sensor;
   _cierre = cierre;
}


boolean puertaInterior::getDoorStatus(){return digitalRead(_sensor);}//return TRUE if door is open or FALSE if door is close
void puertaInterior::openDoor(){digitalWrite(_cierre, LOW);}//activate lock
void puertaInterior::closeDoor(){digitalWrite(_cierre, HIGH);}//desactivate lock

//desactivate lock temporarily
void puertaInterior::openDoor(int t){
  digitalWrite(_cierre, LOW);
  delay(t);
  digitalWrite(_cierre,HIGH);
}
