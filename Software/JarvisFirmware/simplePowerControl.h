#ifndef spc
#define spc

#include "jarvisModule.h"
#include "relay.h"


class simplePowerControl : public jarvisModule
{
public:
    simplePowerControl(int relayPin) : jarvisModule(), m_relay(relayPin)
    {
        m_components.push_back(&m_relay);
        m_id = "simplePowerControl";
    }

    void setup()
    {
        jarvisModule::setup();
        m_statusLed.extraLedIdle();
    }

    virtual void sendEvent(String source,jarvisEvents event)
    {//sobrecargar esta funcion para reaccionar a los eventos salientes.
        if(source == "relay")
        {
            if(event == E_ENABLED)
            {
                m_speaker.beep();
                m_statusLed.extraLedIdle();
            }
            else if(event == E_DISABLED)
            {
                m_speaker.beep();
                m_statusLed.extraLedError();
            }
            else if(event == E_ACTIVATED)
            {
                m_speaker.beep();
                m_statusLed.extraLedOK();
            }else if (event == E_DEACTIVATED)
            {
                m_speaker.beep();
                m_statusLed.extraLedIdle();
            }
        }
        communicationModule::sendEvent(source,event);
    }
protected:
    relay m_relay;
};


#endif
