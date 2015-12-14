
#ifndef JMODULE
#define JMODULE

#include "communication.h"
#include "ws2812led.h"
#include "piezoSpeaker.h"
#include "communication.h"
#include "settings.h"
#include "nodeComponent.h"
#include "dataLogger.h"


#ifdef ESP8266
class jarvisModule : public espNative
{
public:
  jarvisModule(commModes cmode = nativeNode) : espNative(cmode , EEPROMStorage::getSettings().localPort,EEPROMStorage::getSettings().ledStripPin) ,
#else 
class jarvisModule : public espProxy
{
public:
  jarvisModule() : espProxy(EEPROMStorage::getSettings().localPort,EEPROMStorage::getSettings().ledStripPin) ,
#endif
    m_speaker(m_EEPROM.settings().piezoPin),
    m_dataLogger(&m_components)
  {
    if(m_ledStrip.isValid())
        m_components.push_back(&m_statusLed);
    if(m_speaker.isValid())
        m_components.push_back(&m_speaker);
  }

  void setup()
  {
    Serial.begin(115200);
    delay(10);
    debugln(String(F("I:INIT")));
    
    if(m_EEPROM.hasSettings())
      debugln(String(F("I:-EEPROM Settings")));
    else
      debugln(String(F("I:-Hardcoded Settings")));
    
    ///////////
    //if(m_EEPROM.settings().factoryResetPin != -1) debugln(String(F("I:-Factory reset button")));
    //checkFactoryReset();

    if(m_ledStrip.isValid())
    {
      debugln(String(F("I:-WS2812")));
      m_ledStrip.setup();
      m_ledStrip.off();
      m_statusLed.controllerInit();
    }
  
  #ifdef ESP8266
    debugln(String(F("I:-ESP8266")));
    espNative::setup();
  #else
    debugln(String(F("I:-Arduino")));
    espProxy::setup();
  #endif

    for(int c = 0 ; c < m_components.size() ; c++ )
    {
        m_components[c]->setup();
        #ifdef VERBOSE_DEBUG
        debugln(m_components[c]->id());
        #endif
    }

    if(m_EEPROM.settings().alivePin != -1)
    {
      pinMode(m_EEPROM.settings().alivePin,OUTPUT);
      debugln(String(F("I:-Alive led")));
    }

  m_dataLogger.setup();
  debugln(String(F("I:INIT OK")));
  m_statusLed.controllerOK();  
  }

  void update()
  {
//    checkFreeMem();
    imAlive();
  #ifdef ESP8266
    espNative::update();
  #else
    espProxy::update();
  #endif
    for(int c = 0 ; c<m_components.size() ; c++ )
    {
        nodeComponent* comp = m_components[c];
        comp->update();
        if(comp->hasEvents())
        {
            std::vector<jarvisEvents> events = comp->getEvents();
            for(int e = 0 ; e <  events.size() ; e++)
              sendEvent(comp->id(),events[e]);
        }
    }
    m_dataLogger.update();
    delay(updateInterval);
  }


protected:
  jarvisModules         m_type;
  EEPROMStorage         m_EEPROM;// Toda la configuracion está en el settings.h
  uint8_t m_loopCount = 0;

  std::vector<nodeComponent*>   m_components;
  piezoSpeaker                  m_speaker;   //(m_EEPROM.settings().piezoPin);
  dataLogger                    m_dataLogger;

  void checkFactoryReset()
  {
    if(m_EEPROM.settings().factoryResetPin != -1)
    {
       pinMode(m_EEPROM.settings().factoryResetPin, INPUT);
       if(digitalRead(m_EEPROM.settings().factoryResetPin) == LOW)
       {
          m_ledStrip.setup();
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
    if(m_loopCount >= (1000/updateInterval))
    {
      digitalWrite(m_EEPROM.settings().alivePin, !digitalRead(m_EEPROM.settings().alivePin));
      m_loopCount = 0;
      m_statusLed.controllerOK();
      debugln(String(F("D:ImAlive!")));
    }
    else
      m_loopCount++;
  }
  



  virtual void sendEvent(String source,jarvisEvents event)
  {
      //sobrecargar esta funcion para reaccionar a los eventos salientes.
      communicationModule::sendEvent(source,event);
  }

// Implementaciones del protocolo
  virtual void sendComponents()
  {
      for(int i  = 0 ; i < m_components.size() ; i++ )
      {
        std::vector<String> args;
        args.push_back(C_COMPONENT);
        nodeComponent* comp = m_components[i];
        args.push_back(comp->id());
        args.push_back(E_EVENT);
        for (int i = 0 ; i < comp->capableEvents().size() ; i++)
        {
            args.push_back(String(comp->capableEvents()[i]));
        }
        args.push_back(E_ACTION);
        for (int i = 0 ; i < comp->getActions().size() ; i++)
        {
            args.push_back(String(comp->getActions()[i]));
        }
            send(encodeJarvisMsg(args));
      }
  }

  virtual void sendSensors()
  {
      std::vector<String> args;
      args.push_back(C_SENSORS);
      for(int i  = 0 ; i < m_components.size() ; i++ )
      {
        nodeComponent* comp = m_components[i];
        if(comp->canRead())
        {
          args.push_back(comp->id());
          args.push_back(String(comp->read()));  
        }
      }
      args.push_back(F("FreeMem"));
      args.push_back(String(getFreeMem()));
      send(encodeJarvisMsg(args));
  }

  virtual void sendSensor(String id)
  {
      std::vector<String> args;
      args.push_back(C_SENSOR);
      for(int i  = 0 ; i < m_components.size() ; i++ )
      {
        nodeComponent* comp = m_components[i];
        if(comp->id() == id)
        {
          args.push_back(comp->id());
          args.push_back(String(comp->read()));
        }
      }
      send(encodeJarvisMsg(args));
  }

  virtual void doAction(std::vector<String> args)
  {
      if(args.size() < 2) return;
      for(int i = 0 ; i < m_components.size() ; i++)
      {
        nodeComponent* comp = m_components[i];
        std::vector<String> arguments = args;
        if(comp->id() == arguments[0])
        {
            arguments.erase(arguments.begin());
            jarvisActions action = jarvisActions(arguments[0].toInt());
            arguments.erase(arguments.begin());
            comp->doAction(action,arguments);
        }
      }
  }

};

#endif
