#ifndef RELAY_H
#define RELAY_H

#include "nodeComponent.h"

class relay : public nodeComponent
{
  public:
    relay(int pin = -1) : nodeComponent(pin)
    {
      m_id = "relay";
    }

    virtual bool isValid()
    {

    }

    virtual void update()
    {

    }

  protected:

};


#endif // RELAY_H
