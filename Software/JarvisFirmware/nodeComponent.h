#ifndef NODECOMPONENTS_H
#define NODECOMPONENTS_H

//interfaz para manejar sensores/actuadores.

#include <vector>
#include "jarvisProtocol.h"

uint16_t getFreeMem()
{
  #ifdef ESP8266
  return system_get_free_heap_size();
  #else
  return freeMemory();
  #endif
}

class nodeComponent{
public:
    class event
    {
      public:
      event(jarvisEvents e) : jevent(e) {};
      jarvisEvents          jevent;
      std::vector<String>   args;      
    };

//    class action
//    {
//      public:
//      action(jarvisActions a) : jaction(a) {}
//      jarvisActions        jaction;
//      std::vector<String>  args;
//    };
    
    nodeComponent(int pin = -1) : m_pin(pin)
    {
        m_actions.push_back(A_ENABLE);
        m_actions.push_back(A_DISABLE);
        m_capableEvents.push_back(E_ENABLED);
        m_capableEvents.push_back(E_DISABLED);
    }

    String  id()                {return m_id;}
    void    setId(String nID)   {m_id = nID;}
    int     pinNr()             {return m_pin;}

//Sobrecargar en la herencia si es menester
    virtual bool isValid()      {return true;}
    virtual bool isEnabled()    {return m_enabled;}

    virtual void setup()    {;}
    virtual void update()   {;}

    virtual bool     canRead()  {return false;}
    virtual uint16_t readRaw()  {return 666;}
    virtual float    readData() {return 666;}

    //interfaces para las acciones:
    virtual void enable()
    {
        m_enabled = true;
        m_events.push_back(event(E_ENABLED));
    }
    virtual void disable()
    {
        deactivate();
        m_enabled = false;
        m_events.push_back(event(E_DISABLED));
    }

    virtual void activate()                               {;}
    virtual void deactivate()                             {;}
    virtual void toggle()                                 {;}

    virtual void sendRawRead()
    {
        event e(E_RAW_READ);
        e.args.push_back(String(readRaw()));
        m_events.push_back(e);
    }

    virtual void sendDataRead()
    {
        event e(E_DATA_READ);
        e.args.push_back(String(readData()));
        m_events.push_back(e);
    }

    virtual void dimm(uint8_t power)                            {;}
    virtual void blink()                                        {;}
    virtual void glow()                                         {;}
    virtual void fade()                                         {;}
    virtual void setColor(uint8_t r, uint8_t g,uint8_t b)       {;}
    virtual void cylon()                                        {;}
    virtual void setLeds(std::vector<String>&  args)            {;}
    virtual void display(std::vector<String>&  args)            {;}
    virtual void beep()                                         {;}
    virtual void playTone(int tone =50,int toneDuration=100)    {;}
    virtual void playRtttl(char *p)                             {;}
    virtual void makeCoffee()                                   {;}


    //Acciones y eventos

    virtual std::vector<jarvisActions> getActions()
    {
        return m_actions;
    }

    virtual void doAction(jarvisActions act,std::vector<String>& args)
    {
        //Serial.print("processing action:");
        //Serial.println(getFreeMem());
        //Serial.print("act:");
        Serial.print(act);
        Serial.print(" args:");
        Serial.println(args.size());
        if          (act == A_ENABLE) {
            enable();
        } else if   (act == A_DISABLE) {
            disable();
        } else if   (!m_enabled) {//si esta deshabilitado solo se procesa el enable/disable
            return;
        } else if   (act == A_ACTIVATE) {
            activate();
        } else if   (act == A_DEACTIVATE) {
            deactivate();
        } else if   (act == A_TOGGLE) {
            toggle();
        } else if   (act == A_READ_RAW) {
            sendRawRead();
        } else if   (act == A_READ_DATA) {
            sendDataRead();
        } else if   (act == A_DIMM) {
            if(args.size() != 1) return;
            dimm(args[0].toInt());
        } else if   (act == A_BLINK) {
            blink();
        } else if   (act == A_GLOW) {
            glow();
        } else if   (act == A_FADE) {
            fade();
        } else if   (act == A_SET_COLOR) {
            if(args.size() != 3) return;
            setColor(args[0].toInt(),args[1].toInt(),args[2].toInt());
        } else if   (act == A_CYLON) {
            cylon();
        } else if   (act == A_SET_LEDS) {
            setLeds(args);
        } else if   (act == A_DISPLAY) {
            display(args);
        } else if   (act == A_BEEP) {
            beep();
        } else if   (act == A_PLAYTONE) {
            if(args.size() != 2) return;
            playTone(args[0].toInt(),args[1].toInt());
        } else if   (act == A_PLAYRTTTL) {
            if(args.size() < 1) return;
            char song[args[0].length()];
            args[0].toCharArray(song, sizeof(song));
            playRtttl(song);
        } else if   (act == A_MAKE_COFFEE) {
            makeCoffee();
        }
    }

    void addEvent(event nevent)
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

    std::vector<event> getEvents()
    {
        std::vector<event> events;
        events = m_events;
        m_events.clear();
        return events;
    }

protected:
    int                         m_pin;
    std::vector<jarvisActions>  m_actions;
    std::vector<event>          m_events;
    std::vector<jarvisEvents>   m_capableEvents;
    String                      m_id;
    bool                        m_enabled = true;

};

#endif // NODECOMPONENTS_H
