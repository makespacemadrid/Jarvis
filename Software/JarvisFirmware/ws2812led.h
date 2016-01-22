#ifndef LED
#define LED


#include <Adafruit_NeoPixel.h> //WTF hay que hacer el include en el ino!
#include <vector>
#include "nodeComponent.h"
#include "ledMatrixIcons.h"


class ws2812Strip {
public:
    class led{ // clase led dentro de la clase Strip
    public:
      led(): r(0), g(0), b(0), bright(1.0) {;}
      void setColor(uint8_t red =0,uint8_t green=0,uint8_t blue=0)
        {
          r=red   * bright;
          g=green * bright;
          b=blue  * bright;
        }

      void dimm(float factor = 0.5f)
      {
          r *= factor;
          g *= factor;
          b *= factor;
      }

      void setBrightness(float b)
      {
          bright = b;
      }

      void off()   {setColor(0,0,0);}
      void white() {setColor(200,200,200);}
      uint8_t r;
      uint8_t g;
      uint8_t b;
      float bright;
    };
  
  ws2812Strip(int pin = -1, int lednr = 25) : m_pin(pin) , m_pixels(lednr, pin, NEO_GRB + NEO_KHZ800)
  {
    m_brightness = 0.2f;
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
    off();

  }
  
  void update(){
    if(!isValid()) return;
    for(int i=0;i<m_leds.size();i++)
    {
      m_pixels.setPixelColor(i, m_leds[i].r*m_brightness,m_leds[i].g*m_brightness,m_leds[i].b*m_brightness);
    }
    m_pixels.show(); 
  }

  void off()
  {
    if(!isValid()) return;
    for( uint16_t i = 0 ; i < m_leds.size() ; i++)
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
    for( uint16_t i = 0 ; i < m_leds.size() ; i++)
    {
      m_leds[i].setColor(20,20,20);
      update();
      //delay(5);
    }
  }
  
private:
    std::vector<led>    m_leds;
    int                 m_pin;
    float               m_brightness;
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

    ledGadget(ws2812Strip* m_parentStrip): m_strip(m_parentStrip) , m_bright(1.0)
    {
      m_capableEvents.push_back(E_ACTIVATED);
      m_capableEvents.push_back(E_DEACTIVATED);
      m_actions.push_back(A_ACTIVATE);
      m_actions.push_back(A_DEACTIVATE);
      m_actions.push_back(A_SET_COLOR);
      m_actions.push_back(A_SET_LEDS);
      m_actions.push_back(A_SET_LED);
    }
    
    virtual void animate() {;}

    bool isValid() {return m_strip->isValid();}

    void deactivate()
    {
        if(!m_enabled) return;
        off();
        m_events.push_back(E_DEACTIVATED);
    }

    virtual void off()
    {
        if(!m_enabled) return;
        m_animationType = animationNone;
        for(int i = 0 ; i < m_leds.size() ; i++)
        {
            m_leds[i]->off();
        }
        m_strip->update();
    }

    virtual void setColor(uint8_t r, uint8_t g, uint8_t b)
    {
        if(!m_enabled) return;
        m_animationType = animationNone;
        for(int i = 0 ; i < m_leds.size() ; i++)
        {
            m_leds[i]->setColor(r,g,b);
        }
        m_strip->update();
    }

    void setBrightness(float b = 1.0)
    {
        m_bright = b;
        for(int i = 0 ; i < m_leds.size() ; i++)
        {
            m_leds[i]->setBrightness(b);
        }
        m_strip->update();
    }


    void setLed(std::vector<String>&  args)
    {
        if((args.size() < 4) || !m_enabled ) return;
        m_animationType = animationNone;
        for(int p = 0 ; p < args.size() ; p+=4)
        {
            int ledPos = args[p].toInt();
            int r = args[p+1].toInt();
            int g = args[p+2].toInt();
            int b = args[p+3].toInt();

            if(m_leds.size() < ledPos) return;
            m_leds[ledPos]->setColor(r,g,b);
        }
    }

    void setLeds(std::vector<String>&  args)
    {
      if(!m_enabled) return;
      m_animationType = animationNone;
      for(int i = 0 ; i < args.size(); i+=3)
      {
          int n = i/3;
          if( n <= m_leds.size())
          {
            ws2812Strip::led* l = m_leds[n];
            l->setColor(args[i].toInt(),args[i+1].toInt(),args[i+2].toInt());
          }
      }
    }

    void setLeds(std::vector<uint8_t>  args)
    {
      if(!m_enabled) return;
      m_animationType = animationNone;
      for(int i = 0 ; i < args.size(); i+=3)
      {
          int n = i/3;
          if( n <= m_leds.size())
          {
            uint8_t r = args[i];
            uint8_t g = args[i+1];
            uint8_t b = args[i+2];
            ws2812Strip::led* l = m_leds[n];
            l->setColor(r,g,b);
          }
      }
    }

    void addLed(ws2812Strip::led* nLed)
    {
        m_leds.push_back(nLed);
    }

