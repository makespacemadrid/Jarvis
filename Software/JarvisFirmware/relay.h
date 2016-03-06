#ifndef RELAY_H
#define RELAY_H

#include "nodeComponent.h"

class relay : public nodeComponent
{
  public:
    relay(int pin = -1) : nodeComponent(pin)
    {
      m_id = "relay";
      m_actions.push_back(A_ACTIVATE);
      m_actions.push_back(A_DEACTIVATE);
      m_actions.push_back(A_TOGGLE);
      m_actions.push_back(A_READ_RAW);
      m_capableEvents.push_back(E_ACTIVATED);
      m_capableEvents.push_back(E_DEACTIVATED);
      m_capableEvents.push_back(E_DATA_READ);
    }

    bool canRead()
    {
        return true;
    }

    void setInvertedLogic(bool inverted)
    {
        m_invertedLogic = inverted;
    }

    uint16_t readRaw()
    {
        return m_status;
    }

    float readData()
    {
        return m_status;
    }

    virtual void activate()
    {
        if(!m_status)
          setStatus(true);
    }

    virtual void deactivate()
    {
        if(m_status)
          setStatus(false);
    }

    virtual void toggle()
    {
      setStatus(!m_status);
    }
    
    bool getStatus()
    {
        return m_status;
    }

    void setStatus(bool status)
    {
        if(!isValid() || !m_enabled) return;
        m_status = status;

        if(m_invertedLogic)
            digitalWrite(m_pin,!m_status);
        else
            digitalWrite(m_pin,m_status);

        if(m_status)
            m_events.push_back(E_ACTIVATED);
        else
            m_events.push_back(E_DEACTIVATED);
    }

    virtual bool isValid()
    {
        return m_pin != -1;
    }

    virtual void setup()
    {
        if(!isValid()) return;
        pinMode(m_pin, OUTPUT);
    }

  protected:

    bool m_status = false;
    bool m_invertedLogic = false;
};


#endif // RELAY_H
