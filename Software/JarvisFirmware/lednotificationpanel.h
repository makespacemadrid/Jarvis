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
        m_ledMatrix(m_eeprom->settings().ledMatrixFirstLed,
                    m_eeprom->settings().ledMatrixCols,
                    m_eeprom->settings().ledMatrixRows,
                    &m_ledStrip,
                    m_eeprom->settings().ledMatrixMirror,
                    m_eeprom->settings().ledMatrixZigZag),

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
        m_ledMatrix.setLeds(ledMatrixIcons::mMakeSpaceIcon16x16());
        m_ledMatrix.glow();
        m_events.push_back(E_ACTIVATED);
    }

    void deactivate()
    {
        m_ledMatrix.deactivate();
        m_events.push_back(E_DEACTIVATED);
    }

    void setColor(uint8_t r,uint8_t g,uint8_t b)
    {
        m_ledMatrix.setColor(r,g,b);
    }

    void dimm(uint8_t power)
    {
        m_ledStrip.setBrightness(power);
        m_eeprom->settings().ledStripBrightness = power/100.0;
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
        m_ledMatrix.glow();
    }

    void jarvisConnected()
    {
        activate();
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
