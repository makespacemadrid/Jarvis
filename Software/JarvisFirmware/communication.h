#ifndef COMM
#define COMM

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include "webconfigurator.h"
#include <WiFiClient.h> 
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#endif

#include "jarvisParser.h"
#include "ws2812led.h"
#include "settings.h"

//Status del wifi:
//WL_CONNECTED   3
//WL_IDLE_STATUS  0
//WL_NO_SSID_AVAIL  1
//WL_CONNECT_FAILED   4
//WL_CONNECTION_LOST  5
//WL_DISCONNECTED   6

extern uint8_t updateInterval;


class communicationModule : public jarvisParser , public nodeComponent
{
  public:

  enum commModes
  {
      nativeNode,
      espRepeater,
      arduinoThoughProxy
  };
    
    communicationModule(commModes cmode, int localPort, int ledStripPin, int ledNr = 25) : jarvisParser()
      , m_localPort(localPort), m_commMode(cmode), m_ledStrip(ledStripPin,ledNr), m_statusLed(&m_ledStrip,0,1,2)
    {
        if(m_ledStrip.isValid())
        {
          debugln(String(F("I:-WS2812")));
          m_ledStrip.setup();
          m_ledStrip.off();
          m_statusLed.controllerInit();
        }
    }

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
      m_lastConnectionStatus = connectionStatus();
      float i = 0;
      debugln("I:Init wifi conn...");
      while (connectionStatus() != 3)
      {
#ifdef VERBOSE_DEBUG
        debug(F("."));
#endif
        if(i >= 15) // Esperamos 15 segundos para que se establezca la conexion por defecto
          break;
        update();
        i+=updateInterval/1000.0;
      }

      if(connectionStatus() != 3)
      {
        String name = F("ConfigureMe-");
        name += String(ESP.getChipId());
        setAP(name,F("configureme"));
#ifdef VERBOSE_DEBUG
        debugln(String(F("I:ConfigAP:")));
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
      debug(String(F("D: IP:")));
      debug(localIP());
      debug(String(F("  p:")));
      debugln(m_localPort);
#endif
    }
    
    virtual void update()
    {
      read();
      parseBuffer(m_rxBuffer);
      yield();
      int connstatus = connectionStatus();
      if( connstatus != m_lastConnectionStatus)
      {
          m_lastConnectionStatus = connstatus;
          debug("WifiStatus:");
          debugln(connstatus);
          if(connstatus == 3)
          {
              debug(String(F("D: IP:")));
              debug(localIP());
              debug(String(F("  p:")));
              debugln(m_localPort);
              i_wifiConnected();
          }
          else if (connstatus == 0)
          {
              debug(String(F("D: IP:")));
              debug(localIP());
              debug(String(F("  p:")));
              debugln(m_localPort);
              i_wifiConnected();
          }
          else
          {
              debugln(String(F("D: Wifi disconnected")));
              i_wifiDisConnected();
          }
      }

      if(m_reconnectJarvis)
      {
          m_reconnectTimer -= updateInterval/1000.0;
          if((m_reconnectTimer < 0.0f) && (!validatingConn()))
          {
              connectToJarvis();
          }
      }
    }

    uint16_t bufferCount()
    {
      return m_rxBuffer.length();
    }

