#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H

#include "nodeComponent.h"

class dhtTemperatureSensor : public nodeComponent
{
public:
  dhtTemperatureSensor(DHT* dhts) : nodeComponent(0), m_dht(dhts)
  {
    m_id = F("dhtTemp");
    m_actions.push_back(A_READ_DATA);
    m_capableEvents.push_back(E_DATA_READ);
  }

  bool     canRead()  {return true;}


  virtual float      readData()
  {
    if(!m_enabled){return 0;} // si el dispositivo no esta habilitado ni se intenta acceder al hardware
    return m_dht->readTemperature();
  }

protected:
  DHT* m_dht;

};

class dhtHumiditySensor : public nodeComponent
{
public:
  dhtHumiditySensor(DHT* dhts) : nodeComponent(0), m_dht(dhts)
  {
    m_id = F("dhtHumid");
    m_actions.push_back(A_READ_DATA);
    m_capableEvents.push_back(E_DATA_READ);
  }

  bool     canRead()  {return true;}

  virtual float      readData()
  {
    if(!m_enabled){return 0;}
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

  nodeComponent* temperatureSensor() {return &m_temp;}
  nodeComponent* humiditySensor()    {return &m_humid;}

protected:
  DHT m_dht;
  dhtTemperatureSensor  m_temp;
  dhtHumiditySensor     m_humid;

};

class ntc100kTemperatureSensor : public nodeComponent
{
public:
  ntc100kTemperatureSensor(int pin) : nodeComponent(pin)
  {
    m_id = F("Temp");
    m_actions.push_back(A_READ_RAW);
    m_capableEvents.push_back(E_RAW_READ);
    m_actions.push_back(A_READ_DATA);
    m_capableEvents.push_back(E_DATA_READ);
  }

  void setup()
  {

  }

  bool isValid()
  {
      return true;
  }

  bool     canRead()  {return true;}

  uint16_t readRaw()
  {
      return 1024;
  }

  virtual float      readData()
  {
    if(!m_enabled){return 0;} // si el dispositivo no esta habilitado ni se intenta acceder al hardware
    return convertRead(readRaw());
  }


  static float convertRead(uint16_t raw)
  {
      return raw * 0.20f;
  }

protected:


};

#endif // TEMPERATURESENSOR_H
