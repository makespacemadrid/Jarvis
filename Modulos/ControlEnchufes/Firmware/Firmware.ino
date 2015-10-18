// Arduino IDE WTF #1 hay que hacer el include de las librerias en el .ino! No vale si las haces en el .c
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

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
#endif



#include "ws2812led.h"
#include "ssr.h"
#include "piezoSpeaker.h"
#include "settings.h"
#include "communication.h"

EEPROMStorage         myEEPROM;
settingList           mySettings(myEEPROM.getSettings()); // Toda la configuracion está en el settings.h

#ifdef ESP8266
communicationModule* myWifi = new espNative(mySettings.localPort,mySettings.bridgeMode);
#else
communicationModule* myWifi = new espProxy(mySettings.localPort);
#endif

SSR                   mySwitch(mySettings.relayPin ,mySettings.currentMeterPin,mySettings.relayMaxAmps,mySettings.relayDimmable,mySettings.relayTemperatureSensor,mySettings.fanPin);
ws2812Strip           myLedStrip(mySettings.ledStripPin, mySettings.ledNumber);
piezoSpeaker          mySpeaker(mySettings.piezoPin);

//functionPointer int0Pointer = 0;

void checkFactoryReset()
{
  if(mySettings.factoryResetPin != -1)
  {
     pinMode(mySettings.factoryResetPin, INPUT);
     if(digitalRead(mySettings.factoryResetPin) == LOW)
     {
        Serial.println("I:Factory reset!");
        if(myLedStrip.isValid())
        {
          myLedStrip.setup();
          myLedStrip.setColor(10,10,0);
        }
        mySpeaker.playTone(200,100);
        mySpeaker.playTone(500,100);
        mySpeaker.playTone(200,100);

        myEEPROM.clearEEPROM();
        mySpeaker.playTone(500,100);
        if(myLedStrip.isValid())
          myLedStrip.leds()[0].setColor(0,0,20);

        mySettings = myEEPROM.getSettings();
        mySpeaker.playTone(500,100);
        if(myLedStrip.isValid())
          myLedStrip.leds()[1].setColor(0,0,20);

        myEEPROM.storeSettings(mySettings);
        mySpeaker.playTone(1000,300);
        if(myLedStrip.isValid())
          myLedStrip.leds()[2].setColor(0,0,20);

        softReset();
     }
  }
}


void setup() {
  Serial.begin(115200);
  Serial.println("I:INIT");
  checkFactoryReset();
#ifdef ESP8266
  Serial.println("I:-ESP8266");
#else
  Serial.println("I:-Arduino");
#endif
  if(myEEPROM.hasSettings())
        Serial.println("I:-EEPROM");
  if(myLedStrip.isValid())
  {
    Serial.println("I:-WS2812");
    myLedStrip.setup();
    myLedStrip.off();
    myLedStrip.leds()[0].setColor(10,10,0);
    mySwitch.setStatusLed(&myLedStrip.leds()[2]);
    myWifi->setStatusLed(&myLedStrip.leds()[1]);
  }
  if(mySettings.alivePin != -1)
  {
    pinMode(mySettings.alivePin,OUTPUT);
    Serial.println("I:-Alive led");
  } 
  if(mySettings.factoryResetPin != -1) Serial.println("I:-Factory reset button");
  if(mySwitch.has_switch_pin()) 
  {
    Serial.println("I:-Switch");
    mySwitch.setup();
  }
  if(mySwitch.has_current_sensor())    Serial.println("I:-Current sensor");
  if(mySwitch.has_temp_sensor())       Serial.println("I:-Temperature sensor");
  if(mySpeaker.isValid())
  {
    Serial.println("I:-Buzzer");
    mySpeaker.setup();
  }

  myWifi->setup();
  //if(mySwitch.currentSensor().isValid())
    //int0Pointer = mySwitch.currentSensor().isrRead;
  Serial.println("I:INIT OK");
  if(myLedStrip.isValid()) myLedStrip.leds()[0].setColor(0,20,0);
}

uint8_t loopCount = 0;

void imAlive()
{
  if(loopCount == 15)
  {
    digitalWrite(mySettings.alivePin, !digitalRead(mySettings.alivePin));
    loopCount = 0;
    Serial.println("D:ImAlive!");
  }
  else
  {
    loopCount++;
  }
}

#ifndef ESP8266
ISR(TIMER0_COMPA_vect){//interrupcion 0.
    //if(int0Pointer) int0Pointer();
    mySwitch.currentSensor().isrRead();
}
#endif

void loop() {
  imAlive();
  mySwitch.update();
  myWifi->update();
  //myLedStrip.update();
  delay(100);
}
