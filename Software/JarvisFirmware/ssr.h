#include "sensors.h"
#include "ws2812led.h"

static bool pinCanPWM(char pin)
{
#ifdef ESP8266
  return true; //Creo que todos los pines del esp pueden hacer pwm el (0 no)
#else
  if((pin == 3)  || //Arduino Nano puede hacer pwm en los pines 3,5,6,9,10,11.
     (pin == 5)  || 
     (pin == 6)  ||
     (pin == 9)  ||
     (pin == 10) ||
     (pin == 11))
    return true;
  else
    return false;
#endif
}

class SSR {
  public:
	SSR(int swPin = -1,int currentPin = -1,int maxAmps = 15,bool dimmable = false ,int tempPin = -1,int fanPin = -1) : 
	    m_switch_pin(swPin),m_current_sensor(currentPin),m_max_amps(maxAmps),m_fan_pin(fanPin) 
	    {
        if(dimmable)
          dimmable = pinCanPWM(swPin);
	    };
	    
	bool has_switch_pin()    {return m_switch_pin  != -1;}
	bool has_current_sensor(){return m_current_sensor.isValid();}
	//bool has_temp_sensor()   {return m_temp_sensor.isValid();}
	bool has_fan_pin()       {return m_fan_pin     != -1;}
	bool switched()          {return m_switched;}
	bool is_dimmable()       {return m_dimmable;}
  void setStatusLed(ws2812Strip::led* statusLed) {m_status_led  = statusLed;}
  magneticCurrentSensor& currentSensor(){return m_current_sensor;}


  void setup()
  {
    if(has_switch_pin()) pinMode(m_switch_pin, OUTPUT);
    //if(currentSensor().isValid())
    //  currentSensor().set2KhzInterrupt1();
    if(m_status_led)
      m_status_led->setColor(0,0,20);
  }

  void switchOn()
  {
    if(!has_switch_pin()) return;
    m_switched = true, m_dim_power = 100;
    digitalWrite(m_switch_pin, HIGH);
    if(m_status_led)
      m_status_led->setColor(0,20,0);
  }
  
  void switchOff()
  {
    if(!has_switch_pin()) return;
    m_switched = false;
    digitalWrite(m_switch_pin, LOW);
    if(m_status_led)
      m_status_led->setColor(0,0,20);
  }
	
	void dimm(int power = 50)
	{
    if(!has_switch_pin()) return;
    if(!m_dimmable)
    {
      switchOn();
      return;
    }
    m_switched = true, m_dim_power = power;  
    int value = power/100*255;
    analogWrite(m_switch_pin, value);
	}
    
	void update()
	{
		if(m_current_sensor.isValid())
		{
			m_last_miliamps = m_current_sensor.readMiliamps();
			if(m_last_miliamps >= m_max_amps * 1000) 
			  switchOff();
      //Serial.print("current:");
      //Serial.println(m_last_miliamps,10);
		}
//		if(m_temp_sensor.isValid())
//		{
			//m_last_temp = m_temp_sensor.readCelsius();
//			if(m_last_temp >= m_max_temp)
//				switchOff();
//		}
	}

  private:
  int  m_switch_pin;
  int  m_current_pin;
  int  m_max_amps;
  int  m_max_temp			  = 60;
  int  m_fan_pin;
  bool m_switched			  = 0;
  bool m_dimmable;
  int  m_dim_power			= 0;
  int  m_last_miliamps  = 0;
  int  m_last_temp 			= 0;
  int  m_fan_pwr   			= 0;

  //TemperatureSensor     m_temp_sensor;
  magneticCurrentSensor m_current_sensor;
  ws2812Strip::led*     m_status_led = 0;
};

