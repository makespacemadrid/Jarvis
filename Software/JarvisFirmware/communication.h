#ifndef COMM
#define COMM

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

extern uint8_t updateInterval;


class communicationModule : public jarvisParser
{
  public:

  enum commModes
  {
      nativeNode,
      espRepeater,
      arduinoThoughProxy
  };
    
    communicationModule(commModes cmode, int localPort, int ledStripPin) : jarvisParser() , m_localPort(localPort),
    m_commMode(cmode), m_ledStrip(ledStripPin), m_statusLed(&m_ledStrip,0,1,2) {} ;

    void setAP(String essid,String pass, uint8_t channel = 6)
    {

      m_essid = essid, m_pass = pass, m_channel = channel;
      connectAP();
    }

    void setStation(String essid,String pass)
    {

      m_essid = essid, m_pass = pass;
      connectStation();
    }

    virtual void setBridge(bool enabled)
    {
      m_bridge = enabled;
#ifdef DEBUG_STRINGS
      if(enabled)
        debugln(String(F("D:Bridge enabled")));
      else
        debugln(String(F("D:Bridge disabled")));
#endif
    }

    virtual void setup()
    {
      if(m_commMode == espRepeater)
      {// Si estamos en modo puente dejamos que sea el arduino el que haga el bucle de conexion.
        return;
      }
      m_statusLed.wifiInit();
      int i = 0;
      while (connectionStatus() != 3)
      {
#ifdef VERBOSE_DEBUG
        debug(F("."));
#endif
        if(i >= 15000/(updateInterval+1)) // Esperamos 15 segundos para que se establezca la conexion por defecto
          break;
        update();;
        i++;
      }
      if(connectionStatus() != 3)
      {
        setAP(F("ConfigureMe"),F("configureme"));
#ifdef VERBOSE_DEBUG
        debugln(String(F("I:ConfigurationAP:")));
#endif
        m_statusLed.wifiAutoConfig();
      }
      else
      {
#ifdef VERBOSE_DEBUG
        debugln(String(F("I:Wificlient:")));
#endif
        m_statusLed.wifiClient();
      }
#ifdef VERBOSE_DEBUG
      debug(localIP());
      debug(String(F("  p:")));
      debugln(m_localPort);
#endif
    }
    
    virtual void update()
    {
      read();
      parseBuffer(m_rxBuffer);
      if(connectionStatus() == 3)
          m_statusLed.wifiOK();
      else
          m_statusLed.wifiError();
    }

    void softReset()
    {
      debugln(String(F("\nRESET\n")));
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

    uint16_t getFreeMem()
    {
      #ifdef ESP8266
      return system_get_free_heap_size();
      #else
      return freeMemory();
      #endif    
    }

    virtual String localIP()        = 0;
    virtual bool isConnected()      = 0;
    virtual int  connectionStatus() = 0;
    virtual void send(String str)   = 0;
    virtual void read()             = 0;

  protected:
    commModes m_commMode;

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

    ws2812Strip           m_ledStrip;//(m_EEPROM.settings().ledStripPin);
    ledStatusTrio         m_statusLed;
    
    virtual void connectAP()      = 0;
    virtual void connectStation() = 0;


//parseado del protocolo

//protocolo ESP
    virtual void processEspMsg(std::vector<String>& args)
    {
      if(args.size()<=0) return; // si no hay nada que procesar no hacemos nada

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
        send(encodeEspMsg(args));
      }
      else if(args[0] == C_LOCALIP)
      {
        std::vector<String> args;
        args.push_back(C_LOCALIP);
        args.push_back(localIP());
        send(encodeEspMsg(args));
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
        else if(args[1] == C_DISABLE)
          setBridge(false);
      }
      else if(args[0] == C_PING)
      {
        std::vector<String> args;
        args.push_back(C_PONG);
        send(encodeEspMsg(args));
      }
    }
//Protocolo de nodo
    virtual void processNodeMsg(std::vector<String>& args)
    {
        if(args.size()<=0) return; // si no hay nada que procesar no hacemos nada
        if(m_commMode == espRepeater) return; //En modo repetidor no escuchamos este protocolo

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
          send(encodeNodeMsg(args));
        }
        else if(args[0] == C_LOCALIP)
        {
          std::vector<String> args;
          args.push_back(C_LOCALIP);
          args.push_back(localIP());
          send(encodeNodeMsg(args));
        }
        else if(args[0] == C_RESET)
        {
          softReset();
        }
        else if(args[0] == C_PING)
        {
          std::vector<String> args;
          args.push_back(C_PONG);
          send(encodeNodeMsg(args));
        }
        else if(args[0] == C_GETID)
        {
            sendId();
        }
        else if(args[0] == C_GETCOMPONENTS)
        {
            sendComponents();
        }
        else if(args[0] == C_READSENSOR)
        {
            if(args.size() != 2 ) return;
            sendSensor(args[1]);
        }
        else if(args[0] == C_READSENSORS)
        {
            sendSensors();
        }
        else if(args[0] == C_DOACTION)
        {
            args.erase(args.begin());
            doAction(args);
        }else if(args[0] == C_GET_FREEM)
        {
           sendFreeMem();
        }
    }
//Funciones para facilitar las repuestas:
    virtual void sendComponents()                   = 0;
    virtual void sendSensors()                      = 0;
    virtual void sendSensor(String id)              = 0;
    virtual void doAction(std::vector<String> args) = 0;

    void sendEvent(String source,jarvisEvents event)
    {
      std::vector<String> args;
      args.push_back(E_EVENT);
      args.push_back(source);
      args.push_back(String(event));
      send(encodeJarvisMsg(args));
    }

    void sendId()
    {
        std::vector<String> args;
        args.push_back(C_ID);
        args.push_back(m_id);
        send(encodeJarvisMsg(args));
    }

    void sendFreeMem()
    {
        std::vector<String> args;
        args.push_back(C_FREEMEM);
        args.push_back(String(getFreeMem()));
        send(encodeJarvisMsg(args));      
    }

//funciones para redireccionar el debug
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
    void debug(float number)
    {
      Serial.print(number);      
    }
    void debugln(float number)
    {
      Serial.println(number);
    }
};

#ifdef ESP8266
class espNative : public communicationModule
{
  public:
    espNative(commModes mode, int localPort, int ledStripPin) :communicationModule(mode,localPort,ledStripPin), m_server(localPort), m_webServer(80)
    {
        if(mode == nativeNode)
            m_bridge = false;
        else
            m_bridge = true;
    }

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
      m_statusLed.wifiTX();
      int len = str.length();
      char sbuf[len+1];
      str.toCharArray(sbuf,len+1);
      for(uint8_t i = 0; i < m_max_clients; i++){
        if (m_server_clients[i] && m_server_clients[i].connected()){
          m_server_clients[i].write(sbuf, len);   
          delay(1);
        }
      #ifdef EXTRA_CARRIAGE_RETURN
      m_server_clients[i].write("\n", 1);   
      #endif
      }
    }

    void read()//lee desde el wifi
    {
      //check clients for data
      for(uint8_t i = 0; i < m_max_clients; i++){
        if (m_server_clients[i] && m_server_clients[i].connected()){
          if(m_server_clients[i].available()){
            //get data from the telnet client and push it to the UART
            if(m_server_clients[i].available())
              m_statusLed.wifiRX();
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
    espProxy(int localPort, int ledStripPin, commModes mode = arduinoThoughProxy ) : communicationModule(mode, localPort,ledStripPin) {;}
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
        m_statusLed.wifiTX();
        Serial.print(str);
    }

    void read()
    {
      if(Serial.available())
          m_statusLed.wifiRX();
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
