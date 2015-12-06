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

class button : public sensor
{
public:
  button(int pin = -1) : sensor(pin), m_invertedLogic(false), m_statusChanged(false)
  {
      m_capableEvents.push_back(E_ACTIVATED);
      m_capableEvents.push_back(E_DEACTIVATED);
      m_id = F("switch");
  }

  virtual uint16_t readRaw()
  {
      if(!isValid()) return 0;
      if(m_invertedLogic)
        return !digitalRead(m_pin);
      else
        return !digitalRead(m_pin);
  }

  virtual float     read()
  {
      return readRaw();
  }

  virtual void setup()
  {
      if(!isValid()) return;
      pinMode(m_pin, INPUT);
      m_lastStatus = readRaw();
  }

  bool switched()
  {
      return m_lastStatus;
  }


  virtual void update()
  {//añadir mecanismo de debounce?
      bool status = readRaw();
      if(status != m_lastStatus)
      {
          m_lastStatus = status;
          if(status)
              m_events.push_back(E_ACTIVATED);
          else
              m_events.push_back(E_DEACTIVATED);
      }
  }

  void setInvertedLogic(bool inverted) {m_invertedLogic = inverted;}

protected:
  bool m_invertedLogic;
  bool m_lastStatus;
  bool m_statusChanged;
};

class dhtTemperatureSensor : public sensor
{
public:
  dhtTemperatureSensor(DHT* dhts) : sensor(0), m_dht(dhts)
  {
    m_id = F("dhtTemp");
  }
  void setup()
  {
    //m_dht.init()
  }


  virtual uint16_t readRaw()
  {
    return m_dht->readTemperature();
  }
  virtual float      read()
  {
    return m_dht->readTemperature();
  }
  
protected:
  DHT* m_dht;
};

class dhtHumiditySensor : public sensor
{
public:
  dhtHumiditySensor(DHT* dhts) : sensor(0), m_dht(dhts)
  {
    m_id = F("dhtHumid");
  }

  void setup()
  {

  }

  virtual uint16_t readRaw()
  {
    return m_dht->readHumidity();
  }

  virtual float      read()
  {
    return m_dht->readHumidity();
  }
  
protected:
  DHT* m_dht;
};


/////
//// Pasan cosas raras al instanciar dos veces el DHT, le he creado su propio objeto y a los dos objetos sensor(temp y humedad) se les pasa el puntero
////
////

class dhtSensor
{
public:
  dhtSensor(int pin = -1) : 
  m_dht(pin,DHT22), m_temp(&m_dht), m_humid(&m_dht) {}

  sensor* temperatureSensor() {return &m_temp;}
  sensor* humiditySensor()    {return &m_humid;}
  
protected:
  DHT m_dht;
  dhtTemperatureSensor  m_temp;
  dhtHumiditySensor     m_humid;
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
  void set2KhzInterrupt1()
  {
#ifndef ESP8266
  cli();//stop interrupts
  //set timer0 interrupt at 2kHz
    TCCR0A = 0;// set entire TCCR2A register to 0
    TCCR0B = 0;// same for TCCR2B
    TCNT0  = 0;//initialize counter value to 0
    // set compare match register for 2khz increments
    OCR0A = 124;// = (16*10^6) / (2000*64) - 1 (must be <256)
    // turn on CTC mode
    TCCR0A |= (1 << WGM01);
    // Set CS01 and CS00 bits for 64 prescaler
    TCCR0B |= (1 << CS01) | (1 << CS00);   
    // enable timer compare interrupt
    TIMSK0 |= (1 << OCIE0A);
    sei();
#endif
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
