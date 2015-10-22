/*
connecion.cpp - Library for connecion jarvis to wifi. 
Created by Crakernano, May 20, 2015. Released into the public domain.
http://www.trastejant.es
*/
#include "Arduino.h"
#include "connection.h"
#include <EEPROM.h>

connection::connection(){}
void connection::setMAC(uint8_t mac){EEPROM.write(0, mac);}//metodo para asignar MAC de forma dinamica
uint8_t connection::getMAC(){return  EEPROM.read(0);}//metodo que devuelve la MAC almacenada en la EPROM


