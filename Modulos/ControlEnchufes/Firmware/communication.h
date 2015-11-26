#ifndef COMM
#define COMM

#define I2C_TRANSPORT

#include <Wire.h>

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include "webconfigurator.h"
#include <WiFiClient.h> 
#endif
#include "jarvisParser.h"
#include "ws2812led.h"

//Status del wifi:
//WL_CONNECTED   3
//WL_IDLE_STATUS  0
//WL_NO_SSID_AVAIL  1
//WL_CONNECT_FAILED   4
//WL_CONNECTION_LOST  5
//WL_DISCONNECTED   6



class communicationModule : public jarvisParser
{
  public:
    communicationModule(int localPort, bool bridgeMode =false) : jarvisParser(),  m_bridge(bridgeMode) , m_localPort(localPort) {};

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

    virtual void setBridge(bool enabled)
    {
      m_bridge = enabled;
      if(enabled)
        debugln(String(F("D:Bridge enabled")));
      else
        debugln(String(F("D:Bridge disabled")));
    }

    virtual void setup()
    {
      if(m_bridge)
      {// Si estamos en modo puente dejamos que sea el arduino el que haga el bucle de conexion.
        return;
      }
      int i = 0;
      while (connectionStatus() != 3)
      {
        debugln(F("D:Waiting for connection..."));
        if(i == 300)
          break;
        update();//al hacer el update duerme 50ms;
        i++;
      }
      if(connectionStatus() != 3)
      {
        setAP(F("ConfigureMe"),F("configureme"));
        debugln(String(F("I:ConfigurationAP:")));
      }
      else
      {
        debugln(String(F("I:Wificlient:")));
      }
      debug(localIP());
      debug(String(F("  p:")));
      debugln(m_localPort);
    }
    
    virtual void update()
    {
      read();
      parseBuffer(m_rxBuffer);
    }

    void setStatusLed(ws2812Strip::led* statusLed) 
    {
      m_status_led= statusLed;
      m_status_led->setColor(0,0,10); 
    }

    void softReset()
    {
      debugln(String(F("I:RESET!")));
      delay(5);
      #ifdef ESP8266
      ESP.reset();
        //ESP.wdtEnable(WDTO_15MS);
        //while(true); // Al meter el programa en un bucle se fuerza a que el watchdog salte y haga un reset del micro
      #else
        //wdt_enable(WDTO_15MS); // En arduino parece no funcionar como se espera...WTF
        asm volatile ("  jmp 0");
      #endif
    }

    virtual String localIP()        = 0;
    virtual bool isConnected()      = 0;
    virtual int  connectionStatus() = 0;
    virtual void send(String str)   = 0;
    virtual void read()             = 0;
  protected:
    bool     m_bridge;
    
    String   m_rxBuffer;

    String   m_id;

    String   m_essid;
    String   m_mac_addr;
    String   m_pass;
    uint8_t  m_channel;

    int      m_localPort;
    String   m_remotehost;
    int      m_remotePort;

    ws2812Strip::led*  m_status_led = 0;
    
    virtual void connectAP()      = 0;
    virtual void connectStation() = 0;

    void debug(String str)
    {
      Serial.print(str);
    }
    void debugln(String str)
    {
      Serial.println(str);
    }

    void debug(int number)
    {
      Serial.print(number);      
    }
    void debugln(int number)
    {
      Serial.println(number);
    }

};

#ifdef ESP8266
class espNative : public communicationModule
{
  public:
    espNative(int localPort, bool bridge = false) :communicationModule(localPort,bridge), m_server(localPort), m_webServer(80) {;}

    bool isConnected()      {return WiFi.status() != WL_CONNECTED;}

    int connectionStatus()  {return WiFi.status();}

    void setup()
    {
      communicationModule::setup();
      m_webServer.setup();
      m_server.begin();
      m_server.setNoDelay(true);
      #ifdef I2C_TRANSPORT
      Wire.begin(0,2);// guardar los pines i2c en la eeprom!
      #endif
    }

    void update()
    {
      communicationModule::update();
      manageClients();
      m_webServer.handleClient();
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
            debugln(String(F("D:New client")));
            continue;
          }
        }
        //no free/disconnected spot so reject
        WiFiClient serverClient = m_server.available();
        serverClient.stop();
    }
  }      


    void send(String str)//envia por wifi
    {
      int len = str.length();
      char sbuf[len+1];
      str.toCharArray(sbuf,len+1);
      for(uint8_t i = 0; i < m_max_clients; i++){
        if (m_server_clients[i] && m_server_clients[i].connected()){
          m_server_clients[i].write(sbuf, len);
          if(m_status_led) 
            m_status_led->setColor(20,0,0);    
          delay(1);
        }
      }
    }

    void read()//lee desde el wifi
    {
      //check clients for data
      for(uint8_t i = 0; i < m_max_clients; i++){
        if (m_server_clients[i] && m_server_clients[i].connected()){
          if(m_server_clients[i].available()){
            //get data from the telnet client and push it to the UART
            if(m_status_led && m_server_clients[i].available())
              m_status_led->setColor(0,20,0);
            String buff;
            while(m_server_clients[i].available())
            {
              char b = m_server_clients[i].read();
              if(m_bridge)Serial.write(b);
              buff += b;
            }
            m_rxBuffer += buff;
          }
        }
      }
    }
