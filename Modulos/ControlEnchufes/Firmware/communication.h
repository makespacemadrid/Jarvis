#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#endif

#include "ws2812led.h"


class communicationModule
{
  public:
    communicationModule(bool bridgeMode) : m_bridge(bridgeMode) {};

    void setAP(String essid,String pass, uint8_t channel = 6)      
    {
     if(m_status_led) 
        m_status_led->setColor(10,10,0);

      m_essid = essid, m_pass = pass, m_channel = channel;
      connectAP();
    }

    void setStation(String essid,String pass)
    {
      if(m_status_led) 
        m_status_led->setColor(10,10,0);

      m_essid = essid, m_pass = pass;
      connectStation();
    }

    void setBridge(bool enabled)
    {
      m_bridge = enabled;
    }
    
    virtual void update()
    {
      read();
      parseBuffer(m_rxBuffer);
    }
    //void setMacAddr();
    void setStatusLed(ws2812Strip::led* statusLed) 
    {
      m_status_led= statusLed;
      m_status_led->setColor(0,0,10);
      
    }
    virtual bool isConnected()    = 0;
    virtual void setup()          = 0;
    virtual void send(String str) = 0;
    virtual void read()           = 0;
  protected:
    bool     m_bridge = false;
    
    String   m_rxBuffer;

    String   m_id;

    String   m_essid;
    String   m_mac_addr;
    String   m_pass;
    uint8_t  m_channel;

    uint16_t m_localPort = 180;
    String   m_remotehost;
    uint16_t m_remotePort;

    ws2812Strip::led*     m_status_led = 0;
    
    virtual void connectAP()      = 0;
    virtual void connectStation() = 0;     

    void parseBuffer(String& buf)
    {
      //Serial.print(buf);
      int index = buf.indexOf("\n");
      while(index >= 0)
      {
        String line = buf.substring(0,index);
        parseCommand(line);
        buf = buf.substring(index+1);
        index = buf.indexOf("\n");
      }
    }
    void parseCommand(String& line)
    {
      //Serial.print(line);
      if(m_bridge && (!line.startsWith("ESP"))) return; //En modo puente solo hace caso a los comandos del modo esp
      if(line.startsWith("ESP:")) parseESPCommand(line.substring(4));
      //else if(line.startsWith("ESP:"))
      //else if(line.startsWith("ESP:"))
    }

    void parseESPCommand(String cmd)
    {
      //Serial.println("Comando ESP");
      if       (cmd.startsWith("AP:"))
      {
        //Serial.println("AP");
        int index = cmd.indexOf(":",3);
        //if(!index >= 0) return;
        String essid = cmd.substring(3,index);
        String pass = cmd.substring(index+1);
        Serial.println("a:"+essid+ " p:"+pass);
        setAP(essid,pass);
        
      } else if(cmd.startsWith("CLIENT:"))
      {
        int index = cmd.indexOf(":",7);
        //if(!index >= 0) return;
        String essid = cmd.substring(7,index);
        String pass = cmd.substring(index+1);
        Serial.println("\na:"+essid+ " p:"+pass);
        setStation(essid,pass);        
      } else if(cmd.startsWith("CSTATUS"))
      {
        
        
      }
    }
};

#ifdef ESP8266
class espNative : public communicationModule
{
  public:
    espNative(int localPort = 31416, bool bridge = false) :communicationModule(bridge), m_server(localPort) {;}
    bool isConnected(){return WiFi.status() != WL_CONNECTED;}

    void setup()
    {
      m_server.begin();
    }

    void update()
    {
      communicationModule::update();
      readSerial();
      parseBuffer(m_serialBuffer);
    }

    void send(String str)
    {
      int len = str.length();
      char sbuf[len];
      for(uint8_t i = 0; i < m_max_clients; i++){
        if (m_server_clients[i] && m_server_clients[i].connected()){
          m_server_clients[i].write(sbuf, len);
          if(m_status_led) 
            m_status_led->setColor(20,0,0);    
          delay(1);
        }
      }
    }

    void read()
    {
      if (m_server.hasClient()){
        for(uint8_t i = 0; i < m_max_clients; i++){
          //find free/disconnected spot
          if (!m_server_clients[i] || !m_server_clients[i].connected()){
            if(m_server_clients[i]) m_server_clients[i].stop();
            m_server_clients[i] = m_server.available();
          }
        }
        //no free/disconnected spot so reject
        WiFiClient serverClient = m_server.available();
        serverClient.stop();
      }
      //check clients for data
      for(uint8_t i = 0; i < m_max_clients; i++){
        if (m_server_clients[i] && m_server_clients[i].connected()){
          if(m_server_clients[i].available()){
            //get data from the telnet client and push it to the UART
            if(m_status_led && m_server_clients[i].available())
              m_status_led->setColor(0,20,0);
            while(m_server_clients[i].available())
            {
              char b = m_server_clients[i].read();
              if(m_bridge)Serial.write(b);
              m_rxBuffer += b;
            }
          }
        }
      }
    }

    void readSerial()
    {
      if(m_status_led && Serial.available())
          m_status_led->setColor(0,20,0);
      while(Serial.available())
      {
        size_t len = Serial.available();
        //Serial.print("s:");
        //Serial.println(len);
        char sbuf[len];
        Serial.readBytes(sbuf, len);
        m_serialBuffer += sbuf;
        if(m_bridge)
        {//push UART data to all connected telnet clients
          for(int i = 0; i < m_max_clients; i++)
          {
            if (m_server_clients[i] && m_server_clients[i].connected())
            {
              m_server_clients[i].write(sbuf, len);
              delay(1);
            }      
          }
        }
      } 
    }
    
  private:
    String     m_serialBuffer;
    uint8_t    m_max_clients =2;
    String     m_serialBufer;
    WiFiServer m_server;
    WiFiClient m_server_clients[2];
    void connectAP()
    {
      char* essid  = new char[20];
      char* pass   = new char[20];
      m_essid.toCharArray(essid,20);
      m_pass.toCharArray(pass,20);
      Serial.print("Creando AP: ");
      Serial.print(essid);
      Serial.print(" ");
      Serial.println(pass);
      WiFi.softAP(essid, pass);
      delete[] pass;
      delete[] essid;
    }
    void connectStation()
    {
      //WTF! no hay cosa mas odiosa que los punteros a char....
      // parece ser que ("pollas en vinagre") != (char* variable = "pollas en vinagre") != (char[] array = pollas en vinagre) != (String str("pollas en vinagre"))
      char* essid = new char[40];
      char* pass  = new char[40];
      m_essid.toCharArray(essid,40);
      m_pass.toCharArray(pass,40);
      Serial.print("Conectando a red: ");
      Serial.print(m_essid);
      Serial.print("#");
      Serial.print(essid);
      Serial.print("#");
      Serial.print(m_pass);
      Serial.print("#");
      Serial.print(pass);
      Serial.println("#");
      WiFi.begin(essid, pass);
        while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
      delete[] pass;
      delete[] essid;
    }
};
#endif

class espProxy : public communicationModule
{
  public:
    espProxy() : communicationModule(false) {;}
    bool isConnected() {;}
    void setup() {;}
    void send(String str) {;}
    void read() {;}
  private:
    void connectAP(){;}
    void connectStation(){;}
};
