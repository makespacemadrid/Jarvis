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
      if(!m_enabled){return 0;}
      return analogRead(m_pin);
  }

  virtual float      readData()
  {
    if(!m_enabled){return 0;} // si el dispositivo no esta habilitado ni se intenta acceder al hardware
    return convertRead(readRaw());
  }


  static float convertRead(uint16_t raw)
  {

    #define THERMISTORNOMINAL 80000
    // temp. for nominal resistance (almost always 25 C)
    #define TEMPERATURENOMINAL 25
    // how many samples to take and average, more takes longer
    // but is more 'smooth'
    //#define NUMSAMPLES 5
    // The beta coefficient of the thermistor (usually 3000-4000)
    #define BCOEFFICIENT 3950
    // the value of the 'other' resistor
    #define SERIESRESISTOR 160000

      float average = raw;
      // convert the value to resistance
      average = 1023 / average - 1;
      average = SERIESRESISTOR / average;
      Serial.print("Thermistor resistance ");
      Serial.println(average);

      float steinhart;
      steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
      steinhart = log(steinhart);                  // ln(R/Ro)
      steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
      steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
      steinhart = 1.0 / steinhart;                 // Invert
      steinhart -= 273.15;                         // convert to C

      Serial.print("Temperature ");
      Serial.print(steinhart);
      Serial.println(" *C");
      return steinhart;
  }

protected:


};

#endif // TEMPERATURESENSOR_H