#ifdef I2C_TRANSPORT

    void sendSerial(String str)
    {
      Serial.print(str);
    }

    void readSerial()
    {
    
    }

#else
    void sendSerial(String str)
    {
      Serial.print(str);
    }

    void readSerial()
    {
      if(m_status_led && Serial.available())
          m_status_led->setColor(0,20,0);
      while(Serial.available())
      {
        size_t len = Serial.available();
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
#endif

    String localIP()
    {
      IPAddress local = WiFi.localIP();
      String result;
      result += local[0];
      result += ".";
      result += local[1];
      result += ".";
      result += local[2];
      result += ".";
      result += local[3];
      return result;
    }
    
  protected:
    String           m_serialBuffer;
    uint8_t          m_max_clients =2;
    String           m_serialBufer;
    WiFiServer       m_server;
    WiFiClient       m_server_clients[2];
    webConfigurator  m_webServer;

    void processEspMsg(std::vector<String>& args)
    {
      if(args.size()<=0) return;
      
      if     (args[0] == C_SETAP)
      {
        if(args.size() != 3) return;
        setAP(args[1],args[2]);
      } 
      else if(args[0] == C_SETCLIENT)
      {
        if(args.size() != 3) return;
        setStation(args[1],args[2]);
      } 
      else if(args[0] == C_WSTATUS)
      {
        std::vector<String> args;
        args.push_back(C_WSTATUS);
        args.push_back(String(connectionStatus()));
        sendSerial(encodeEspMsg(args));
      }
      else if(args[0] == C_LOCALIP)
      {
        std::vector<String> args;
        args.push_back(C_LOCALIP);
        args.push_back(localIP());
        sendSerial(encodeEspMsg(args));
      }
      else if(args[0] == C_RESET)
      {
        softReset();
      }
      else if(args[0] == C_BRIDGEMODE)
      {
        if(args.size() != 2) return;
        if(args[1] == C_ENABLE)
          setBridge(true);
        else
          setBridge(false);
      }
    }
    
    void connectAP()
    {
      char* essid  = new char[m_essid.length()+1];
      char* pass   = new char[m_pass.length()+1];
      m_essid.toCharArray(essid,m_essid.length()+1);
      m_pass.toCharArray(pass,m_pass.length()+1);
      debug(String(F("D:AP: ")));
      debug(essid);
      debug(String(F("#")));
      debugln(pass);
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
      debug(String(F("D:AP: ")));
      debug(essid);
      debug(String(F("#")));
      debugln(pass);
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
    espProxy(int localPort,bool bridge=false) : communicationModule(localPort,bridge) {;}
    bool isConnected()      {return m_lastStatus == 3;}

    int  connectionStatus() 
    {
      updateConnectionStatus();
      return m_lastStatus;
    }
    
    void setup()
    {
      setBridge(true);
      #ifdef I2C_TRANSPORT
      Wire.begin(8);
      Wire.onRequest(sendi2c);
      Wire.onReceive(readi2c);
      #endif
      communicationModule::setup();
    }
    
#ifdef I2C_TRANSPORT
    static void sendi2c()
    {
      
    }

    static void readi2c(int count)
    {

    }
    
    void send(String str)
    {
      Serial.print(str);
    }

    void read()
    {
    }

#else
    void send(String str)
    {
      Serial.print(str);
    }

    void read()
    {
      if(m_status_led && Serial.available())
          m_status_led->setColor(0,20,0);
      while(Serial.available())
      {
        size_t len = Serial.available();
        char sbuf[len];
        Serial.readBytes(sbuf, len);
        m_rxBuffer += sbuf;
      }
    }
#endif

    virtual void update()
    {
      communicationModule::update();
    }
    
    String localIP()
    {
      updateConnectionStatus();
      delay(5);
      return m_localIP;
    }
    
  protected:
    int     m_lastStatus = 6;
    String  m_localIP = "0";
    
    void processEspMsg(std::vector<String>& args)
    {
      if(args.size()<=0) return;
      
      if(args[0] == C_WSTATUS)
      {
        if(args.size() != 2) return;
        m_lastStatus = args[1].toInt();
      }
      else if(args[0] == C_LOCALIP)
      {
        if(args.size() != 2) return;
        m_localIP = args[1];
      }
    }

    void updateConnectionStatus()
    {
      std::vector<String> args;
      args.push_back(C_WSTATUS);
      send(encodeEspMsg(args));
      args.clear();
      args.push_back(C_LOCALIP);
      send(encodeEspMsg(args));
      delay(5);
      update();
    }
    
    void connectAP()
    {
      std::vector<String> args;
      args.push_back(C_SETAP);
      args.push_back(m_essid);
      args.push_back(m_pass);
      send(encodeEspMsg(args));
    }
    void connectStation()
    {
      std::vector<String> args;
      args.push_back(C_SETCLIENT);
      args.push_back(m_essid);
      args.push_back(m_pass);
      send(encodeEspMsg(args));
    }

    void setBridge(bool enabled)
    {
      std::vector<String> args;
      args.push_back(C_BRIDGEMODE);

      if(enabled)
        args.push_back(C_ENABLE);
      else
        args.push_back(C_DISABLE);

      send(encodeEspMsg(args));
    }
};

#endif
