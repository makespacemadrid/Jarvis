//WTF hay que hacer el include de las librerias en el .ino!
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif
//WTF hay que hacer el include de las librerias en el .ino!


#include "ws2812led.h"
#include "ssr.h"
#include "piezoSpeaker.h"
#include "storage.h"
#include "communication.h"

#ifdef ESP8266
communicationModule* myWifi = new espNative();
#else
communicationModule* myWifi = new espProxy();
#endif

EEPROMStorage myEEPROM;
settingList settings(myEEPROM.getSettings()); // Toda la configuracion está en el settings.h

SSR                   mySwitch(settings.relayPin ,settings.currentMeterPin,settings.relayMaxAmps,settings.relayDimmable,settings.relayTemperatureSensor,settings.fanPin);
ws2812Strip           myLedStrip(settings.ledStripPin, settings.ledNumber);
piezoSpeaker          mySpeaker(settings.piezoPin);

//functionPointer int0Pointer = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("I:INIT");
  if(settings.factoryResetPin != -1)
  {
     pinMode(settings.factoryResetPin, INPUT);
     if(digitalRead(settings.factoryResetPin) == HIGH)
     {
        Serial.println("I:Factory reset!");
        myEEPROM.clearEEPROM();
        settings = myEEPROM.getSettings();
        //Falta resetear para que se cargen los nuevos settings
     }
  }

#ifdef ESP8266
  Serial.println("-ESP8266");
#else
  Serial.println("-Arduino");
#endif
  if(settings.factoryResetPin != -1) Serial.println("-Factory reset button");
  if(mySwitch.has_switch_pin())      Serial.println("-Switch");
  if(mySwitch.has_current_sensor())  Serial.println("-Current sensor");
  if(mySwitch.has_temp_sensor())     Serial.println("-Temperature sensor");
  if(mySpeaker.isValid())            Serial.println("-Buzzer");
  if(myLedStrip.isValid())           Serial.println("-WS2812");
  
  mySwitch.setup();
  myLedStrip.setup();
  mySpeaker.setup();
  //if(mySwitch.currentSensor().isValid())
    //int0Pointer = mySwitch.currentSensor().isrRead;
  Serial.println("I:INIT OK");
}

#ifndef ESP8266
ISR(TIMER0_COMPA_vect){
    //if(int0Pointer) int0Pointer();
    mySwitch.currentSensor().isrRead();
}
#endif

void loop() {
  
  mySwitch.update();
  myLedStrip.update();
  delay(50);
}
