#include "sensors.h"

class SSR {
  public:
	SSR(int swPin = -1,int currentPin = -1,int maxAmps = 15,bool dimmable = false ,int tempPin = -1,int fanPin = -1) : 
	    m_switch_pin(swPin),m_current_sensor(currentPin),m_max_amps(maxAmps),m_temp_pin(tempPin),m_fan_pin(fanPin) {};
	    
	bool has_switch_pin()    {return m_switch_pin  != -1;}
	bool has_current_sensor(){return m_current_sensor.isValid();}
	bool has_temp_pin()      {return m_temp_pin    != -1;}
	bool has_fan_pin()       {return m_fan_pin     != -1;}
	bool switched()			 {return m_switched;}
	bool is_dimmable()		 {return m_dimmable;}
	void switch_on()		 {m_switched = true, m_dim_power = 100;}
	void switch_off()		 {m_switched = false;}
	void dimm(int power = 50){m_switched = true, m_dim_power = power;}

  magneticCurrentSensor& currentSensor(){return m_current_sensor;}
    
	void update()
	{
		if(m_current_sensor.isValid())
		{
			m_last_miliamps = m_current_sensor.readMiliamps();
			if(m_last_miliamps >= m_max_amps * 1000) 
			  switch_off();
		}
		if(has_temp_pin())
		{
			read_temp();
			if(m_last_temp >= m_max_temp)
				switch_off();
		}
	}

  private:
  int  m_switch_pin;
  int  m_current_pin;
  int  m_max_amps;
  int  m_max_temp			  = 60;
  int  m_temp_pin;
  int  m_fan_pin;
  bool m_switched			  = 0;
  bool m_dimmable;
  int  m_dim_power			= 0;
  int  m_last_miliamps  = 0;
  int  m_last_temp 			= 0;
  int  m_fan_pwr   			= 0;

  magneticCurrentSensor m_current_sensor;

void read_temp()
  {
    if(!has_temp_pin()) return;
    //leer
    //m_last_temp
  }

};

