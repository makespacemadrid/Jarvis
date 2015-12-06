// Arduino IDE WTF #1 hay que hacer el include de las librerias en el .ino! No vale si las haces en el .c
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <Wire.h>
#include <DHT.h>

#ifdef ESP8266
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
#endif

//Arduino IDE WTF#2
//Por aguna razon la siguiente libreria que es para poder usar la clase vector con el arduino hace crujir la compilacion en el ESP
//Aunque la he probado a poner bajo un ifndef se sigue activando la libreria y fallando para el esp.
//Lo unico que funciona es comentarla para compilar en ESP. 
//  
//
#ifndef ESP8266
  //#include <StandardCplusplus.h>  //Comentar para compilar en el ESP//Descomentar en arduino!!!!!!!!!!!!
  //#include <MemoryFree.h>         //Comentar para compilar en el ESP//Descomentar en arduino!!!!!!!!!!!!
#endif

#include "jarvisModule.h"
#include "MakeSwitch.h"


jarvisModule* jarvisNode;

void setup() 
{
    jarvisModules type = EEPROMStorage::getSettings().moduleType;
    if      (type == unknownModule)
    {
        jarvisNode = new jarvisModule();
    }else if(type == espRepeaterModule)
    {
        jarvisNode = new jarvisModule(jarvisModule::espRepeater);

    }else if(type == simpleSwitchModule)
    {
        jarvisNode = new simpleSwitch();
    }else if(type == makeSwitchModule)
    {
        jarvisNode = new makeSwitch();
    }else if(type == airQualityModule)
    {

    }else if(type == simplePowerControlModule)
    {

    }else if(type == advancedPowerControlModule)
    {

    }

    jarvisNode->setup();
}


#ifndef ESP8266
ISR(TIMER0_COMPA_vect){//interrupcion 0.
    //if(int0Pointer) int0Pointer();
    //mySwitch.currentSensor().isrRead();
}
#endif

void loop() {
  jarvisNode->update();
}
