#ifndef LEDPANELNODE_H
#define LEDPANELNODE_H

#include "jarvisNode.h"
#include "ledMatrixIcons.h"
#include "temperatureSensor.h"
#include "buttons.h"

class ledPanelNode : public jarvisNode
{
public:
    ledPanelNode() : jarvisNode() ,m_ledMatrix(0,30,10,&m_ledStrip,false,true), m_dhtSensor(12)
      , m_activateBtn(13), m_deactivateBtn(15), m_potenciometer(A0)
    {
        m_components.push_back(&m_ledMatrix);
        m_components.push_back(m_dhtSensor.temperatureSensor());
        m_components.push_back(m_dhtSensor.humiditySensor());
        m_components.push_back(&m_activateBtn);
        m_activateBtn.setId("ActivateButton");
        m_components.push_back((&m_deactivateBtn));
        m_deactivateBtn.setId("DeactivateButton");
        m_components.push_back(&m_potenciometer);

        m_id = "ledPanel";
        m_actions.push_back(A_ACTIVATE);
        m_actions.push_back(A_DEACTIVATE);
        m_actions.push_back(A_DIMM);
        m_actions.push_back(A_DISPLAY);

        m_capableEvents.push_back(E_ACTIVATED);
        m_capableEvents.push_back(E_DEACTIVATED);
        m_statusLed.disable();
    }

    void activate()
    {
        m_ledMatrix.activate();
    }

    void deactivate()
    {
        m_ledMatrix.deactivate();
        m_events.push_back(E_DEACTIVATED);
    }

    void dimm(uint8_t power)
    {
        m_ledStrip.dimm(power);
    }

    void display(std::vector<String>& args)
    {
        m_ledMatrix.display(args);
    }

    void setup()
    {
        m_ledMatrix.setLeds(ledMatrixIcons::wifiYellowIcon16x16());
        m_ledMatrix.glow();
        jarvisNode::setup();
    }

    void update()
    {
        jarvisNode::update();
        uint8_t power = m_potenciometer.readData();
        m_ledStrip.dimm(power);
    }

    void wifiConnected()
    {
        if(connectionStatus() == 3)
        {
            m_ledMatrix.setLeds(ledMatrixIcons::wifiGreenIcon16x16());
            m_ledMatrix.glow();
        }
        else if(connectionStatus() == 0)
        {
            m_ledMatrix.setLeds(ledMatrixIcons::wifiBlueIcon16x16());
        }
    }

    void wifiDisConnected()
    {
        m_ledMatrix.setLeds(ledMatrixIcons::wifiRedIcon16x16());
    }

    void jarvisConnected()
    {
        m_ledMatrix.setLeds(ledMatrixIcons::mMakeSpaceIcon16x16());
        m_ledMatrix.glow();
    }

    void jarvisDisConnected()
    {
        if(connectionStatus() == 3)
        {
            m_ledMatrix.setLeds(ledMatrixIcons::wifiGreenIcon16x16());
            m_ledMatrix.glow();
        }
        else if(connectionStatus() == 0)
        {
            m_ledMatrix.setLeds(ledMatrixIcons::wifiBlueIcon16x16());
        }
    }

protected:
    ledMatrix       m_ledMatrix;
    dhtSensor       m_dhtSensor;
    button          m_activateBtn;
    button          m_deactivateBtn;
    potenciometer   m_potenciometer;

};

#endif // LEDPANELNODE_H