    void addLed(int start,int count,bool reverseOrder = false)
    {
        if(reverseOrder)
        {
            for(int i = start+count-1 ; i >= start ; i-- )
            {
                m_leds.push_back(m_strip->getLed(i));
            }
        }
        else
        {
            for(int i = start ; i < start+count ; i++ )
            {
                m_leds.push_back(m_strip->getLed(i));
            }
        }
    }

  protected:
    ws2812Strip*                    m_strip;
    std::vector<ws2812Strip::led*>  m_leds;
    animationType                   m_animationType = animationNone;
    float m_bright;
};

class ledBar : public ledGadget
{
public:
    ledBar(ws2812Strip* parentStrip) : ledGadget(parentStrip), m_reversed(false) 
    {
      m_id = "ledBar";
      m_actions.push_back(A_CYLON);
      m_actions.push_back(A_GLOW);
      m_actions.push_back(A_FADE);
    }
    void setReversed(bool reversed) {m_reversed = reversed;}

    void update()
    {
      if(!m_enabled) return;
      animate();
    }

    virtual void fade()
    {
        m_counter1 = 0, m_counter2 = 0 ; m_counter3 = 0;
        m_animationType = animationFade;
    }

    virtual void glow()
    {
        m_counter1 = 50, m_counter2 = 1 ; m_counter3 = 50;
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
        if(!m_enabled) return;
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
        m_strip->update();
    }


protected:
    bool m_reversed;
    uint8_t m_counter1;
    uint8_t m_counter2;
    uint8_t m_counter3;


    virtual void animateFade()
    {
        bool all_off = true;
        for(int i = 0 ; i < m_leds.size() ; i++)
        {
            m_leds[i]->dimm(0.8);
            if ( (m_leds[i]->r > 0) ||
                 (m_leds[i]->g > 0) ||
                 (m_leds[i]->b > 0) )
            {
                all_off = false;
            }
        }
        if(all_off)
            deactivate();
    }

    virtual void animateGlow()
    {//contador 1 se usa contar la iteracion, y el 2 para la direccion(sumando/restando), y el 3 para el limite
        if(m_counter2 == 0)
        {//sumando
            for(int i = 0 ; i < m_leds.size() ; i++)
            {
                if ((m_leds[i]->r > 0) && (m_leds[i]->r < 250))
                {
                    m_leds[i]->r += 5;
                }
                if((m_leds[i]->g > 0) && (m_leds[i]->g < 250))
                {
                   m_leds[i]->g += 5;
               }
               if((m_leds[i]->b > 0) && (m_leds[i]->b < 250))
               {
                   m_leds[i]->b += 5;
               }
//                m_leds[i]->setBrightness(m_counter3*100.0f/m_counter1/100.0f);
            }
            m_counter1++;
            if(m_counter1 == m_counter3)
              m_counter2 = 1;
        }
        else
        {//restando
            for(int i = 0 ; i < m_leds.size() ; i++)
            {
                if(m_leds[i]->r > 5)
                {
                    m_leds[i]->r -= 5;
                }
               if(m_leds[i]->g > 5)
                {
                    m_leds[i]->g -= 5;
                }
                if(m_leds[i]->b > 5)
                {
                    m_leds[i]->b -= 5;
                }
//                m_leds[i]->setBrightness(m_counter3*100.0f/m_counter1/100.0f);
            }
            m_counter1--;
            if(m_counter1 == 0)
                m_counter2 = 0;
        }
 //       Serial.print("C1: ");
 //       Serial.print(m_counter1);
 //       Serial.print(" :C2: ");
 //       Serial.print(m_counter2);
 //       Serial.print(" :C3: ");
 //       Serial.println(m_counter3);
 //       Serial.println("   ");
 //       Serial.println(m_leds[0]->b);
    }

    virtual void animateBlink()
    {

    }

    virtual void animateCylon()
    {//contador 1 para la iteracion, 2 para la direccion
        int min = 0;
        int max = m_leds.size();
        int count = max - min;

        if(m_counter2 == 0)//incrementando
        {
            int i = 0;
            for(int l = min ; l < max ; l++ )
            {
                ws2812Strip::led *led = m_leds[l];
                if(i == m_counter1)
                {
                   led->setColor(255,0,0);
                }
                else
                {
                    led->dimm();
                }

                if(m_counter1 >= max)
                {
                    m_counter1 = max;
                    m_counter2 = 1;
                    return;
                }
                else
                {
                    i++;
                }
            }
            m_counter1++;
        }else //decrementando
        {
            int i = count;
            for(int l = max-1 ; l >= min ; l-- )
            {
                ws2812Strip::led *led = m_leds[l];
                if(i == m_counter1)
                {
                   led->setColor(255,0,0);
                }
                else
                {
                    led->dimm();
                }

                if(m_counter1 <= min)
                {
                    m_counter1 = 0;
                    m_counter2 = 0;
                    return;
                }
                else
                {
                    i--;
                }
            }
            m_counter1--;
        }
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
        if(!m_enabled) return;
        ledBar::animate();

        if(m_statusDecay)
            m_controllerLed->dimm();
        if(m_wifiStatusDecay)
            m_wifiLed->dimm();
        if(m_extraLedDecay)
            m_extraLed->dimm();
        m_strip->update();
    }

