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

EEPROMStorage         myEEPROM;
settingList           mySettings(myEEPROM.getSettings()); // Toda la configuracion está en el settings.h

SSR                   mySwitch(mySettings.relayPin ,mySettings.currentMeterPin,mySettings.relayMaxAmps,mySettings.relayDimmable,mySettings.relayTemperatureSensor,mySettings.fanPin);
ws2812Strip           myLedStrip(mySettings.ledStripPin, mySettings.ledNumber);
piezoSpeaker          mySpeaker(mySettings.piezoPin);

//functionPointer int0Pointer = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("I:INIT");
  if(mySettings.factoryResetPin != -1)
  {
     pinMode(mySettings.factoryResetPin, INPUT);
     if(digitalRead(mySettings.factoryResetPin) == HIGH)
     {
        Serial.println("I:Factory reset!");
        myEEPROM.clearEEPROM();
        mySettings = myEEPROM.getSettings();
        //Falta resetear para que se cargen los nuevos settings
     }
  }
#ifdef ESP8266
  Serial.println("-ESP8266");
#else
  Serial.println("-Arduino");
#endif
  if(myLedStrip.isValid())
  {
    Serial.println("I:-WS2812");
    myLedStrip.setup();
    myLedStrip.off();
    myLedStrip.leds()[0].setColor(10,10,0);
    mySwitch.setStatusLed(&myLedStrip.leds()[2]);
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


  //if(mySwitch.currentSensor().isValid())
    //int0Pointer = mySwitch.currentSensor().isrRead;
  Serial.println("I:INIT OK");
}

uint8_t loopCount = 0;
void imAlive()
{
  if(loopCount == 15)
  {
    if(mySwitch.switched())
      mySwitch.switchOff();
    else
      mySwitch.switchOn();
      
    digitalWrite(mySettings.alivePin, !digitalRead(mySettings.alivePin));
    loopCount = 0;
  }
  else
  {
    loopCount++;
  }
}

#ifndef ESP8266
ISR(TIMER0_COMPA_vect){
    //if(int0Pointer) int0Pointer();
    mySwitch.currentSensor().isrRead();
}
#endif

void loop() {

  imAlive();
  mySwitch.update();
  //myLedStrip.update();
  delay(50);
}
