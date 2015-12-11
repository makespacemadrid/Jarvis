#include <DHT.h>

#ifndef SENSOR
#define SENSOR

#include "jarvisProtocol.h"

class sensor
{
  public:
    sensor(int pin = -1) : m_pin(pin)
    {};

    String&           id()       {return m_id;}
    void setId(String nId)       {m_id= nId;}
    virtual void     setup()     = 0;
    virtual uint16_t readRaw()   = 0;
    virtual float    read()      = 0;

    virtual void     update()
    {
      
    }

    bool isValid()
    {
      return (m_pin != -1);
    }

    void addCapableEvent(jarvisEvents nevent)
    {
        m_capableEvents.push_back(nevent);
    }

    void addEvent(jarvisEvents nevent)
    {
        m_events.push_back(nevent);
    }

    const std::vector<jarvisEvents>& capableEvents()   {return m_capableEvents;}

    bool hasEvents()
    {
        return (m_events.size() > 0);
    }

    std::vector<jarvisEvents> getEvents()
    {
        std::vector<jarvisEvents> events;
        while(m_events.size())
        {
            events.push_back(m_events[0]);
            m_events.erase(m_events.begin());
        }

        return events;
    }

  protected:
    String m_id;
    int m_pin;
    std::vector<jarvisEvents>  m_capableEvents;
    std::vector<jarvisEvents>  m_events;

};





//Replantear para hacer uso de la interfaz sensors
class magneticCurrentSensor
{
public:
  magneticCurrentSensor(int pin = -1, int bufferSize = 300) : m_pin(pin), m_buffer_size(bufferSize)
  {
    m_buffer = new int[m_buffer_size];
    for (int i = 0 ; i < m_buffer_size ; i++)
      m_buffer[i] = 0;
  }
  ~magneticCurrentSensor() {delete m_buffer;}
  
  bool isValid()    {return m_pin!=-1;}
  int bufferSize()  {return m_buffer_size;}
  void setConversionfactor(float factor) {m_conversion_factor = factor;}
  void isrRead()
  {
    if(!isValid()) return;
    if(m_index >= m_buffer_size) m_index = 0;
    m_buffer[m_index] = analogRead(m_pin);
    m_index++;
  }

  int readWatts()
  {
    //P = V*I
    return m_volts * (readMiliamps()/1000);
  }

  int readMiliamps()
  {
    if(!isValid()) return 0;
    
    int average = 0;
    for (int i = 0  ; i < m_buffer_size  ; i++ )
    {
      average += (m_buffer[i] + (average*(i)))/i+1;
    }
    return average * m_conversion_factor;

  }

private:
  int   m_pin;
  int*  m_buffer;
  int   m_buffer_size;
  int   m_index = 0;
  float m_conversion_factor = 29.296875f;
  int   m_volts             = 220;
};

#endif
