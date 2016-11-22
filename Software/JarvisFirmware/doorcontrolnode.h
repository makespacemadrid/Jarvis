#ifndef DOORCONTROLNODE
#define DOORCONTROLNODE

#include "jarvisNode.h"
#include "relay.h"
#include "buttons.h"

class doorControlNode : public jarvisNode //Definicion de la clase doorControlNode, hereda de jarvisNode
{
public:
    doorControlNode(EEPROMStorage *settings) :
        jarvisNode(settings),
        m_relay  (m_eeprom->settings().relaypins[0]),
        m_button (m_eeprom->settings().buttonPins[0])
    {//Contructor de la clase, se inicializan los componentes con su pin desde la config de la eeprom.
        //Poner nombre a los componentes
        m_relay.setId("abrePuerta");
        m_button.setId("Pulsador");

        //añadir los componentes del modulo al array de componentes para que los gestione el nodo
        m_components.push_back(&m_relay);
        m_components.push_back(&m_button);

        //definir los eventos que de los que el nodo puede informar
        addCapableEvent(E_ACTIVATED);
        //definir las acciones que puede realizar
        m_actions.push_back(A_ACTIVATE);

        //pone a ceroel contador de desconexion
        m_openTimeout = 0;
    }

    virtual void update()
    {//Este update sera ejecutado una vez por cada ciclo de update

        if(m_openTimeout > 0) //Si el tiempo de desactivacion es mayor a 0 es que estamos abiendo la puerta.
        {//Si estamos abriendo la puerta se resta al contador de desactivacion el tiempo que ha pasado desde ultima ejecucion del update.
            m_openTimeout -= updateInterval/1000.0f;

            if( !(m_openTimeout > 0) )
            {//si ya hemos llegado a cero,o menos, desactivamos el rele y el booleano de control
                m_relay.deactivate();
            }
        }

        jarvisNode::update(); // Para terminar se llama al nodo para que haga su update
    }

    virtual void activate()
    {//esta es la funcion que se ejecutara al mandarle al nodo la señal de activacion
        m_speaker.beep();//Pita dos veces
        m_speaker.beep();

        m_relay.activate();//activa el rele y pone el temporizador de desconexion
        m_openTimeout = 5;

        m_events.push_back(E_ACTIVATED); //Manda el evento de haber sido activado
    }

    virtual void sendEvent(String source,nodeComponent::event e)
    {//Esta funcion ve pasar los eventos que los componentes le mandan a Jarvis
     //Si hay que reaccionar a alguno de ellos este es un buen sitio
     //en 'source' esta el id del componente que manda el evento, y en 'e' el tipo de evento.
        if(!m_enabled)
        {//los eventos de los componentes solo se procesan si el nodo esta habilitado
            jarvisNode::sendEvent(source,e);
            return;
        }

        if(source == "Pulsador")
        {
            if(e.jevent == E_ACTIVATED)
            {
                activate();
            }
        }

        jarvisNode::sendEvent(source,e); // al terminar se llama al padre para que envie el evento
    }

private:
    relay  m_relay;
    button m_button;
    float  m_openTimeout;
};

#endif // DOORCONTROLNODE