    void controllerInit()
    {
        m_statusDecay = false;
        m_controllerLed->setColor(0,100,100);
    }

    void controllerOK()
    {
        m_statusDecay = true;
        m_controllerLed->setColor(0,100,0);
    }

    void controllerError()
    {
        m_statusDecay = false;
        m_controllerLed->setColor(100,0,0);
    }

    void wifiTX()
    {
        m_wifiStatusDecay = true;
        m_wifiLed->setColor(0,100,0);
    }

    void wifiRX()
    {
        m_wifiStatusDecay = true;
        m_wifiLed->setColor(100,0,0);
    }

    void wifiInit()
    {
        m_wifiStatusDecay = false;
        m_wifiLed->setColor(100,100,0);
    }

    void wifiClient()
    {
        m_wifiStatusDecay = false;
        m_wifiLed->setColor(0,100,100);
        m_wifiStatusColor = *m_wifiLed;
    }

    void wifiAutoConfig()

    {
        m_wifiStatusDecay = false;
        m_wifiLed->setColor(100,0,100);
        m_wifiStatusColor = *m_wifiLed;
    }

    void wifiOK()
    {
        *m_wifiLed = m_wifiStatusColor;
    }

    void wifiError()
    {
        m_wifiStatusDecay = false;
        m_wifiLed->setColor(100,0,0);
    }

    void extraLedOK()
    {
        m_extraLed->setColor(0,200,0);
    }

    void extraLedIdle()
    {
        m_extraLed->setColor(0,0,200);
    }

    void extraLedError()
    {
        m_extraLed->setColor(200,0,0);
    }

    void extraLedSetDecay(bool decay)
    {
      m_extraLedDecay = decay;
    }

  private:
    ws2812Strip::led*   m_controllerLed;
    ws2812Strip::led*   m_wifiLed;
    ws2812Strip::led*   m_extraLed;
    ws2812Strip::led    m_wifiStatusColor;

    bool                m_statusDecay     = false;
    bool                m_wifiStatusDecay = false;
    bool                m_extraLedDecay   = false;
};


class ledMatrix : public ledBar
{
public:
    ledMatrix(uint8_t firstLednr, uint8_t cols, uint8_t rows, ws2812Strip* parentStrip,bool mirror = false, bool invertEachRow = false) : ledBar(parentStrip)
    {
        for(int r = 0 ; r < rows ; r++)
        {
            m_matrix.push_back(std::vector<ws2812Strip::led*>());
            bool invertedRow = invertEachRow&&(r % 2 != 0);
            if(mirror) invertedRow = !invertEachRow;

            int start = r*cols+firstLednr;
            int count = cols;

            if(invertedRow)
            {
                for(int i = start+count-1 ; i >= start ; i-- )
                {
                    ws2812Strip::led* l = m_strip->getLed(i);
                    m_leds.push_back(l);
                    m_matrix[r].push_back(l);
                }
            }
            else
            {
                for(int i = start ; i < start+count ; i++ )
                {
                    ws2812Strip::led* l = m_strip->getLed(i);
                    m_leds.push_back(l);
                    m_matrix[r].push_back(l);
                }
            }
        }
        m_id = "ws2812Matrix-";
        m_id +=cols;
        m_id +="x";
        m_id += rows;
    }

protected:
    std::vector<std::vector<ws2812Strip::led*> >  m_matrix;
    void animateCylon()
    {
        setLeds(ledMatrixIcons::cylonIcon16x16());
        m_animationType = animationCylon;
        //contador 1 para la iteracion, 2 para la direccion
        int min = 33;
        int max = 47;
        int count = max - min;
        //Serial.println(m_counter1);
        if(m_counter2 == 0)//incrementando
        {
            int i = 0;
            for(int l = min ; l < max ; l++ )
            {
                if(i == m_counter1)
                {
                   m_leds[l]->setColor(255,0,0);
                   m_leds[l+16]->setColor(255,0,0);
                   m_leds[l+32]->setColor(255,0,0);
                }
                else
                {
                    m_leds[l]->dimm();
                }

                if(m_counter1 >= count)
                {
                    m_counter1 = count;
                    m_counter2 = 1;
                    return;
                }
                else
                {
                    i++;
                }
            }
            m_counter1++;
        }else //decrementando
        {
            int i = count;
            for(int l = max-1 ; l >= min ; l-- )
            {
                if(i == m_counter1)
                {
                    m_leds[l]->setColor(255,0,0);
                    m_leds[l+16]->setColor(255,0,0);
                    m_leds[l+32]->setColor(255,0,0);                        }
                else
                {
                    m_leds[l]->dimm();
                }

                if(m_counter1 <= 0)
                {
                    m_counter1 = 0;
                    m_counter2 = 0;
                    return;
                }
                else
                {
                    i--;
                }
            }
            m_counter1--;
        }
    }
};

#endif
