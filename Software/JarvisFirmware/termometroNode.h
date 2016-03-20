#ifndef TERMOMETRONODE_H
#define TERMOMETRONODE_H

#include "jarvisNode.h"
#include"temperatureSensor.h"

class termometroNode : public jarvisNode{
public:
    termometroNode(EEPROMStorage* settings) :
        jarvisNode(settings),
        m_dhtSensor(m_eeprom->settings().tempSensorPins[0])
    {
        m_components.push_back(m_dhtSensor.temperatureSensor());
        m_components.push_back(m_dhtSensor.humiditySensor());
        disable();
    }   

    void setup(){
        jarvisNode::setup();
    }


    void jarvisDisConnected()
    {
        wifiConnected();
        disable();
    }

    void jarvisConnected(){
        enable();

    }

    virtual void update(){
        jarvisNode::update();
        if(!m_enabled)return;
    }


protected:
    dhtSensor    m_dhtSensor;

};



#endif // TERMOMETRO_H
