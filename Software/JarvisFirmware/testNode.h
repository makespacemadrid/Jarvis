#ifndef TESTNODE_H
#define TESTNODE_H
#include "jarvisNode.h"
#include "testSensor.h"

class testNode : public jarvisNode
{
public:
    testNode(EEPROMStorage* settings) :
        jarvisNode(settings)
    {

        m_components.push_back(&m_sensor);
    }

    void setup(){
        jarvisNode::setup();//llama al setup del padre
    }

protected:

    testSensor m_sensor;
};


#endif // TESTNODE_H
