
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
class jarvisNode : public espNative
{
public:
  jarvisNode(commModes cmode = nativeNode) : espNative(cmode , EEPROMStorage::getSettings().localPort,EEPROMStorage::getSettings().ledStripPin,EEPROMStorage::getSettings().ledStripLedNr) ,
#else 
class jarvisNode : public espProxy
{
public:
  jarvisNode() : espProxy(EEPROMStorage::getSettings().localPort,EEPROMStorage::getSettings().ledStripPin,EEPROMStorage::getSettings().ledStripLedNr) ,
#endif
    m_speaker(m_EEPROM.settings().piezoPin),
    m_dataLogger(&m_components)
  {
      if(m_ledStrip.isValid())
      {
        debugln(String(F("I:-WS2812")));
        m_ledStrip.setup();
        m_ledStrip.off();
        m_statusLed.controllerInit();
        m_components.push_back(&m_statusLed);
      }

    if(m_speaker.isValid())
        m_components.push_back(&m_speaker);
    m_id = "jarvisNode";
  }

  void setup()
  {
    Serial.begin(115200);
    yield();
    debugln(String(F("I:INIT")));
    
    if(m_EEPROM.hasSettings())
      debugln(String(F("I:-EEPROM Settings")));
    else
      debugln(String(F("I:-Hardcoded Settings")));
    
    ///////////
    //if(m_EEPROM.settings().factoryResetPin != -1) debugln(String(F("I:-Factory reset button")));
    //checkFactoryReset();
  
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
    if(this->hasEvents())
    {
        std::vector<nodeComponent::event> events = this->getEvents();
        for(int e = 0 ; e <  events.size() ; e++)
          sendEvent(m_id,events[e]);
    }

    for(int c = 0 ; c<m_components.size() ; c++ )
    {
        nodeComponent* comp = m_components[c];
        comp->update();
        yield();
        if(comp->hasEvents())
        {
            std::vector<nodeComponent::event> events = comp->getEvents();
            for(int e = 0 ; e <  events.size() ; e++)
              sendEvent(comp->id(),events[e]);
        }
    }

    if(m_pollingSensors.size())
    {
        std::vector<String> args;
        args.push_back(C_SENSORS);
        for(int c = 0 ; c<m_pollingSensors.size() ; c++ )
            for(int i  = 0 ; i < m_components.size() ; i++ )
            {
              nodeComponent* comp = m_components[i];
              if(comp->id() == m_pollingSensors[c])
              {
                args.push_back(comp->id());
                args.push_back(String(comp->readData()));
              }
            }
        send(encodeJarvisMsg(args));
    }

    m_dataLogger.update();
    delay(updateInterval);
  }

protected:
  jarvisModules         m_type;
  EEPROMStorage         m_EEPROM;// Toda la configuracion está en el settings.h
  uint8_t m_loopCount = 0;
  std::vector<String>           m_pollingSensors;
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
  

  virtual void sendEvent(String source,nodeComponent::event e)
  {
      //sobrecargar esta funcion para reaccionar a los eventos salientes.
      communicationModule::sendEvent(source,e);
  }

// Implementaciones del protocolo
  virtual void sendComponents()
  {
      sendComponent(this);
      for(int i  = 0 ; i < m_components.size() ; i++ )
        sendComponent(m_components[i]);
  }

  virtual void sendComponent(nodeComponent* comp)
  {
      std::vector<String> args;
      args.push_back(C_COMPONENT);
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

  virtual void pollSensors(int delay = -1)
  {
      if(delay != -1)
          setUpdateInterval(delay);

      for(int i  = 0 ; i < m_components.size() ; i++ )
      {
        nodeComponent* comp = m_components[i];
        if(comp->canRead())
        {
            m_pollingSensors.push_back(comp->id());
        }
      }

//      std::vector<String> args;
//      args.push_back(C_SENSORS);
//      for(int i  = 0 ; i < m_components.size() ; i++ )
//      {
//        nodeComponent* comp = m_components[i];
//        if(comp->canRead())
//        {
//          args.push_back(comp->id());
//          args.push_back(String(comp->read()));
//        }
//      }
//      args.push_back(F("FreeMem"));
//      args.push_back(String(getFreeMem()));
//      args.push_back(F("BufferSize"));
//      args.push_back(String(bufferCount()));
//      send(encodeJarvisMsg(args));
  }

  virtual void pollSensor(String id,int delay = -1)
  {
      if(delay != -1)
          setUpdateInterval(delay);
      m_pollingSensors.push_back(id);

//      std::vector<String> args;
//      args.push_back(C_SENSOR);
//      for(int i  = 0 ; i < m_components.size() ; i++ )
//      {
//        nodeComponent* comp = m_components[i];
//        if(comp->id() == id)
//        {
//          args.push_back(comp->id());
//          args.push_back(String(comp->read()));
//        }
//      }
//      send(encodeJarvisMsg(args));
  }

  virtual void stopPolling()
  {
      m_pollingSensors.clear();
  }

  virtual void processDoAction(std::vector<String> args)
  {
      if(args.size() < 2) return;
      String dest = args[0];
      args.erase(args.begin());
      jarvisActions action = jarvisActions(args[0].toInt());
      args.erase(args.begin());

      if(m_id == dest)
      {
          doAction(action,args);
          return;
      }

      for(int i = 0 ; i < m_components.size() ; i++)
      {
        nodeComponent* comp = m_components[i];
        if(comp->id() == dest)
        {
            comp->doAction(action,args);
            return;
        }
      }
  }

};

#endif