    void softReset()
    {
      debugln(String(F("\nRESET\n")));
      yield();
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

    //String   m_id;

    String   m_essid;
    String   m_pass;
    uint8_t  m_channel;

    int      m_localPort;
    String   m_remotehost;
    int      m_remotePort;

    uint8_t  m_lastConnectionStatus;
    bool     m_jarvisConnected = false;
    bool     m_reconnectJarvis = false;
    float    m_reconnectTimer  = 0.0f;

    ws2812Strip           m_ledStrip;//(m_EEPROM.settings().ledStripPin);
    ledStatusTrio         m_statusLed;
    
    virtual void connectAP()      = 0;
    virtual void connectStation() = 0;



    virtual void connectToJarvis()  = 0;
    virtual bool validatingConn()   = 0;

    void i_wifiConnected()
    {
        if(m_lastConnectionStatus == 3)
        {
            m_statusLed.wifiClient();
        }else{
            m_statusLed.wifiAutoConfig();
        }
        wifiConnected();
        connectToJarvis();
    }

    void i_wifiDisConnected()
    {
        m_statusLed.wifiError();
        wifiDisConnected();
    }

    void i_jarvisConnected()
    {
        m_jarvisConnected = true;
        m_reconnectJarvis = false;
        jarvisConnected();
    }

    void i_jarvisDisConnected()
    {
        m_jarvisConnected = false;
        jarvisDisConnected();
        if(isConnected())
        {
            debugln("D:Will reconnect in ~10s");
            m_reconnectJarvis = true;
            m_reconnectTimer = 10.0;
        }
        else
        {
            m_reconnectJarvis = false;
            debugln("D:Will NOT reconnect until wifi is connected");
        }
    }


    virtual void wifiConnected()        {;}
    virtual void wifiDisConnected()     {;}
    virtual void jarvisConnected()      {;}
    virtual void jarvisDisConnected()   {;}


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
        String command = args[0];
        args.erase(args.begin());
        //Serial.println(args[0]);

        if     (command == C_SETAP)
        {
          if(args.size() != 2) return;
          setAP(args[0],args[1]);
        }
        else if(command == C_SETCLIENT)
        {
          if(args.size() != 2) return;
          setStation(args[0],args[1]);
        }
        else if(command == C_WSTATUS)
        {
          std::vector<String> args;
          args.push_back(C_WSTATUS);
          args.push_back(String(connectionStatus()));
          send(encodeJarvisMsg(args));
        }
        else if(command == C_LOCALIP)
        {
          std::vector<String> args;
          args.push_back(C_LOCALIP);
          args.push_back(localIP());
          send(encodeJarvisMsg(args));
        }
        else if(command == C_RESET)
        {
          softReset();
        }
        else if(command == C_PING)
        {
          std::vector<String> args;
          args.push_back(C_PONG);
          send(encodeJarvisMsg(args));
        }
        else if(command == C_GETID)
        {
            sendId();
        }
        else if(command == C_GETCOMPONENTS)
        {
            sendComponents();
        }
        else if(command == C_POLLSENSOR)
        {
            if(args.size() == 1)
                pollSensor(args[0]);
            else if(args.size() == 2)
                pollSensor(args[0],args[1].toInt());
        }
        else if(command == C_POLLSENSORS)
        {
            if(args.size())
                pollSensors(args[0].toInt());
            else
                pollSensors();
        }else if(command == C_STOP_POLLING)
        {
            stopPolling();
        }else if(command == C_DOACTION)
        {
            processDoAction(args);
        }else if(command == C_GET_FREEM)
        {
           sendFreeMem();
        }else if(command == C_GET_PROTOCOL_VERSION)
        {
           sendProtocolVersion();
        }else if(command == C_SET_UPDATE_INTERVAL)
        {
            if(args.size() == 1)
                setUpdateInterval(args[0].toInt());
        }else{
            debug("D:Unknonw command:");
            debugln(args[0]);
        }
    }

    void setUpdateInterval(int nup)
    {
        updateInterval =nup;
    }

//Funciones para facilitar las repuestas:
    virtual void sendComponents()                   = 0;
    virtual void pollSensors(int delay = -1)          = 0;
    virtual void pollSensor(String id,int delay = -1) = 0;
    virtual void stopPolling()                      = 0;
    virtual void processDoAction(std::vector<String>& args) = 0;

