#ifndef COFFEEMAKER_H
#define COFFEEMAKER_H

#include "simplePowerControl.h"

class coffeeMaker : public simplePowerControl
{
public:
    coffeeMaker(int relayPin) : simplePowerControl(relayPin)
    {
        //m_relay.setInvertedLogic(true);
        m_timeout = 0.0;
        m_relay.activate();
        m_id = "coffeeMaker";
    }

    void setup()
    {
        simplePowerControl::setup();
    }

    void update()
    {
        simplePowerControl::update();
        if(m_timeout < 600.0)
        {
            m_timeout += updateInterval/1000.0f;
            //Serial.println(m_timeout);
        }else
        {
            m_timeout = 0;
            if(m_relay.getStatus())
                m_relay.deactivate();
        }

    }

protected:
    float m_timeout;
};


#endif // COFFEEMAKER_H
