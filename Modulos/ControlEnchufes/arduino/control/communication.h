#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif

class communicationModule
{
  public:
    communicationModule() {};

    void setAP(String essid, String pass, uint8_t channel = 6)      
    {
      m_essid = essid, m_pass = pass, m_channel = channel;
      connectAP();
    }

    void setStation(String essid, String pass, uint8_t channel = 6)
    {
      m_essid = essid, m_pass = pass, m_channel = channel;
      connectAP();
    }

    void update()
    {
      read();
      parseBuffer();
    }
    //void setMacAddr();

    virtual bool isConnected()    = 0;
    virtual void setup()          = 0;
    virtual void send(String str) = 0;
    virtual void read()           = 0;
  protected:
    String   m_rxBuffer;

    String   m_id;

    String   m_essid;
    String   m_macAddr;
    String   m_pass;
    uint8_t  m_channel;

    uint16_t m_localPort = 180;
    String   m_remotehost;
    uint16_t m_remotePort;
    
    virtual void connectAP()      = 0;
    virtual void connectStation() = 0;     

    void parseBuffer()
    {
     ;
    }
};

#ifdef ESP8266
class espNative : public communicationModule
{
  public:
    espNative() {;}
    bool isConnected(){;}
    void setup(){;}
    void send(String str){;}
    void read(){;}
  private:
    void connectAP(){;}
    void connectStation(){;}
};
#endif

class espProxy : public communicationModule
{
  public:
    espProxy() {;}
    bool isConnected() {;}
    void setup() {;}
    void send(String str) {;}
    void read() {;}
  private:
    void connectAP(){;}
    void connectStation(){;}
};
