#ifndef LED
#define LED


#include <Adafruit_NeoPixel.h> //WTF hay que hacer el include en el ino!
#include <vector>
#include "nodeComponent.h"


class ws2812Strip {
public:
    class led{ // clase led dentro de la clase Strip
    public:
      led(): r(0), g(0), b(0) {;}
      void setColor(uint8_t red =0,uint8_t green=0,uint8_t blue=0)
        {
          r=red,g=green,b=blue;
        }
      void off()   {setColor(0,0,0);}
      void white() {setColor(200,200,200);}
      uint8_t r;
      uint8_t g;
      uint8_t b;
    };
  
  ws2812Strip(int pin = -1, int lednr = 25) : m_pin(pin) , m_pixels(lednr, pin, NEO_GRB + NEO_KHZ800)
  {
    if((isValid()))
    {
      for(int i = 0 ; i < lednr ; i++) m_leds.push_back(led());
    }
  }

  bool isValid() {return  (m_pin != -1);}

  std::vector<led> leds()&   {return m_leds;}

  led* getLed(int pos)
  {
    return &m_leds[pos];
  }

  void setup()
  {
    if(!isValid()) return;
    m_pixels.begin();
    test();  
  }
  
  void update(){
    if(!isValid()) return;
    for(int i=0;i<m_leds.size();i++)
    {
      m_pixels.setPixelColor(i, m_leds[i].r,m_leds[i].g,m_leds[i].b);
    }
    m_pixels.show(); 
  }

  void off()
  {
    if(!isValid()) return;
    for( uint8_t i = 0 ; i < m_leds.size() ; i++) 
    {
	    m_leds[i].off();
    }
    update();
  }

  void setColor(uint8_t r,uint8_t g, uint8_t b){
    if(!isValid()) return;     
    for(int i=0;i<m_leds.size();i++)
    {
      m_leds[i].setColor(r,g,b);
    }
    update();
  }

  void test()
  {
    if(!isValid()) return;
    for( uint8_t i = 0 ; i < m_leds.size() ; i++) 
    {
      m_leds[i].setColor(50,50,50);
      update();
      delay(20);
    }
  }
  
private:
    std::vector<led>    m_leds;
    int                 m_pin;
    Adafruit_NeoPixel   m_pixels;
};


class ledGadget :  public nodeComponent
{
  public:
    enum animationType
    {
        animationNone,
        animationFade,
        animationGlow,
        animationBlink,
        animationCylon
    };

    ledGadget(ws2812Strip* m_parentStrip): m_strip(m_parentStrip)
    {
      m_capableEvents.push_back(E_ACTIVATED);
      m_capableEvents.push_back(E_DEACTIVATED);
      m_actions.push_back(A_ACTIVATE);
      m_actions.push_back(A_DEACTIVATE);
      m_actions.push_back(A_SET_COLOR);
    }
    
    virtual void animate() {;}

    bool isValid() {return m_strip->isValid();}

    void deactivate()
    {
        off();
        m_events.push_back(E_DEACTIVATED);
    }

    virtual void off()
    {
        m_animationType = animationNone;
        for(int i = 0 ; i < m_leds.size() ; i++)
        {
            m_leds[i]->off();
        }
        m_strip->update();
    }

    virtual void setColor(uint8_t r, uint8_t g, uint8_t b)
    {
        m_animationType = animationNone;
        for(int i = 0 ; i < m_leds.size() ; i++)
        {
            m_leds[i]->setColor(r,g,b);
        }
        m_strip->update();
    }

    void addLed(ws2812Strip::led* nLed)
    {
        m_leds.push_back(nLed);
    }
    void addLed(int start,int count)
    {
        for(int i = start ; i < start+count ; i++ )
        {
            m_leds.push_back(m_strip->getLed(i));
        }
    }

  protected:
    ws2812Strip*                    m_strip;
    std::vector<ws2812Strip::led*>  m_leds;
    animationType                   m_animationType = animationNone;
};

class ledBar : public ledGadget
{
public:
    ledBar(ws2812Strip* parentStrip) : ledGadget(parentStrip), m_reversed(false) 
    {
      m_id = "ledBar";
      m_actions.push_back(A_CYLON);
    }
    void setReversed(bool reversed) {m_reversed = reversed;}

    void update()
    {
      animate();
    }

    virtual void fade()
    {
        m_counter1 = 0, m_counter2 = 0 ; m_counter3 = 0;
        m_animationType = animationFade;
    }

    virtual void glow()
    {
        m_counter1 = 0, m_counter2 = 0 ; m_counter3 = 150;
        m_animationType = animationGlow;
    }

    virtual void blink()
    {
        m_counter1 = 0, m_counter2 = 0 ; m_counter3 = 0;
        m_animationType = animationBlink;
    }

    virtual void cylon()
    {//animacion cylon
        m_counter1 = 0, m_counter2 = 0 ; m_counter3 = 0;
        m_animationType = animationCylon;
    }

