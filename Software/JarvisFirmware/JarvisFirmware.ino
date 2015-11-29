// Arduino IDE WTF #1 hay que hacer el include de las librerias en el .ino! No vale si las haces en el .c
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <Wire.h>

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
  #include <StandardCplusplus.h> //Comentar para compilar en el ESP!!!!!!!!!!!!
  #include <MemoryFree.h>
#endif

#include "jarvisModule.h"


jarvisModule jarvisNode;//functionPointer int0Pointer = 0;

void setup() 
{
  jarvisNode.setup();
}


#ifndef ESP8266
ISR(TIMER0_COMPA_vect){//interrupcion 0.
    //if(int0Pointer) int0Pointer();
    //mySwitch.currentSensor().isrRead();
}
#endif

void loop() {
  jarvisNode.update();
}
