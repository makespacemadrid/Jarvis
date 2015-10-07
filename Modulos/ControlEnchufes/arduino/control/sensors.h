
typedef void (* functionPointer) ();


class temperatureSensor
{
public:
  temperatureSensor(int pin = -1) : m_pin(pin) {};
  bool isValid() {return m_pin != -1;}
  void setup()
  {
    
  }
  float readCelsius()
  {
    
  }
  
private:
  int m_pin;

};


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
