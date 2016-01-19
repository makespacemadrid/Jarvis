// Arduino IDE WTF #1 hay que hacer el include de las librerias en el .ino! No vale si las haces en el .c
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <Wire.h>
#include <DHT.h>
#include <RTClib.h>

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
  //#include <MemoryFree.h>
  //#include <StandardCplusplus.h>  //Comentar para compilar en el ESP//Descomentar en arduino!!!!!!!!!!!!
#else
extern "C" {
#include "user_interface.h"
}
#endif

//Magia negra con ifdefs
//#define I2C_TRANSPORT (no implementado)
//#define DEBUG_STRINGS
//#define BIG_FLASH
#define EXTRA_CARRIAGE_RETURN //añade un retorno de carro a los paquetes de protocolo para leerlos mejor

#ifdef ESP8266
    #define DEBUG_STRINGS
    #define VERBOSE_DEBUG
    #define BIG_FLASH
#endif

uint8_t updateInterval = 25;

#include "helpers.h"
#include "jarvisNode.h"
#include "simplePowerControl.h"
#include "MakeSwitch.h"
#include "coffeeMaker.h"
#include "ledPanelNode.h"
#include "testNode.h"
#include "termometroNode.h"

jarvisNode* node;


void setup() 
{
    Serial.begin(115200);
    jarvisModules type = EEPROMStorage::getSettings().moduleType;
    if      (type == unknownModule)
    {
        node = new jarvisNode();
    }
    #ifdef ESP8266
    else if(type == espRepeaterModule)
    {
        node = new jarvisNode(jarvisNode::espRepeater);
    }
    #endif
    else if(type == simpleSwitchModule)
    {
        node = new simpleSwitch();
    }else if(type == makeSwitchModule)
    {
        node = new makeSwitch();
    }else if(type == airQualityModule)
    {

    }else if(type == simplePowerControlModule)
    {
        node = new simplePowerControl(14);
    }else if(type == advancedPowerControlModule)
    {

    }else if(type == coffeeMakerModule)
    {
        node = new coffeeMaker(14,15,A0);
    }else if(type == ledPanelModule)
    {
        node = new ledPanelNode();
    }else if(type == testNodeModule){
        node = new testNode();
    }else if(type == termometroNodeModule){
        node = new termometroNode();
    }
    node->setup();
}


#ifndef ESP8266
ISR(TIMER0_COMPA_vect){//interrupcion 0.
    //if(int0Pointer) int0Pointer();
    //mySwitch.currentSensor().isrRead();
}
#endif

void loop() {
  node->update();
}