    virtual void animate()
    {
        if      (m_animationType == animationNone)
        {
            return;
        }else if(m_animationType == animationFade)
        {
            animateFade();
        }else if(m_animationType == animationGlow)
        {
            animateGlow();
        }else if(m_animationType == animationBlink)
        {
            animateBlink();
        }else if(m_animationType == animationCylon)
        {
            animateCylon();
        }
    }


protected:
    bool m_reversed;
    uint8_t m_counter1;
    uint8_t m_counter2;
    uint8_t m_counter3;

    void animateFade()
    {
        bool all_off = true;
        for(int i = 0 ; i < m_leds.size() ; i++)
        {
            if(m_leds[i]->r > 0)
            {
                all_off = false;
                m_leds[i]->r -= 1;
            }
            if(m_leds[i]->g > 0)
            {
                all_off = false;
                m_leds[i]->g -= 1;
            }
            if(m_leds[i]->b > 0)
            {
                all_off = false;
                m_leds[i]->b -= 1;
            }
        }
        if(all_off) m_animationType == animationNone;
        m_strip->update();
    }

    void animateGlow()
    {//contador 1 se usa contar la iteracion, y el 2 para la direccion(sumando/restando), y el 3 para el limite
        if(m_counter2 == 0)
        {//sumando
            for(int i = 0 ; i < m_leds.size() ; i++)
            {
                if(m_leds[i]->r < 255)
                {
                    m_leds[i]->r += 1;
                }
                if(m_leds[i]->g < 255)
                {
                    m_leds[i]->g += 1;
                }
                if(m_leds[i]->b < 255)
                {
                    m_leds[i]->b += 1;
                }
            }
            m_counter1++;
            if(m_counter1 == m_counter3)
              m_counter2 = 1;
        }
        else
        {//restando
            for(int i = 0 ; i < m_leds.size() ; i++)
            {
                if(m_leds[i]->r > 0)
                {
                    m_leds[i]->r -= 1;
                }
                if(m_leds[i]->g > 0)
                {
                    m_leds[i]->g -= 1;
                }
                if(m_leds[i]->b > 0)
                {
                    m_leds[i]->b -= 1;
                }
            }
            m_counter1--;
            if(m_counter1 == 0)
                m_counter2 = 0;
        }
//        Serial.print("C1: ");
//        Serial.print(m_counter1);
//        Serial.print(" :C2: ");
//        Serial.print(m_counter2);
//        Serial.print(" :C3: ");
//        Serial.println(m_counter3);
//        Serial.println("   ");
//        Serial.println(m_leds[0]->b);
        m_strip->update();
    }

    void animateBlink()
    {

    }

    void animateCylon()
    {

    }

};


class ledStatusTrio : public ledBar
{
  public:
    ledStatusTrio(ws2812Strip* m_parentStrip, uint8_t mControllerLed, uint8_t wifiLed, uint8_t extraLed) :
       ledBar(m_parentStrip)
    {
        m_id = "ledStatusTrio";
        m_controllerLed = m_strip->getLed(mControllerLed);
        m_wifiLed       = m_strip->getLed(wifiLed);
        m_extraLed      = m_strip->getLed(extraLed);
        addLed(m_controllerLed);
        addLed(m_wifiLed);
        addLed(m_extraLed);
    }

    void animate()
    {

    }

    void controllerInit()
    {

        m_controllerLed->setColor(0,10,10);
        m_strip->update();
    }

    void controllerOK()
    {
        m_controllerLed->setColor(0,10,0);
        m_strip->update();
    }

    void controllerError()
    {
        m_controllerLed->setColor(10,0,0);
        m_strip->update();
    }

    void wifiTX()
    {
        m_wifiLed->setColor(0,10,0);
        m_strip->update();
    }

    void wifiRX()
    {
        m_wifiLed->setColor(10,0,0);
        m_strip->update();
    }

    void wifiInit()
    {
        m_wifiLed->setColor(10,10,0);
        m_strip->update();
    }

    void wifiClient()
    {
        m_wifiLed->setColor(0,0,0);
        m_wifiStatusColor = *m_wifiLed;
        m_strip->update();
    }

    void wifiAutoConfig()

    {
        m_wifiLed->setColor(10,0,10);
        m_wifiStatusColor = *m_wifiLed;
        m_strip->update();
    }

    void wifiOK()
    {
        *m_wifiLed = m_wifiStatusColor;
        m_strip->update();
    }

    void wifiError()
    {
        m_wifiLed->setColor(10,0,0);
        m_strip->update();
    }

  private:
    ws2812Strip::led*   m_controllerLed;
    ws2812Strip::led*   m_wifiLed;
    ws2812Strip::led*   m_extraLed;
    ws2812Strip::led    m_wifiStatusColor;
};

#endif
