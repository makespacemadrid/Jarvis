#ifndef NODECOMPONENTS_H
#define NODECOMPONENTS_H

//interfaz para manejar sensores/actuadores.

#include <vector>
#include "jarvisProtocol.h"

class nodeComponent{
public:
    nodeComponent(int pin = -1) : m_pin(pin) {;}

    String  id()                {return m_id;}
    void    setId(String nID)   {m_id = nID;}

//Sobrecargar en la herencia si es menester
    virtual bool isValid()      {return true;}
    virtual void setup()    {;}
    virtual void update()   {;}

    virtual bool     canRead()  {return false;}
    virtual uint16_t readRaw()  {return 666;}
    virtual float    read()     {return 666;}

    //interfaces para las acciones:
    virtual void enable()
    {
        m_enabled = true;
        m_events.push_back(E_ENABLED);
    }
    virtual void disable()
    {
        m_enabled = false;
        deactivate();
    }

    virtual void activate()                               {;}
    virtual void deactivate()                             {;}
    virtual void dimm(uint8_t power)                      {;}
    virtual void blink()                                  {;}
    virtual void setColor(uint8_t r, uint8_t g,uint8_t b) {;}
    virtual void cylon()                                  {;}
    virtual void beep(int beepTone,int toneDuration)      {;}


    //Acciones y eventos

    virtual std::vector<jarvisActions> getActions()
    {
        return m_actions;
    }

    virtual void doAction(jarvisActions action,std::vector<String> args)
    {
        if          (action == A_ENABLE) {
            enable();
        } else if   (action == A_DISABLE) {
            disable();
        } else if   (action == A_ACTIVATE) {
            activate();
        } else if   (action == A_DEACTIVATE) {
            deactivate();
        } else if   (action == A_DIMM) {
            if(args.size() != 1) return;
            dimm(args[0].toInt());
        } else if   (action == A_BLINK) {
            blink();
        } else if   (action == A_SET_COLOR) {
            if(args.size() != 3) return;
            setColor(args[0].toInt(),args[1].toInt(),args[2].toInt());
        } else if   (action == A_CYLON) {
            cylon();
        } else if   (action == A_BEEP) {
            if(args.size() != 2) return;
            beep(args[0].toInt(),args[1].toInt());
        }
    }

    void addEvent(jarvisEvents nevent)
    {
        m_events.push_back(nevent);
    }
    
    void addCapableEvent(jarvisEvents nevent)
    {
        m_capableEvents.push_back(nevent);
    }

    const std::vector<jarvisEvents>& capableEvents()   {return m_capableEvents;}

    bool hasEvents()
    {
        return (m_events.size() > 0);
    }

    std::vector<jarvisEvents> getEvents()
    {
        std::vector<jarvisEvents> events;
        events = m_events;
        m_events.clear();
        return events;
    }

protected:
    int                         m_pin;
    std::vector<jarvisActions>  m_actions;
    String                      m_id;
    bool                        m_enabled = true;
    std::vector<jarvisEvents>   m_capableEvents;
    std::vector<jarvisEvents>   m_events;
};

#endif // NODECOMPONENTS_H
