#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

#include "ws2812led.h"
#include "ssr.h"
#include "piezoSpeaker.h"
#include "storage.h"

EEPROMStorage myEEPROM;

settingList settings(myEEPROM.getSettings()); // Toda la configuracion está en el settings.h

SSR                   mySwitch(settings.relayPin ,settings.currentMeterPin,settings.relayMaxAmps,settings.relayDimmable,settings.relayTemperatureSensor,settings.fanPin);
ws2812Strip           myLedStrip(settings.ledStripPin, settings.ledNumber);
piezoSpeaker          mySpeaker(settings.piezoPin);

//functionPointer int0Pointer = 0;

void setup() {
  mySwitch.setup();
  myLedStrip.setup();
  mySpeaker.setup();
  //if(mySwitch.currentSensor().isValid())
    //int0Pointer = mySwitch.currentSensor().isrRead;
}

ISR(TIMER0_COMPA_vect){
    //if(int0Pointer) int0Pointer();
    mySwitch.currentSensor().isrRead();
}

void loop() {
  
  mySwitch.update();
  myLedStrip.update();
}
