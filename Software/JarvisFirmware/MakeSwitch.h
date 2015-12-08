#ifndef MAKESWITCH_H
#define MAKESWITCH_H

#include "jarvisModule.h"


class simpleSwitch : public jarvisModule
{
public:
    simpleSwitch(int pin =16) : jarvisModule(), m_switch(pin)
    {
        m_sensors.push_back(&m_switch);
    }

    virtual void setup()
    {
      jarvisModule::setup();
    }

protected:
    button m_switch;

};


class makeSwitch : public simpleSwitch
{
public:
    makeSwitch() : simpleSwitch(), m_makeLed(&m_ledStrip), m_offLed(&m_ledStrip) , m_dhtSensor(5)
    {//inicializar los componentes extra y anadirlos a los arrays de sensores y actuadores.
        m_makeLed.addLed(3,6);
        m_offLed.addLed(9,6);
        m_actuators.push_back(&m_makeLed);
        m_actuators.push_back(&m_offLed);
        m_sensors.push_back(m_dhtSensor.temperatureSensor());
        m_sensors.push_back(m_dhtSensor.humiditySensor());
    }

    virtual void sendEvent(String source,jarvisEvents event)
    {//sobrecargar esta funcion para reaccionar a los eventos salientes.
        if(source == "switch")
        {
            if(event == E_ACTIVATED)
            {
                m_makeLed.setColor(0,200,0);
                m_offLed.setColor(0,0,0);
            }else if (event == E_DEACTIVATED)
            {
                m_makeLed.setColor(0,0,0);
                m_offLed.setColor(200,0,0);
            }
        }
        communicationModule::sendEvent(source,event);
    }

    virtual void setup()
    {
      simpleSwitch::setup();
      m_makeLed.setColor(200,200,200);
      m_makeLed.glow();
      m_offLed.setColor(200,200,200);
      m_offLed.glow();
    }
protected:
    ledBar m_makeLed;
    ledBar m_offLed;
    dhtSensor  m_dhtSensor;
};

#endif // MAKESWITCH_H
