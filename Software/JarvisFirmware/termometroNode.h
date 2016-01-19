#ifndef TERMOMETRONODE_H
#define TERMOMETRONODE_H

#include "jarvisNode.h"
#include"temperatureSensor.h"

class termometroNode : public jarvisNode{
public:
    termometroNode() : jarvisNode(), m_dhtSensor(4){

        m_components.push_back(m_dhtSensor.temperatureSensor());
        m_components.push_back(m_dhtSensor.humiditySensor());

        m_id = "termometro";
        disable();
    }

    virtual void sendEvent(String source,nodeComponent::event e)
    {//sobrecargar esta funcion para reaccionar a los eventos salientes.
        if(source == "switch"){
            if(!m_enabled){
                jarvisNode::sendEvent(source,e);
                return;
            }


        }
        jarvisNode::sendEvent(source,e);
    }

    void setup(){
        jarvisNode::setup();

    }


    void jarvisDisConnected()
    {
        wifiConnected();
        disable();
    }

    virtual void update(){
        jarvisNode::update();
        if(!m_enabled)return;
    }


protected:
    dhtSensor    m_dhtSensor;

};



#endif // TERMOMETRO_H
