#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#endif

//Status del wifi:
//WL_CONNECTED   3
//WL_IDLE_STATUS  0
//WL_NO_SSID_AVAIL  1
//WL_CONNECT_FAILED   4
//WL_CONNECTION_LOST  5
//WL_DISCONNECTED   6

//Configuracion del parseador. Formato de paquete :
//-COMANDO:ARG:ARG\n
// Ej: -ESP:WifiStatus\n
#define P_PACKETSTART  "-"
#define P_PACKETEND    "\n"
#define P_SEPARATOR    ":"


#include "ws2812led.h"

class communicationModule
{
  public:
    communicationModule(uint16_t localPort = 180, bool bridgeMode =false) : m_bridge(bridgeMode) , m_localPort(localPort) {};

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
    virtual bool isConnected()      = 0;
    virtual int  connectionStatus() = 0;
    virtual void setup()            = 0;
    virtual void send(String str)   = 0;
    virtual void read()             = 0;
    virtual IPAddress localIP()     = 0;
  protected:
    bool     m_bridge = false;
    
    String   m_rxBuffer;

    String   m_id;

    String   m_essid;
    String   m_mac_addr;
    String   m_pass;
    uint8_t  m_channel;

    uint16_t m_localPort;
    String   m_remotehost;
    uint16_t m_remotePort;

    ws2812Strip::led*  m_status_led = 0;
    
    virtual void connectAP()      = 0;
    virtual void connectStation() = 0;     

    void parseBuffer(String& buf)
    {
      if(buf.length() == 0) return;
      int s_index = buf.indexOf(P_PACKETSTART);
      int e_index = buf.indexOf(P_PACKETEND);
      //saneado del buffer
      if      (s_index > 0)
      {
        Serial.print("D:Basurilla en el buffer:");
        Serial.println(buf.substring(0,s_index));
      } 
        else if(s_index < 0)
      {
        Serial.print("D:Basurilla en el buffer:");
        Serial.println(buf);
        buf = "";
        Serial.print("D:Buffer purgado");
        return;
      }
      //extraccion de comandos
      while ((s_index >= 0) && (e_index >= 0)) //Si hay inicio y fin de paquete se extrae el comando.
      {
        String line = buf.substring(s_index+1,e_index);
        parseCommand(line);
        buf = buf.substring(e_index+1);
        s_index = buf.indexOf(P_PACKETSTART);
        e_index = buf.indexOf(P_PACKETEND);
      }
    }

    void parseCommand(String line)
    {
      Serial.print("D:comando:");
      Serial.println(line);
      if(m_bridge && (!line.startsWith("ESP"))) return; //En modo puente solo hace caso a los comandos del modo esp
      if(line.startsWith("ESP")) parseESPCommand(line.substring(4));
      //else if(line.startsWith("ESP:"))
      //else if(line.startsWith("ESP:"))
    }

    void parseESPCommand(String cmd)
    {
      //Serial.println("Comando ESP");
      if       (cmd.startsWith("AP"))
      {
        int index = cmd.indexOf(P_SEPARATOR,3);
        //if(!index >= 0) return;
        String essid = cmd.substring(3,index);
        String pass = cmd.substring(index+1);
        setAP(essid,pass);
      } 
      else if(cmd.startsWith("Client:"))
      {
        int index = cmd.indexOf(P_SEPARATOR,7);
        //if(!index >= 0) return;
        String essid = cmd.substring(7,index);
        String pass = cmd.substring(index+1);
        setStation(essid,pass);
      } 
      else if(cmd.startsWith("WifiStatus"))
      {
        Serial.print("WifiStatus:");
        Serial.println(connectionStatus());
      }
      else if(cmd.startsWith("LocalIP"))
      {
        Serial.print("LocalIP: ");
        Serial.println(localIP());
      }
    }
};

#ifdef ESP8266
class espNative : public communicationModule
{
  public:
    espNative(uint16_t localPort = 180, bool bridge = false) :communicationModule(bridge,localPort), m_server(localPort) {;}

    bool isConnected()      {return WiFi.status() != WL_CONNECTED;}

    int connectionStatus()  {return WiFi.status();}

    IPAddress localIP()     {return WiFi.localIP();}

    void setup()
    {
      int i = 0;
      while (WiFi.status() != WL_CONNECTED)
      {
        if(i == 1000)
          break;
        delay(50);
        i++;
      }
      if(WiFi.status() != WL_CONNECTED)
      {
        Serial.println("D:no hay conexion,lanzando AP");
        setAP("Configureme","configureme");
        delay(100);
        Serial.print("I:WifiAP:");
        Serial.println(WiFi.localIP());
      } 
      else
      {
        Serial.print("I:Wificlient:");
        Serial.println(WiFi.localIP());
      }

      m_server.begin();
      m_server.setNoDelay(true);
    }

    void update()
    {
      communicationModule::update();
      manageClients();  
      readSerial();
      parseBuffer(m_serialBuffer);    
    }

    void manageClients()
    {
      if (m_server.hasClient()){
        for(uint8_t i = 0; i < m_max_clients; i++){
          //find free/disconnected spot
          if (!m_server_clients[i] || !m_server_clients[i].connected()){
            if(m_server_clients[i]) m_server_clients[i].stop();
            m_server_clients[i] = m_server.available();
            Serial1.print("D:New client");
            continue;
          }
        }
        //no free/disconnected spot so reject
        WiFiClient serverClient = m_server.available();
        serverClient.stop();
    }
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
      //if (m_server.hasClient()){
      //  for(uint8_t i = 0; i < m_max_clients; i++){
      //    //find free/disconnected spot
      //    if (!m_server_clients[i] || !m_server_clients[i].connected()){
      //      if(m_server_clients[i]) m_server_clients[i].stop();
      //      m_server_clients[i] = m_server.available();
      //    }
      //  }
        //no free/disconnected spot so reject
        //WiFiClient serverClient = m_server.available();
        //serverClient.stop();
      //}
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
      char* essid  = new char[m_essid.length()+1];
      char* pass   = new char[m_pass.length()+1];
      m_essid.toCharArray(essid,m_essid.length()+1);
      m_pass.toCharArray(pass,m_pass.length()+1);
      Serial.print("D:AP: ");
      Serial.print(essid);
      Serial.print("#");
      Serial.println(pass);
      WiFi.disconnect();
      delay(1000);
      WiFi.softAP(essid, pass);
      delay(500);
      m_server.begin();
      delete pass;
      delete essid;
    }
    
    void connectStation()
    {
      char* essid = new char[m_essid.length()+1];
      char* pass  = new char[m_pass.length()+1];
      m_essid.toCharArray(essid,m_essid.length()+1);
      m_pass.toCharArray(pass,m_pass.length()+1);
      Serial.print("D:Client: ");
      Serial.print(essid);
      Serial.print("#");
      Serial.print(pass);
      WiFi.disconnect();
      delay(10);
      WiFi.begin(essid, pass);
      delay(1000);
      m_server.begin();
      delete pass;
      delete essid;
    }
};
#endif

class espProxy : public communicationModule
{
  public:
    espProxy() : communicationModule(false) {;}
    bool isConnected()      {;}
    int  connectionStatus() {;}
    void setup()            {;}
    void send(String str)   {;}
    void read()             {;}
    IPAddress localIP()     {;}
  private:
    void connectAP(){;}
    void connectStation(){;}
};
