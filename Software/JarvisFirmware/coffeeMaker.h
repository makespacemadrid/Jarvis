#ifndef COFFEEMAKER_H
#define COFFEEMAKER_H

#include "simplePowerControl.h"
#include "temperatureSensor.h"


class coffeeMaker : public simplePowerControl
{
public:
    coffeeMaker(EEPROMStorage* settings) :
        simplePowerControl(settings),
        m_leds(&m_ledStrip),
        m_heating(false),
        m_tempSensor(m_eeprom->settings().tempSensorPins[0])
    {
        m_relay.setup();
        m_button.setId("button");
        heat();//Precalentar;
        //m_relay.setInvertedLogic(true);
        m_timeout = 0.0;
        m_leds.addLed(3,47);
        m_components.push_back(&m_leds);
        m_components.push_back(&m_tempSensor);

        m_actions.push_back(A_MAKE_COFFEE);
        m_actions.push_back(A_ACTIVATE);
        m_actions.push_back(A_DEACTIVATE);
        m_capableEvents.push_back(E_ACTIVATED);
        m_capableEvents.push_back(E_DEACTIVATED);
        m_capableEvents.push_back(E_COFFEE_MAKING);
        m_capableEvents.push_back(E_COFFEE_MADE);

        m_leds.cylon();
    }

    void setup()
    {
        simplePowerControl::setup();
    }

    void update()
    {
        simplePowerControl::update();

        if(m_timeout < 300.0)
        {
            m_timeout += (updateInterval+1)/1000.0f;
        }else
        {
            m_timeout = 0;
            if(m_relay.getStatus())
            {
                m_relay.deactivate();
                m_leds.off();
            }
        }

        if(m_heating)
        {
            if(isHot())
            {
                m_heating = false;
                m_leds.setColor(0,150,0);
            }
        }
        if(m_makingCoffee)
        {
            if(m_timeout > 60)
            {
                stop();
            }
        }
    }

    virtual void sendEvent(String source,nodeComponent::event e)
    {//sobrecargar esta funcion para reaccionar a los eventos salientes.
        if(source == "relay")
        {
            if(e.jevent == E_ENABLED)
            {
                m_speaker.beep();
                m_statusLed.extraLedIdle();
            }
            else if(e.jevent == E_DISABLED)
            {
                m_speaker.beep();
                m_statusLed.extraLedError();
            }
            else if(e.jevent == E_ACTIVATED)
            {
                m_speaker.beep();
                m_statusLed.extraLedOK();
            }else if (e.jevent == E_DEACTIVATED)
            {
                m_speaker.beep();
                m_statusLed.extraLedIdle();
            }
        } else if(source == "button")
        {
            if(e.jevent == E_ACTIVATED)
            {
                if(m_makingCoffee)
                    stop();
                else
                    makeCoffee();
            }
        }
        jarvisNode::sendEvent(source,e);
    }

    void activate()
    {
        heat();
    }

    void deactivate()
    {
        m_heating = false;
        m_relay.deactivate();
        m_leds.deactivate();
        m_events.push_back(E_DEACTIVATED);
    }

    void heat()
    {
        m_leds.setColor(150,0,0);
        m_leds.glow();
        m_relay.activate();
        m_timeout = 0;
        m_heating = true;
        m_events.push_back(E_ACTIVATED);
    }

    bool isHot()
    {
        //return m_timeout > 10;
        return m_tempSensor.readRaw() <= 150;
    }

    void makeCoffee()
    {
        m_makingCoffee = true;
        m_events.push_back(E_COFFEE_MAKING);
        if(!isHot()) heat();
        while(!isHot() && m_makingCoffee)
        {
            update();
        }
        m_leds.setColor(100,100,0);
        m_leds.glow();
    }

    void stop()
    {
        m_makingCoffee = false;
        m_leds.setColor(0,150,0);
        deactivate();
    }

protected:
    ntc100kTemperatureSensor    m_tempSensor;
    float                       m_timeout;
    ledBar                      m_leds;
    bool                        m_heating;
    bool                        m_makingCoffee;

};


#endif // COFFEEMAKER_H
