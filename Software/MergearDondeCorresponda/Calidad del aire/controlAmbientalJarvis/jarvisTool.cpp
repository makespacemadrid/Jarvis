/*
util.cpp - Library tool for Jarvis. 
Created by Crakernano, May 20, 2015. Released into the public domain.
http://www.trastejant.es
*/
#include "Arduino.h"
#include "jarvisTool.h"
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <EEPROM.h>

jarvisTool::jarvisTool(){}
void jarvisTool::setRefresh(int t){EEPROM.write(1, t);}

void jarvisTool::goToSleep(){
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); 
  sleep_enable(); 
}

void jarvisTool::disable(int t){delay(t);}//Permite desabilitar el modulo por un periodo de tiempo
