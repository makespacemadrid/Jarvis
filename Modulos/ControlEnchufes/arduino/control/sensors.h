
#define BUFFER  300

class magneticCurrentSensor
{
public:
  magneticCurrentSensor(int pin = -1, int bufferSize = 300) : m_pin(pin), m_buffer_size(bufferSize)
  {
    m_valid = m_pin!=-1;
  }
  bool isValid()    {return m_valid;}
  int bufferSize()  {return m_buffer_size;}
  void isrRead()
  {
    if(m_index >= m_buffer_size) m_index = 0;
    m_buffer[m_index] = analogRead(m_pin);
    m_index++;
  }
  int readMiliamps()
  {
    if(!isValid()) return 0;
    int average = 0;
    for (int i = 0  ; i < m_buffer_size  ; i++ )
    {
      average += (m_buffer[i] + (average*(i)))/i+1;
    }
    float conversion = 29.296875f;
    return average * conversion;
  }

private:
  bool m_valid = false;
  int  m_pin;
  int  m_buffer[BUFFER] = {0};
  int  m_buffer_size;
  int  m_index = 0;
};
