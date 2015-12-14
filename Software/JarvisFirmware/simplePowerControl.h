#ifndef spc
#define spc

#include "jarvisModule.h"
#include "relay.h"


class simplePowerControl : public jarvisModule
{
public:
    simplePowerControl() : jarvisModule()
    {
        m_components.push_back(&m_relay);
        m_id = "CoffeMaker";
    }

protected:
    relay m_relay;
};


#endif
