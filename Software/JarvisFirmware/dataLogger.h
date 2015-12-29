#ifndef DATALOGGER_H
#define DATALOGGER_H

#include "nodeComponent.h"
#include <vector>

class dataLogger
{
public:
    dataLogger(std::vector<nodeComponent*>* componentsArray) : m_components(componentsArray)
    {

    }

    void setup()
    {

    }

    void update()
    {
        for(int c = 0 ; c < m_components->size() ; c++)
        {
            nodeComponent* component = m_components->at(c);
            if(component->canRead())
            {
                //hacer algo
            }
        }
    }

private:
  std::vector<nodeComponent*>* m_components;

};

#endif // DATALOGGER_H
