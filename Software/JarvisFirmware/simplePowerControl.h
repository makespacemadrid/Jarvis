#ifndef spc
#define spc

#include "jarvisNode.h"
#include "relay.h"


class simplePowerControl : public jarvisNode
{
public:
    simplePowerControl(int relayPin) : jarvisNode(), m_relay(relayPin)
    {
        m_components.push_back(&m_relay);
        m_id = "simplePowerControl";
    }

    void setup()
    {
        jarvisNode::setup();
        if(m_relay.getStatus())
          m_statusLed.extraLedOK();
        else
          m_statusLed.extraLedIdle();
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
        }
        jarvisNode::sendEvent(source,e);
    }
protected:
    relay m_relay;
};


#endif
