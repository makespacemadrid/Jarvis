#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H

#include "nodeComponent.h"

class dhtTemperatureSensor : public nodeComponent
{
public:
  dhtTemperatureSensor(DHT* dhts) : nodeComponent(0), m_dht(dhts)
  {
    m_id = F("dhtTemp");
  }

  bool     canRead()  {return true;}

  virtual float      read()
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
  }

  bool     canRead()  {return true;}

  virtual float      read()
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


#endif // TEMPERATURESENSOR_H
