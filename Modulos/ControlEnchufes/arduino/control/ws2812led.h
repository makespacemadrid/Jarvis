#include <Adafruit_NeoPixel.h> //WTF hay que hacer el include en el main!

class led{
  public:
  led(uint8_t red =0,uint8_t green=0,uint8_t blue=0):
    r(red), g(green), b(blue) {};
  void setColor(uint8_t red =0,uint8_t green=0,uint8_t blue=0)
    {
      r=red,g=green,b=blue;
    }
  uint8_t r;
  uint8_t g;
  uint8_t b;
  void off()   {setColor(0,0,0);}
  void white() {setColor(200,200,200);}
};

class ws2812Strip {
  public:
  ws2812Strip(int pin, int lednr) : m_pin(pin), m_led_nr(lednr), m_pixels(lednr, pin, NEO_GRB + NEO_KHZ800)
  {
    if(( m_pin != -1) && (m_led_nr > 0))
    {
      m_leds = new led[m_led_nr];  
    } else {
      m_leds = new led[1];
    }
  }
  ~ws2812Strip() {delete m_leds;}

  bool isValid() {return m_pin != -1;}
  led* leds()    {return m_leds; }

  void setup()
  {
    if(!isValid()) return;
    m_pixels.begin();
    test();  
  }
  
  void update(){
    if(!isValid()) return;
        
    for(int i=0;i<m_led_nr;i++)
    {
      m_pixels.setPixelColor(i, m_leds[i].r,m_leds[i].g,m_leds[i].b);
    }
    m_pixels.show(); 
  }
  void off()
  {
    if(!isValid()) return;
    for( uint8_t i = 0 ; i < m_led_nr ; i++) 
    {
	    m_leds[i].off();
    }
    update();
  }
  void test()
  {
    if(!isValid()) return;
    for( uint8_t i = 0 ; i < m_led_nr ; i++) 
    {
      m_leds[i].setColor(10,10,10);
      update();
      delay(50);
    }
  }
  
  private:
    led* m_leds;
    int  m_led_nr;
    int  m_pin;
    Adafruit_NeoPixel m_pixels;
};
