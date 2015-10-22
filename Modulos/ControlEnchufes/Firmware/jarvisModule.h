
#ifndef JMODULE
#define JMODULE

#include "communication.h"
#include "ws2812led.h"
#include "ssr.h"
#include "piezoSpeaker.h"
#include "settings.h"
#include "communication.h"

#ifdef ESP8266
class jarvisModule : public espNative
{
public:
  jarvisModule() : m_EEPROM(),espNative(m_EEPROM.settings().localPort,m_EEPROM.settings().bridgeMode) ,
#else 
class jarvisModule : public espProxy
{
public:
  jarvisModule() : m_EEPROM(), espProxy(m_EEPROM.settings().localPort,false) ,
#endif
//inicializacion de los componentes en el constructor.
  m_switch(m_EEPROM.settings().relayPin ,m_EEPROM.settings().currentMeterPin,m_EEPROM.settings().relayMaxAmps,m_EEPROM.settings().relayDimmable,m_EEPROM.settings().relayTemperatureSensor,m_EEPROM.settings().fanPin) ,
  m_ledStrip(m_EEPROM.settings().ledStripPin, m_EEPROM.settings().ledNumber) ,
  m_speaker(m_EEPROM.settings().piezoPin)
  {  };

  void setup()
  {
    Serial.begin(115200);
    debugln(String(F("I:INIT")));
    checkFactoryReset();
  #ifdef ESP8266
    debugln(String(F("I:-ESP8266")));
    espNative::setup();
  #else
    debugln(String(F("I:-Arduino")));
    espProxy::setup();
  #endif
    if(m_EEPROM.hasSettings())
      debugln(String(F("I:-EEPROM")));
    if(m_ledStrip.isValid())
    {
      debugln(String(F("I:-WS2812")));
      m_ledStrip.setup();
      m_ledStrip.off();
      m_ledStrip.leds()[0].setColor(10,10,0);
      m_switch.setStatusLed(&m_ledStrip.leds()[2]);
      setStatusLed(&m_ledStrip.leds()[1]);
    }
    if(m_EEPROM.settings().alivePin != -1)
    {
      pinMode(m_EEPROM.settings().alivePin,OUTPUT);
      debugln(String(F("I:-Alive led")));
    } 
    if(m_EEPROM.settings().factoryResetPin != -1) debugln(String(F("I:-Factory reset button")));
    if(m_switch.has_switch_pin()) 
    {
      debugln(String(F("I:-Switch")));
      m_switch.setup();
    }
    if(m_switch.has_current_sensor())    debugln(String(F("I:-Current sensor")));
    if(m_switch.has_temp_sensor())       debugln(String(F("I:-Temperature sensor")));
    if(m_speaker.isValid())
    {
      debugln(String(F("I:-Buzzer")));
      m_speaker.setup();
    }

  //if(mySwitch.currentSensor().isValid())
    //int0Pointer = mySwitch.currentSensor().isrRead;
  debugln(String(F("I:INIT OK")));
  m_ledStrip.leds()[0].setColor(0,20,0);
  }

  void update()
  {
    imAlive();
  #ifdef ESP8266
    espNative::update();
  #else
    espProxy::update();
  #endif
    m_switch.update();
  }


protected:
  uint8_t m_loopCount = 0;
  EEPROMStorage         m_EEPROM;// Toda la configuracion está en el settings.h
  SSR                   m_switch;//(m_EEPROM.settings().relayPin ,m_EEPROM.settings().currentMeterPin,m_EEPROM.settings().relayMaxAmps,m_EEPROM.settings().relayDimmable,m_EEPROM.settings().relayTemperatureSensor,m_EEPROM.settings().fanPin);
  ws2812Strip           m_ledStrip;//(m_EEPROM.settings().ledStripPin, m_EEPROM.settings().ledNumber);
  piezoSpeaker          m_speaker;//(m_EEPROM.settings().piezoPin);

  void checkFactoryReset()
  {
    if(m_EEPROM.settings().factoryResetPin != -1)
    {
       pinMode(m_EEPROM.settings().factoryResetPin, INPUT);
       if(digitalRead(m_EEPROM.settings().factoryResetPin) == LOW)
       {
          debugln(String(F("I:Factory reset!")));
          m_ledStrip.setColor(10,10,0);
          m_speaker.playTone(200,100);
          m_speaker.playTone(500,100);
          m_speaker.playTone(200,100);
  
          m_EEPROM.clearEEPROM();
          m_speaker.playTone(500,100);
          m_ledStrip.leds()[0].setColor(0,0,20);
  
          m_EEPROM.refresh();
          m_speaker.playTone(500,100);
          m_ledStrip.leds()[1].setColor(0,0,20);
  
          m_EEPROM.storeSettings(m_EEPROM.settings());
          m_speaker.playTone(1000,300);
          m_ledStrip.leds()[2].setColor(0,0,20);

          softReset();
       }
    }
  }

  void softReset()
  {
    debugln(String(F("I:RESET!")));
    delay(1);
    #ifdef ESP8266
      ESP.wdtEnable(WDTO_15MS);
      while(true); // Al meter el programa en un bucle se fuerza a que el watchdog salte y haga un reset del micro
    #else
      //wdt_enable(WDTO_15MS); // En arduino parece no funcionar como se espera...WTF
      asm volatile ("  jmp 0");
    #endif
  }

  void imAlive()
  {
    if(m_loopCount == 15)
    {
      digitalWrite(m_EEPROM.settings().alivePin, !digitalRead(m_EEPROM.settings().alivePin));
      m_loopCount = 0;
      debugln(String(F("D:ImAlive!")));
    }
    else
      m_loopCount++;
  }
};

#endif
