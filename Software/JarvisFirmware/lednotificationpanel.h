#ifndef LEDNOTIFICATIONPANEL_H
#define LEDNOTIFICATIONPANEL_H

#include "jarvisNode.h"
#include "ledMatrixIcons.h"
#include "temperatureSensor.h"


class ledNotificationPanelNode : public jarvisNode
{
public:
    ledNotificationPanelNode(EEPROMStorage* settings) :
        jarvisNode(settings),
        m_ledMatrix(0,16,16,&m_ledStrip,false,true),
        m_dhtSensor(m_eeprom->settings().tempSensorPins[0])
    {
        m_components.push_back(&m_ledMatrix);
        m_components.push_back(m_dhtSensor.temperatureSensor());
        m_components.push_back(m_dhtSensor.humiditySensor());

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
        m_ledStrip.setBrightness(power);
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
};


#endif // LEDNOTIFICATIONPANEL_H