    void sendEvent(String source,nodeComponent::event e)
    {
      std::vector<String> args;
      args.push_back(E_EVENT);
      args.push_back(source);
      args.push_back(String(e.jevent));
      for(int i = 0 ; i < e.args.size() ; i++)
        args.push_back(e.args[i]);

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

    void sendProtocolVersion()
    {
        std::vector<String> args;
        args.push_back(C_PROTOCOL_VERSION);
        args.push_back(String(PROTOCOL_VERSION));
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

#define ESP8266 //Descomentar para que el qt creator pueda hacer autocompletado en esta zona del codigo
//volver a comentar el terminar para que no interfiera con la compilacion para arduino.

#ifdef ESP8266
class espNative : public communicationModule
{
  public:
    espNative(commModes mode, int localPort, int ledStripPin,int ledNr = 25) :communicationModule(mode,localPort,ledStripPin,ledNr), m_server(localPort), m_webServer(80)
    {
        if(mode == nativeNode)
            m_bridge = false;
        else
            m_bridge = true;
    }

    bool isConnected()      { return WiFi.status() == WL_CONNECTED; }

    int connectionStatus()  { return WiFi.status(); }

    void setup()
    {
      communicationModule::setup();
      m_webServer.setup();
      m_server.begin();
      m_server.setNoDelay(true);
      #ifdef I2C_TRANSPORT
      Wire.begin(0,2);// guardar los pines i2c en la eeprom!
      #endif

      char c_id[sizeof(m_id)];
      m_id.toCharArray(c_id, sizeof(c_id));
      debugln("D: Starting mdns responder.");
      MDNS.begin(c_id);
      m_httpUpdater.setup(&m_webServer.webServer());
      MDNS.addService("http", "tcp", 80);
      debug("D: HTTPUpdateServer ready! Open http://");
      debug(m_id);
      debug(".local/update in your browser\n");
    }

    void update()
    {
      communicationModule::update();
      manageClients();
      m_webServer.handleClient();
      yield();
      readSerial();
      parseBuffer(m_serialBuffer);
      yield();
    }

    void connectToJarvis()
    {
        settingList s = EEPROMStorage::getSettings();
        debug("D:Connecting to ");
        debug(s.remoteHost);
        debug(":");
        debugln(s.remotePort);
        WiFiClient client;
        if (client.connect(s.remoteHost, s.remotePort))
        {
            debugln("D:Socket connected, validating client...");
            validateClient(client);
        }else{
            debugln("D:Can't connect!");
            m_reconnectJarvis = true;
            m_reconnectTimer = 10.0f;
        }

    }

    bool validatingConn()
    {
        return m_validatingConns.size() > 0;
    }

    void validateClient(WiFiClient conn)
    {
        m_validatingConns.push_back(conn);
        m_validateTimeout = 5.0;
    }

    void initConn(WiFiClient conn)
    {
        m_validatedConns.push_back(conn);

        std::vector<String> args;
        args.push_back(M_NODE_GREETING);
        conn.print(encodeJarvisMsg(args));
        args.clear();

        sendId();
        sendProtocolVersion();
        sendComponents();

        i_jarvisConnected();

    }

    void validateNewClients()
    {
        for(int i = (m_validatingConns.size() -1) ; i >= 0 ; i--)
        {
            if(m_validatingConns[i].connected())
            {
                if(!m_validatingConns[i].available()) continue;
                String last_char;
                last_char = (char)m_validatingConns[i].read();
                String buff;
                debugln("D:Validating client");
                while(m_validatingConns[i].available() && (last_char != P_PACKETTERMINATOR))
                {
                  yield();
                  last_char = (char)m_validatingConns[i].read();
                  buff += last_char;
                }
                buff.remove(buff.length()-1);
                std::vector<String> args = splitStr(buff,P_PACKETSEPARATOR);
                if((args.size() == 2) &&
                   (args[0] == M_NODEMSG) &&
                   (args[1] == M_JARVIS_GREETING) )
                {
                    debugln(String(F("D:New tcp client!")));
                    initConn(m_validatingConns[i]);
                    m_validatingConns.erase(m_validatingConns.begin()+i);
                }
                else
                {
                    debugln(F("D:Disconnecting"));
                    m_validatingConns[i].print("What!?\n");
                    m_validatingConns[i].stop();
                    m_validatingConns.erase(m_validatingConns.begin()+i);
                    debugln(String(F("D:bad client Disconected")));
                    if(m_validatedConns.size() == 0)
                    {
                        debugln("D:Last client disconnected");
                        i_jarvisDisConnected();

                    }
                    yield();
                }
            }
            else
            {
                debugln(String(F("D:unvalidated client Disconected")));
                m_validatingConns[i].print("What!?\n");
                m_validatingConns[i].stop();
                m_validatingConns.erase(m_validatingConns.begin()+i);
                if(m_validatedConns.size() == 0)
                {
                    debugln("D:Last client disconnected");
                    i_jarvisDisConnected();
                }
                yield();
            }
        }


        if(m_validatingConns.size() > 0)
        {
            m_validateTimeout -= updateInterval/1000.0f;
            debug("D:Validate Timeout:");
            debugln(m_validateTimeout);
            if(m_validateTimeout <= 0.0)
            {
                for(int c = 0 ; c < m_validatingConns.size() ; c++)
                {
                    debugln(String(F("D:Validate timeout for client!")));
                    m_validatingConns[0].print("bye!\n");
                    m_validatingConns[0].stop();
                    m_validatingConns.erase(m_validatingConns.begin());
                }
                if(m_validatedConns.size() == 0)
                {
                    debugln("D:Last client disconnected");
                    i_jarvisDisConnected();
                }
            }
        }
    }

    void manageClients()
    {
        validateNewClients();

        for(int i = (m_validatedConns.size() -1) ; i >= 0 ; i--) {
          //liberar las conexiones que se han cerrado
           if (!m_validatedConns[i].connected()){
              debugln(String(F("D:Client Disconnected")));
              m_validatedConns[i].stop();
              m_validatedConns.erase(m_validatedConns.begin()+i);

              //comprobamos si era el ultimo cliente conectado.
              if(m_validatedConns.size() == 0)
              {
                  debugln("D:Last client disconnected");
                  i_jarvisDisConnected();

              }
           }
        }

        while(m_server.hasClient())
        {//asignacion de nuevos clientes.
            WiFiClient serverClient = m_server.available();
            if(m_validatedConns.size()+m_validatingConns.size() < m_max_clients)
            {
                validateClient(serverClient);
            }
            else
            {//si ya se ha alcanzado el maximo se rechazan los nuevos clientes
                serverClient.println("Sorry, I'm full");
                serverClient.stop();
                debugln(String(F("D:Client rejected: max_clients")));
            }
        }
    }      


    void send(String str)//envia por wifi
    {
      m_statusLed.wifiTX();
      int len = str.length();
      char sbuf[len+1];
      str.toCharArray(sbuf,len+1);
      for(uint8_t i = 0; i < m_validatedConns.size() ; i++){
         m_validatedConns[i].write(sbuf, len);
        #ifdef EXTRA_CARRIAGE_RETURN
          m_validatedConns[i].write("\n", 1);
        #endif
          yield();
      }
    }

    void read()//lee desde el wifi
    {
      //check clients for data
      for(uint8_t i = 0; i < m_validatedConns.size(); i++){
        if (m_validatedConns[i].connected()){
          if(m_validatedConns[i].available()){
            m_statusLed.wifiRX();
            String buff;
            while(m_validatedConns[i].available())
            {
              char b = m_validatedConns[i].read();
              if(m_bridge)Serial.write(b);//Si esta en modo bridge copia los datos al serie
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
          for(int i = 0; i < m_validatedConns.size() ; i++)
          {
            m_validatedConns[i].write(sbuf, len);
            delay(1);
          }
        }
      } 
    }
#endif

    String localIP()
    {
      IPAddress ip;
      if     (m_lastConnectionStatus == 3)
        ip = WiFi.localIP();
      else if(m_lastConnectionStatus == 0)
        ip = WiFi.softAPIP();

      String result;
      result += ip[0];
      result += ".";
      result += ip[1];
      result += ".";
      result += ip[2];
      result += ".";
      result += ip[3];
      return result;
    }
    
  protected:
    String           m_serialBuffer;
    uint8_t          m_max_clients = 3;
    String           m_serialBufer;
    WiFiServer       m_server;

    std::vector<WiFiClient> m_validatedConns;
    std::vector<WiFiClient> m_validatingConns;
    float                  m_validateTimeout;
    webConfigurator         m_webServer;
    ESP8266HTTPUpdateServer m_httpUpdater;


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
      yield();
      WiFi.softAP(essid, pass);
      yield();
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
      yield();
      WiFi.begin(essid, pass);
      yield();
      m_server.begin();
      delete pass;
      delete essid;
    }
};
#endif

class espProxy : public communicationModule
{
  public:
    espProxy(int localPort, int ledStripPin,int ledNr, commModes mode = arduinoThoughProxy ) : communicationModule(mode, localPort,ledStripPin,ledNr) {;}
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
