
#ifndef JMODULE
#define JMODULE

#include "communication.h"
#include "ws2812led.h"
#include "ssr.h"
#include "piezoSpeaker.h"
#include "communication.h"
#include "settings.h"

#include "sensors.h"
#include "actuators.h"

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

//inicializacion de los componentes en el constructor.
//  m_switch(m_EEPROM.settings().relayPin ,m_EEPROM.settings().currentMeterPin,m_EEPROM.settings().relayMaxAmps,m_EEPROM.settings().relayDimmable,m_EEPROM.settings().relayTemperatureSensor,m_EEPROM.settings().fanPin) ,
    m_speaker(m_EEPROM.settings().piezoPin)
  {
    if(m_ledStrip.isValid())
    {
        m_actuators.push_back(&m_statusLed);
    }

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

    for(int s = 0 ; s<m_sensors.size() ; s++ )
    {
        m_sensors[s]->setup();
    }
    for(int a = 0 ; a<m_actuators.size() ; a++ )
    {
        m_actuators[a]->setup();
    }

    if(m_EEPROM.settings().alivePin != -1)
    {
      pinMode(m_EEPROM.settings().alivePin,OUTPUT);
      debugln(String(F("I:-Alive led")));
    }
    
//    if(m_switch.has_switch_pin()) 
//    {
//      debugln(String(F("I:-Switch")));
//      m_switch.setup();
//    }
//    if(m_switch.has_current_sensor())    debugln(String(F("I:-Current sensor")));
//    if(m_switch.has_temp_sensor())       debugln(String(F("I:-Temperature sensor")));
    if(m_speaker.isValid())
    {
      debugln(String(F("I:-Buzzer")));
      m_speaker.setup();
    }

  //if(mySwitch.currentSensor().isValid())
    //int0Pointer = mySwitch.currentSensor().isrRead;
  debugln(String(F("I:INIT OK")));
  m_statusLed.controllerOK();  
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
    for(int s = 0 ; s<m_sensors.size() ; s++ )
    {
        sensor* sen = m_sensors[s];
        sen->update();
        if(sen->hasEvents())
        {
            std::vector<jarvisEvents> events = sen->getEvents();
            for(int e = 0 ; e <  events.size() ; e++)
              sendEvent(sen->id(),events[e]);
        }
    }
        

    for(int s = 0 ; s<m_actuators.size() ; s++ )
    {
        actuators* act = m_actuators[s];
        act->update();
        if(act->hasEvents())
        {
          std::vector<jarvisEvents> events = act->getEvents();
          for(int e = 0 ; e <  events.size() ; e++)
            sendEvent(act->id(),events[e]);
        }
    }
    delay(10);
  }


protected:
  jarvisModules         m_type;
  EEPROMStorage         m_EEPROM;// Toda la configuracion está en el settings.h
  uint8_t m_loopCount = 0;

  std::vector<actuators*>    m_actuators;
  std::vector<sensor*>       m_sensors;

  //SSR                   m_switch;//(m_EEPROM.settings().relayPin ,m_EEPROM.settings().currentMeterPin,m_EEPROM.settings().relayMaxAmps,m_EEPROM.settings().relayDimmable,m_EEPROM.settings().relayTemperatureSensor,m_EEPROM.settings().fanPin);
  piezoSpeaker          m_speaker;   //(m_EEPROM.settings().piezoPin);
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
    if(m_loopCount == 150)
    {
      digitalWrite(m_EEPROM.settings().alivePin, !digitalRead(m_EEPROM.settings().alivePin));
      m_loopCount = 0;
      debugln(String(F("D:ImAlive!")));
    }
    else
      m_loopCount++;
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


  virtual void sendEvent(String source,jarvisEvents event)
  {
      //sobrecargar esta funcion para reaccionar a los eventos salientes.
      communicationModule::sendEvent(source,event);
  }

// Implementaciones del protocolo
  virtual void sendComponents()
  {
      for(int i  = 0 ; i < m_sensors.size() ; i++ )
      {
        std::vector<String> args;
        args.push_back(C_COMPONENT);
        sensor* sen = m_sensors[i];
        args.push_back(sen->id());
        args.push_back(E_EVENT);
        for (int i = 0 ; i < sen->capableEvents().size() ; i++)
        {
            args.push_back(String(sen->capableEvents()[i]));
        }
            send(encodeJarvisMsg(args));
      }

      for(int i  = 0 ; i < m_actuators.size() ; i++ )
      {
        std::vector<String> args;
        args.push_back(C_COMPONENT);
        actuators* act = m_actuators[i];
        args.push_back(act->id());
        args.push_back(E_EVENT);
        for (int i = 0 ; i < act->capableEvents().size() ; i++)
        {
            args.push_back(String(act->capableEvents()[i]));
        }
        args.push_back(E_ACTION);
        for (int i = 0 ; i < act->getActions().size() ; i++)
        {
            args.push_back(String(act->getActions()[i]));
        }
            send(encodeJarvisMsg(args));
      }
  }

  virtual void sendSensors()
  {
      std::vector<String> args;
      args.push_back(C_SENSORS);
      for(int i  = 0 ; i < m_sensors.size() ; i++ )
      {
        sensor* sen = m_sensors[i];
        args.push_back(sen->id());
        args.push_back(String(sen->read()));
      }
      send(encodeJarvisMsg(args));
  }

  virtual void sendSensor(String id)
  {
      std::vector<String> args;
      args.push_back(C_SENSOR);
      for(int i  = 0 ; i < m_sensors.size() ; i++ )
      {
        sensor* sen = m_sensors[i];
        if(sen->id() == id)
        {
          args.push_back(sen->id());
          args.push_back(String(sen->read()));
        }
      }
      send(encodeJarvisMsg(args));
  }

  virtual void doAction(std::vector<String> args)
  {
      if(args.size() < 2) return;
      for(int i = 0 ; i < m_actuators.size() ; i++)
      {
        actuators* act = m_actuators[i];
        std::vector<String> arguments = args;
        if(act->id() == arguments[0])
        {
            arguments.erase(arguments.begin());
            jarvisActions action = jarvisActions(arguments[0].toInt());
            Serial.print("Action:");
            Serial.println(action);
            arguments.erase(arguments.begin());
            act->doAction(action,arguments);
        }
      }
  }

};

#endif
