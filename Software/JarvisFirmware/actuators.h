#ifndef ACTUATORS_H
#define ACTUATORS_H

#include <vector>
#include "jarvisProtocol.h"

class actuators{
public:
//virtual puro : Implementar si o si en el heredado
    String  id()                {return m_id;}
    void    setId(String nID)   {m_id = nID;}

    virtual bool isValid() = 0;

    virtual std::vector<jarvisActions> getActions()
    {
        return m_actions;
    }

    virtual void setup()    {;}

    virtual void update()   {;}
    
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

    //interfaces para las acciones:
    virtual void enable()  {m_enabled = true;}
    virtual void disable() {m_enabled = false;deactivate();}

    //virtual que cada objeto puede elejir que implementa
    virtual void activate()                               {;}
    virtual void deactivate()                             {;}
    virtual void dimm(uint8_t power)                      {;}
    virtual void blink()                                  {;}
    virtual void setColor(uint8_t r, uint8_t g,uint8_t b) {;}
    virtual void cylon()                                  {;}
    virtual void beep(int beepTone,int toneDuration)      {;}

protected:
    std::vector<jarvisActions> m_actions;
    String  m_id;
    bool    m_enabled = true;
    std::vector<jarvisEvents>  m_capableEvents;
    std::vector<jarvisEvents>  m_events;
};

#endif // ACTUATORS_H
