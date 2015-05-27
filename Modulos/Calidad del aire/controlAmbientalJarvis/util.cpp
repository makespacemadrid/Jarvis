/*
util.cpp - Library tool for Jarvis. 
Created by Crakernano, May 20, 2015. Released into the public domain.
http://www.trastejant.es
*/
#include "Arduino.h"
#include "util.h"
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <EEPROM.h>

util::util(){}
void util::setRefresh(int t){EEPROM.write(1, t);}

void util::wakeup(){sleep_disable();}

void util::goToSleep(){
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); 
  sleep_enable(); 
}

//void disable(int t){delay(t);}//Permite desabilitar el modulo por un periodo de tiempo
