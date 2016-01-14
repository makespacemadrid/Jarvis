#ifndef MAKESWITCH_H
#define MAKESWITCH_H

#include "jarvisNode.h"
#include "buttons.h"
#include "temperatureSensor.h"
class simpleSwitch : public jarvisNode
{
public:
    simpleSwitch(int pin =16) : jarvisNode(), m_switch(pin)
    {
        m_components.push_back(&m_switch);
        m_id = "simpleSwitch";

    }

    virtual void setup()
    {
      jarvisNode::setup();
    }

protected:
    button m_switch;

};


class makeSwitch : public simpleSwitch
{
public:
    enum switchStatus
    {
        Off,
        PowerOnRequested,
        PoweringOn,
        On,
        ShutDownRequested,
        ShuttingDown
    };

    makeSwitch() : simpleSwitch(), m_makeLed(&m_ledStrip), m_offLed(&m_ledStrip) , m_dhtSensor(5)
    {//inicializar los componentes extra y anadirlos a los arrays de sensores y actuadores.
        m_makeLed.addLed(3,6);
        m_makeLed.setId("makeLed");
        m_offLed.addLed(9,6);
        m_offLed.setId("offLed");
        m_components.push_back(&m_makeLed);
        m_components.push_back(&m_offLed);
        m_components.push_back(m_dhtSensor.temperatureSensor());
        m_components.push_back(m_dhtSensor.humiditySensor());

        addCapableEvent(E_GLOBAL_POWERON);
        addCapableEvent(E_GLOBAL_SHUTDOWN);
        m_id = "makeSwitch";
        disable();
    }

    virtual void sendEvent(String source,nodeComponent::event e)
    {//sobrecargar esta funcion para reaccionar a los eventos salientes.
        if(source == "switch")
        {
            if(!m_enabled)
            {
                jarvisNode::sendEvent(source,e);
                return;
            }

            if(e.jevent == E_ACTIVATED)
            {
                m_speaker.beep();
                startPowerOn();
            }else if (e.jevent == E_DEACTIVATED)
            {
                m_speaker.beep();
                startShutDown();
            }
        }
        jarvisNode::sendEvent(source,e);
    }

    virtual void setup()
    {
      simpleSwitch::setup();
      m_makeLed.setColor(250,250,0);
      m_offLed.setColor(250,250,0);
      m_makeLed.glow();
      m_offLed.glow();
    }

    void wifiConnected()
    {
        m_makeLed.setColor(0,0,255);
        m_offLed.setColor(0,0,255);
    }

    void wifiDisConnected()
    {
        m_makeLed.setColor(255,0,0);
        m_offLed.setColor(255,0,0);
    }

    void jarvisConnected()
    {
        enable();
        if(m_status == On)
        {
            m_makeLed.setColor(0,250,0);
            m_offLed.off();
        }else {
            m_makeLed.off();
            m_makeLed.setColor(0,0,250);
            m_makeLed.glow();
            m_offLed.setColor(250,0,0);
        }
    }

    void jarvisDisConnected()
    {
        wifiConnected();
        disable();
    }

    virtual void update()
    {
      jarvisNode::update();

      if(!m_enabled) return;
      
      if(m_status == PowerOnRequested)
      {
        m_activationCounter += (updateInterval+1.0)/1000.0f;
        if(m_activationCounter >= 5.0f)
        {
            m_status = PoweringOn;
            m_activationCounter = 0.0;
            addEvent(E_GLOBAL_POWERON);
        }
      }else if(m_status == ShutDownRequested)
      {
          m_deActivationCounter += (updateInterval+1.0)/1000.0f;
          if(m_deActivationCounter >= 5.0f)
          {
              m_status = ShuttingDown;
              m_deActivationCounter = 0.0;
              addEvent(E_GLOBAL_SHUTDOWN);
          }
      }else if(m_status == PoweringOn)
      {
          m_status = On;
          m_makeLed.setColor(0,250,0);
          m_offLed.off();
      }else if(m_status == ShuttingDown)
      {
          m_status = Off;
          m_makeLed.off();
          m_makeLed.setColor(0,0,250);
          m_makeLed.glow();
          m_offLed.setColor(250,0,0);
      }
    }

    void startPowerOn()
    {
        if(m_status == Off)
        {
            m_activationCounter = 0;
            m_status = PowerOnRequested;
            m_makeLed.setColor(250,250,0);
            m_makeLed.glow();
            m_offLed.setColor(250,0,0);
        }else if(m_status == ShutDownRequested)
        {
            m_deActivationCounter = 0;
            m_status = On;
            m_makeLed.setColor(0,250,0);
            m_offLed.off();
        }
    }

    void startShutDown()
    {
        if(m_status == On)
        {
            m_deActivationCounter = 0;
            m_status = ShutDownRequested;
            m_makeLed.setColor(250,250,0);
            m_makeLed.glow();
            m_offLed.setColor(250,0,0);
            m_offLed.glow();
        }else if(m_status == PowerOnRequested)
        {
            m_activationCounter = 0;
            m_status = Off;
            m_makeLed.off();
            m_offLed.setColor(250,0,0);
            m_makeLed.glow();
        }
    }

protected:
    ledBar       m_makeLed;
    ledBar       m_offLed;
    dhtSensor    m_dhtSensor;
    switchStatus m_status              = Off;
    float       m_activationCounter   = 0.0 ;
    float       m_deActivationCounter = 0.0 ;
};

#endif // MAKESWITCH_H
