#ifndef BUTTONS_H
#define BUTTONS_H

#include "nodeComponent.h"

class button : public nodeComponent
{
public:
  button(int pin = -1) : nodeComponent(pin), m_invertedLogic(false), m_statusChanged(false)
  {
      m_capableEvents.push_back(E_ACTIVATED);
      m_capableEvents.push_back(E_DEACTIVATED);
      m_actions.push_back(A_READ_RAW);
      m_capableEvents.push_back(E_RAW_READ);
      m_id = F("switch");
  }
  
  bool     canRead()  {return true;}

  virtual uint16_t readRaw()
  {
      if((!isValid()) || (!m_enabled)) return 0;
      if(m_invertedLogic)
        return !digitalRead(m_pin);
      else
        return digitalRead(m_pin);
  }

  virtual float     read()
  {
      return readRaw();
  }

  virtual void setup()
  {
      if(!isValid()) return;
      pinMode(m_pin, INPUT);
      delay(5);
      m_lastStatus = readRaw();
  }

  bool status()
  {
      return m_lastStatus;
  }


  virtual void update()
  {//añadir mecanismo de debounce?
    if(!m_enabled) return;
      bool status = readRaw();
      if(status != m_lastStatus)
      {
          m_lastStatus = status;
          if(status)
              m_events.push_back(E_ACTIVATED);
          else
              m_events.push_back(E_DEACTIVATED);
      }
  }

  void setInvertedLogic(bool inverted) {m_invertedLogic = inverted;}

protected:
  bool m_invertedLogic;
  bool m_lastStatus;
  bool m_statusChanged;

};

#endif // BUTTONS_H
