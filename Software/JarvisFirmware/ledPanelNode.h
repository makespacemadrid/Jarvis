#ifndef LEDPANELNODE_H
#define LEDPANELNODE_H

#include "jarvisNode.h"
#include "ledMatrixIcons.h"
#include "temperatureSensor.h"
#include "buttons.h"

class ledPanelNode : public jarvisNode
{
public:
    ledPanelNode(EEPROMStorage* settings) :
        jarvisNode(settings) ,
        m_ledMatrix(m_eeprom->settings().ledMatrixFirstLed,
                    m_eeprom->settings().ledMatrixCols,
                    m_eeprom->settings().ledMatrixRows,
                    &m_ledStrip,
                    m_eeprom->settings().ledMatrixMirror,
                    m_eeprom->settings().ledMatrixZigZag),
        m_dhtSensor     (m_eeprom->settings().tempSensorPins[0])
      , m_activateBtn   (m_eeprom->settings().buttonPins[0]),
        m_deactivateBtn (m_eeprom->settings().buttonPins[1]),
        m_potenciometer (m_eeprom->settings().buttonPins[2])
    {
        m_effectIndex = 0;
        m_components.push_back(&m_ledMatrix);
        m_components.push_back(m_dhtSensor.temperatureSensor());
        m_components.push_back(m_dhtSensor.humiditySensor());
        m_activateBtn.setId("OnBtn");
        m_deactivateBtn.setId("OffBtn");
        m_potenciometer.setId("BrightPotenciometer");
        m_components.push_back(&m_activateBtn);
        m_components.push_back(&m_deactivateBtn);
        m_components.push_back(&m_potenciometer);

        m_actions.push_back(A_ACTIVATE);
        m_actions.push_back(A_DEACTIVATE);
        m_actions.push_back(A_DIMM);
        m_actions.push_back(A_DISPLAY);

        m_capableEvents.push_back(E_ACTIVATED);
        m_capableEvents.push_back(E_DEACTIVATED);
        m_statusLed.disable();
        m_ledMatrix.setColor(0,0,0);
    }

    void activate()
    {
        rotateEffects();
        m_events.push_back(E_ACTIVATED);
    }

    void deactivate()
    {
        m_ledMatrix.deactivate();
        m_events.push_back(E_DEACTIVATED);
        m_effectIndex = 0;
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
        jarvisNode::setup();
        activate();
    }

    void update()
    {
        jarvisNode::update();
        if(m_potenciometer.isEnabled())
        {
            uint8_t power = m_potenciometer.readData();
            if( (power > 1) && (power < 98) )
                dimm(power);
        }
    }

    void rotateEffects()
    {
        if      (m_effectIndex == 0)
            m_ledMatrix.displayFromFile("/makespaceBannerBig");       
        else if (m_effectIndex == 1)
            m_ledMatrix.displayFromFile("/martesAbiertos");
        else if (m_effectIndex == 2)
            m_ledMatrix.displayFromFile("/marioStage");
        else if (m_effectIndex == 3)
            m_ledMatrix.cylon();        //Cylon
        else if (m_effectIndex == 4)
            m_ledMatrix.rainbow();      //Luz rainbow
        else if (m_effectIndex == 5)
            m_ledMatrix.chaoticLight(); //Luz caotica
        else if (m_effectIndex == 6)
            m_ledMatrix.setColor(250,0,0);  //Rojo
        else if (m_effectIndex == 7)
            m_ledMatrix.setColor(0,250,0);  //Verde
        else if (m_effectIndex == 8)
            m_ledMatrix.setColor(0,0,250);  //Azul
        else if (m_effectIndex == 9)
            m_ledMatrix.setColor(250,250,0);//Amarillo
        else if (m_effectIndex == 10)
        {
            m_ledMatrix.setColor(250,0,0);  //Rojo GLOW
            m_ledMatrix.glow();
        }
        else if (m_effectIndex == 11)
        {
            m_ledMatrix.setColor(0,250,0);  //Verde GLOW
            m_ledMatrix.glow();
        }
        else if (m_effectIndex == 12)
        {
            m_ledMatrix.setColor(0,0,250);  //Azul GLOW
            m_ledMatrix.glow();
        }
        else if (m_effectIndex == 13)
        {
            m_ledMatrix.setColor(250,250,0);//Amarillo GLOW
            m_ledMatrix.glow();
        }
        else

        {
            m_effectIndex = 0;
            return;
        }

        m_effectIndex++;
    }

protected:
    ledMatrix       m_ledMatrix;
    dhtSensor       m_dhtSensor;
    button          m_activateBtn;
    button          m_deactivateBtn;
    potenciometer   m_potenciometer;
    uint8_t         m_effectIndex;

};

#endif // LEDPANELNODE_H
