
#ifndef JMODULE
#define JMODULE

#include "communication.h"
#include "ws2812led.h"
#include "ssr.h"
#include "piezoSpeaker.h"
#include "communication.h"
#include "settings.h"

#ifdef ESP8266
class jarvisModule : public espNative
{
public:
  jarvisModule() : espNative(EEPROMStorage::getSettings().localPort,m_EEPROM.settings().bridgeMode) ,
#else 
class jarvisModule : public espProxy
{
public:
  jarvisModule() : espProxy(EEPROMStorage::getSettings().localPort,false) ,
#endif

//inicializacion de los componentes en el constructor.
  m_switch(m_EEPROM.settings().relayPin ,m_EEPROM.settings().currentMeterPin,m_EEPROM.settings().relayMaxAmps,m_EEPROM.settings().relayDimmable,m_EEPROM.settings().relayTemperatureSensor,m_EEPROM.settings().fanPin) ,
  m_ledStrip(m_EEPROM.settings().ledStripPin, m_EEPROM.settings().ledNumber) ,
  m_speaker(m_EEPROM.settings().piezoPin)
  {  };

  void setup()
  {
    Serial.begin(115200);
    delay(10);
    debugln(String(F("I:INIT")));
    
    if(m_EEPROM.hasSettings())
      debugln(String(F("I:-EEPROM Settings")));
    else
      debugln(String(F("I:-Hardcoded Settings")));

    if(m_EEPROM.settings().factoryResetPin != -1) debugln(String(F("I:-Factory reset button")));
      
    checkFactoryReset();
  #ifdef ESP8266
    debugln(String(F("I:-ESP8266")));
    espNative::setup();
  #else
    debugln(String(F("I:-Arduino")));
    espProxy::setup();
  #endif
  
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
    checkFreeMem();
    imAlive();
  #ifdef ESP8266
    espNative::update();
  #else
    espProxy::update();
  #endif
    m_switch.update();
    delay(100);
  }


protected:
  EEPROMStorage         m_EEPROM;// Toda la configuracion está en el settings.h
  uint8_t m_loopCount = 0;
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

  virtual void processNodeMsg(std::vector<String>& args)
  {
    if((m_bridge) || (args.size()<=0)) return; // En modo bridge no se procesan los mensajes de nodo(solo esp).
    
    if     (args[0] == C_SETAP)
    {
      if(args.size() != 3) return; //Guardar los parametros a la EEPROM!
      setAP(args[1],args[2]);
    } 
    else if(args[0] == C_SETCLIENT)
    {
      if(args.size() != 3) return;
      setStation(args[1],args[2]);
    } 
    else if(args[0] == C_RESET)
    {
      softReset();
    }
    else if(args[0] == C_PING)
    {
      std::vector<String> args;
      args.push_back(C_PONG);
      send(encodeNodeMsg(args));
    }    
  }

  void checkFreeMem()
  {
    #ifndef ESP8266
    int freem = freeMemory();
    if(freem < 400)
    {
      debug(F("D:LOW RAM :"));
      debug(freem);
      debugln(F("bytes. Shit can happen anytime!"));
    }
    #endif
  }

};

#endif
