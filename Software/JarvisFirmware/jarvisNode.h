
#ifndef JMODULE
#define JMODULE

#include "communication.h"
#include "ws2812led.h"
#include "piezoSpeaker.h"
#include "communication.h"
#include "settings.h"
#include "nodeComponent.h"
#include "dataLogger.h"



class jarvisNode : public espNative
{
public:
  jarvisNode(EEPROMStorage* settings) :
   espNative(settings) ,
    m_speaker(m_eeprom->settings().piezoPin),
    m_dataLogger(&m_components)
  {
    if(m_ledStrip.isValid())
    {
        m_components.push_back(&m_statusLed);
    }

    if(m_speaker.isValid())
    {
        m_components.push_back(&m_speaker);
        debug(String(F("I:-Piezo on: ")));
        debugln(m_speaker.pinNr());
        m_speaker.setup();
    }
    updateInterval = m_eeprom->settings().updateInterval;
    if(m_eeprom->settings().alivePin != -1)
    {
        pinMode(m_eeprom->settings().alivePin, OUTPUT);
    }

  }

  void setup()
  {
    debugln(String(F("I:SETUP:")));
    
    if(m_eeprom->hasSettings())
      debugln(String(F("I:-EEPROM Settings")));
    else
      debugln(String(F("I:-Hardcoded Settings")));
    checkFactoryReset();
  
  #ifdef ESP8266
    debugln(String(F("I:-ESP8266")));
    espNative::setup();
  #else
    debugln(String(F("I:-Arduino")));
    espProxy::setup();
  #endif

    yield();

    for(int c = 0 ; c < m_components.size() ; c++ )
    {
        m_components[c]->setup();
        #ifdef VERBOSE_DEBUG
        debugln(m_components[c]->id());
        #endif
    }

    m_dataLogger.setup();

    debugln(String(F("I:INIT OK")));
    m_statusLed.controllerOK();
    m_initDone = true;
    yield();

//Resetear el Serie, aveces se queda pillado despues de hacer el setup() y no recibe
    Serial.begin(115200);
  }

  void update()
  {
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
          sendEvent(m_eeprom->settings().id,events[e]);
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
        args.push_back(F("Freemem:"));
        args.push_back(String(getFreeMem()));
        //args.push_back(F("BufferLength:"));
        //args.push_back(String(bufferCount()));
        send(encodeJarvisMsg(args));
    }

    m_dataLogger.update();
    delay(updateInterval);
  }

protected:
  jarvisModules                 m_type;
  float                         m_loopCount = 0;
  std::vector<String>           m_pollingSensors;
  std::vector<nodeComponent*>   m_components;
  piezoSpeaker                  m_speaker;   //(m_EEPROM.settings().piezoPin);
  dataLogger                    m_dataLogger;

  virtual bool checkFactoryReset()
  {
      return false; // Proveer de un metodo por defecto para hacer el reset!
  }

  void factoryReset()
  {
    debugln(String(F("I:Factory reset!")));
    m_ledStrip.setColor(10,10,0);
    m_speaker.playTone(200,100);
    m_speaker.playTone(500,100);
    m_speaker.playTone(200,100);

    m_eeprom->clearEEPROM();
    m_speaker.playTone(500,100);
    m_ledStrip.leds()[0].setColor(0,0,20);

    m_eeprom->reload();
    m_speaker.playTone(500,100);
    m_ledStrip.leds()[1].setColor(0,0,20);

    m_eeprom->storeSettings();
    m_speaker.playTone(1000,300);
    m_ledStrip.leds()[2].setColor(0,0,20);

    softReset();
  }

  void imAlive()
  {
    if(m_loopCount > 5)
    {
      if(m_eeprom->settings().alivePin != -1)   digitalWrite(m_eeprom->settings().alivePin, !digitalRead(m_eeprom->settings().alivePin));
      m_loopCount = 0;
      m_statusLed.controllerOK();
      debugln("\nD:ImAlive!");

      debug("\tFreeMem:");
      debug(getFreeMem());
      debug("\tIP:");
      debug(localIP());
      debug("\twifiStatus:");
      debug(connectionStatus());
      debug("\tEstablished conns:");
      debug((int)m_validatedConns.size());
      debug("\tIncoming conns:");
      debugln((int)m_validatingConns.size());

      debug("\tComponents:");
      debug((int)m_components.size());
      debug("\tUpdateInterval:");
      debug(updateInterval);
      debug("\tSerialBuffer:");
      debug((int)m_serialBuffer.length());
      debug("\tWifiBuffer:");
      debugln((int)m_rxBuffer.length());
    }
    else
      m_loopCount += (updateInterval+10)/1000.0f;
    yield();
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
  }

  virtual void pollSensor(String id,int delay = -1)
  {
      if(delay != -1)
          setUpdateInterval(delay);
      m_pollingSensors.push_back(id);
  }

  virtual void stopPolling()
  {
      m_pollingSensors.clear();
  }

  virtual void processDoAction(std::vector<String>& args)
  {
      if(args.size() < 2) return;
//      Serial.print("N-process doAction, fm:");
//      Serial.println(getFreeMem());
      String dest = args[0];
      args.erase(args.begin());
      jarvisActions action = jarvisActions(args[0].toInt());
      args.erase(args.begin());
      String id = m_eeprom->settings().id;
      if(id == dest)
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
