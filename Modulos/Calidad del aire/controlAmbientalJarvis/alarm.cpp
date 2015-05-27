/*
alarm.cpp - Library for Jarvis alarm. 
Created by Crakernano, May 27, 2015. Released into the public domain.
http://www.trastejant.es
*/
#include "Arduino.h"
#include "alarm.h"

alarm::alarm(int LEDpin, int altavozPin){
   pinMode(LEDpin, OUTPUT);
   pinMode(altavozPin, OUTPUT);
   _LEDpin = LEDpin;
   _altavozPin = altavozPin;
}

//Alarma Sonora
void alarm::soundAlarm(int t){
  analogWrite(_altavozPin,20);
  delay(t);
}

void alarm::soundAlarm(int t1, int frecuency){
  analogWrite(_altavozPin,frecuency);
  delay(t);
}

void alarm::soundAlarm(int t1, int frecuency, int repeat){
  for(int i = 1; i< repeat; i++){
    analogWrite(_altavozPin,frecuency);
    delay(t);
  }
}

//Alarma visual
void alarm::ligthAlarm(int t, int d){
  digitalWrite(_LEDpin, HIGH);
  delay(t);
  digitalWrite(_LEDpin, LOW);
  delay(d);
}




void alarm::typeAlarm(int type){
  /*
  0 -> Alarma de incendios
  1 -> Alarma de gases explosivos
  2 -> Alarma de CO
  3 -> Alarma de CO2
  4 -> Alarma de humo
  5 -> Alarma de temperatura
  6 -> Alarma de alimentacion
  7 -> Alarma de cambio de modo
  8 -> Alarma de modo ahorro
  9 -> Alarma de desabilitacion
  10 -> Mal funcionamiento de la tarjeta SD
  
  */
  //Tiempos de encedido y apagado de la alarma visual
  int   fireLight[]  =   {200,100};
  int   gasLight[]   =   {200,100};
  int   COLight[]    =   {200,100};
  int   COOLight[]   =   {200,100};
  int   smokeLight[] =   {200,100};
  int   tempLight[]  =   {200,100};
  int   vccLight[]   =   {200,100};
  int   modeLight[]  =   {200,100};
  int   lowEnergyModeLight[] = {200,100};
  int   disableLight[]       = {200,100};
  int   SDfailLight[]        = {200,100};
  
  //Frecuencia de sonido de al alarma acustica
  int   fireSound  = 200;  
  int   gasSound   = 200;  
  int   COSound    = 200;  
  int   COOSound   = 200;  
  int   smokeSound = 200;  
  int   tempSound  = 200;  
  int   vccSound   = 200;  
  int   modeSound  = 200;  
  int   lowEnergyModeSound  = 200;  
  int   disableSound        = 200;  
  int   SDfailSound         = 200;
  
  switch (type) {
  case 0:    //Alarma de incendios
    soundAlarm(fireSound);
    ligthAlarm(fireLight[0],fireLight[1]);
    break;    
  case 1:    //Alarma de gases explisivos
    soundAlarm(gasSound);
    ligthAlarm(gasLight[0],gasLight[1]);
    break;
  case 2:    // Alarma de monoxido de carbono
    soundAlarm(COSound);
    ligthAlarm(COLight[0],COLight[1]);
    break;
  case 3:    // Alarma de dioxido de carbono
    soundAlarm(COOSound);
    ligthAlarm(COOLight[0],COOLight[1]);
    break;
  case 4:    // Alarma de humo
    soundAlarm(smokeSound);
    ligthAlarm(smokeLight[0],smokeLight[1]);
    break;   
  case 5:  //Alarma de temperatura
   soundAlarm(tempSound);
    ligthAlarm(tempLight[0],tempLight[1]); 
    break;
  case 6:    // Alarma de alimentacion
    soundAlarm(vccSound);
    ligthAlarm(vccLight[0],vccLight[1]);
    break;
  case 7:    // Alarma de cambio de modo
    soundAlarm(modeSound);
    ligthAlarm(modeLight[0],modeLight[1]);
    break;
  case 8:    // Alarma de modo LowEnergy
    soundAlarm(lowEnergyModeSound);
    ligthAlarm(lowEnergyModeLight[0],lowEnergyModeLight[1]);
    break;
  case 9:    // Alarma de 
    soundAlarm(disableSound);
    ligthAlarm(disableLight[0],disableLight[1]);
    break;    
  case 10:    // Alarma de desabilitacion de la placa
    soundAlarm(SDfailSound);
    ligthAlarm(SDfailLight[0],SDfailLight[1]);
    break;  
  default:    // your hand is nowhere near the sensor
    Serial.println("bright");
    break;
    
  } 
  
}
