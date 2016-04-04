#ifndef COMM
#define COMM

#ifdef ESP8266
#include "espWebServer.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266mDNS.h>
#include <ESP8266HTTPUpdateServer.h>
#include "spifsstorage.h"
#endif

#include "jarvisParser.h"
#include "ws2812led.h"
#include "eepromstorage.h"

//Status del wifi:
//WL_CONNECTED   3
//WL_IDLE_STATUS  0
//WL_NO_SSID_AVAIL  1
//WL_CONNECT_FAILED   4
//WL_CONNECTION_LOST  5
//WL_DISCONNECTED   6

extern uint16_t updateInterval;


class communicationModule : public jarvisParser , public nodeComponent
{
  public:

  enum commModes
  {
      nativeNode,
      espRepeater,
      arduinoThoughProxy
  };
    
    communicationModule(EEPROMStorage* eeprom) : jarvisParser(),
      m_eeprom(eeprom), m_ledStrip(eeprom->settings().ledStripPin,eeprom->settings().ledCount)
    , m_statusLed(&m_ledStrip,0,1,2)
    {
        m_spiFsStorage.setup();
        m_commMode = nativeNode;
        m_initDone = false;
        if(m_ledStrip.isValid())
        {
          debugln(String(F("I:-WS2812")));
          m_ledStrip.setBrightness(m_eeprom->settings().ledStripBrightness);
          m_ledStrip.setup();
          m_ledStrip.off();
          m_statusLed.controllerInit();
        }
        m_id = m_eeprom->settings().id;
    }

    void setAP(String essid,String pass)
    {

      m_essid = essid, m_pass = pass;
      m_eeprom->setWifiConfig(essid,pass,true);
      connectAP();
    }

    void setStation(String essid,String pass)
    {

      m_essid = essid, m_pass = pass;
      m_eeprom->setWifiConfig(essid,pass,false);
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
      m_statusLed.wifiInit();
      m_essid = m_eeprom->settings().wifiESSID;
      m_pass  = m_eeprom->settings().wifiPasswd;

      if(m_eeprom->settings().wifiAPMode)
      {
          debug("D:WifiAP: ");
          debugln(m_eeprom->settings().wifiESSID);
          connectAP();
      }
      else
      {
          debug("D:WifiClient, connecting to:");
          debugln(m_eeprom->settings().wifiESSID);
          connectStation();
      }

      m_lastConnectionStatus = 6;
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
              debugln(m_eeprom->settings().remotePort);
              i_wifiConnected();
          }
          else if (connstatus == 0)
          {
              debug(String(F("D: IP:")));
              debug(localIP());
              debug(String(F("  p:")));
              debugln(m_eeprom->settings().remotePort);
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
          if((m_reconnectTimer <= 0.0f) && (!validatingConn()))
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

    virtual String  localIP()           = 0;
    virtual bool    isConnected()       = 0;
    virtual int     connectionStatus()  = 0;
    virtual void    send(String str)    = 0;
    virtual void    read()              = 0;

  protected:
    EEPROMStorage*  m_eeprom;
    commModes       m_commMode;
    bool            m_bridge;
    String          m_essid;
    String          m_pass;
    bool            m_initDone;
    String          m_rxBuffer;

    uint8_t  m_lastConnectionStatus;
    bool     m_jarvisConnected          = false;
    bool     m_reconnectJarvis          = false;
    float    m_reconnectTimer           = 0.0f;
    ws2812Strip           m_ledStrip;
    ledStatusTrio         m_statusLed;
    SPIFSStorage          m_spiFsStorage;
    
    virtual void connectAP()      = 0;
    virtual void connectStation() = 0;

    virtual void connectToJarvis()  = 0;
    virtual bool validatingConn()   = 0;

    void i_wifiConnected()
    {
        debug("D: Starting mdns responder...  ");

        if(MDNS.begin(m_eeprom->settings().id))
        {
            debug("OK! : ");
            debug(m_eeprom->settings().id);
            debugln(".local");
        }
        else
        {
            debug("FAIL! : ");
            debugln(localIP());
        }

        if(m_lastConnectionStatus == 3)
        {
            m_statusLed.wifiClient();
            wifiConnected();
            connectToJarvis();
        }else{
            m_statusLed.wifiAutoConfig();
            wifiConnected();
        }
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
            m_reconnectTimer = 5.0f;
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



//Protocolo de nodo
    virtual void processNodeMsg(std::vector<String>& args)
    {
        if(args.size()<=0) return; // si no hay nada que procesar nada se hace

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
        else if(command == C_SAVE_EEPROM)
        {
          m_eeprom->storeSettings();
          yield();
          debugln("D:Settings saved.");
        }
        else if(command == C_CLEAR_EEPROM)
        {
          m_eeprom->clearEEPROM();
          yield();
          debugln("D:EEPROM Erased.");
        }
        else if(command == C_SET_CONFIG)
        {
            char data[sizeof(settingList)];
            for(int i = 0 ; (i < sizeof(data)) && (i < args.size()) ; i++)
                data[i] = (char)args[i].toInt();
            if(!m_eeprom->setSettings((uint8_t*)&data))
            {
                debugln("D:Bad Settings");
            }
        }
        else if(command == C_GET_CONFIG)
        {
          std::vector<String> args;
          args.push_back(C_CONFIG);
          int length = sizeof(settingList);
          uint8_t data[length];
          m_eeprom->getSettingsBytes((uint8_t*)&data);
          for(int i = 0 ; i < length; i++)
          {
            args.push_back(String(data[i]));
          }
          send(encodeJarvisMsg(args));
          yield();
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
        updateInterval = nup;
        m_eeprom->settings().updateInterval = nup;
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
        args.push_back(m_eeprom->settings().id);
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
    espNative(EEPROMStorage* settings) :communicationModule(settings),
        m_server(m_eeprom->settings().localPort),
        m_webServer(80)
    {
        m_bridge = false;
    }

    bool isConnected()      { return WiFi.status() == WL_CONNECTED; }

    int connectionStatus()  { return WiFi.status(); }

    void setup()
    {
      communicationModule::setup();
      m_webServer.setup();
      debugln("D:Setting update server");
      m_httpUpdater.setup(&m_webServer.webServer());
      yield();
      debug("D:Listening connections on port:");
      debugln(m_eeprom->settings().localPort);
      m_server.begin();
      MDNS.addService("http", "tcp", 80);
      debug("D: HTTPUpdateServer on http://<IP>/update");
      yield();
    }

    void update()
    {
      communicationModule::update();
      manageClients();
      m_webServer.handleClient();
      readSerial();
      parseBuffer(m_serialBuffer);
      yield();
    }

    void connectToJarvis()
    {
        if(connectionStatus() != 3) return; //Solo se inicia la conexion si se esta en modo cliente
        if(!m_initDone)                     //Para evitar problemas se espera a que se termine toda la inicializacion antes de conectar.
        {
            debugln("D:Wifi connected but setup not completed yet, will connect to Jarvis in ~10s");
            m_reconnectJarvis = true;
            m_reconnectTimer = 1.0f;
            return;
        }

        debug("D:Connecting to ");
        debug(m_eeprom->settings().remoteHost);
        debug(":");
        debugln(m_eeprom->settings().remotePort);
        WiFiClient client;
        if (client.connect(m_eeprom->settings().remoteHost, m_eeprom->settings().remotePort))
        {
            debugln("D:Socket connected, validating client...");
            validateClient(client);
        }else{
            debugln("D:Can't connect!");
            client.stop();
            m_reconnectJarvis = true;
            m_reconnectTimer = 5.0f;
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
                String buff;
                debugln("D:Validating client");
                while(m_validatingConns[i].available() && splitStr(buff,P_PACKETTERMINATOR).size() < 2)
                {
                  buff += (char)m_validatingConns[i].read();
                }

                String startstr         = P_PACKETSTART;
                String terminatorstr    = P_PACKETTERMINATOR;

                if(buff.length() > startstr.length()+terminatorstr.length())
                {//Limpiar los principios/finales de paquete.
                    buff.remove(0,startstr.length());

                    for(int i = 0 ; i < terminatorstr.length() ; i++)
                        buff.remove(buff.length()-1);
                }

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
      char sbuf[str.length()+1];
      str.toCharArray(sbuf,sizeof(sbuf));
      for(uint16_t i = 0; i < m_validatedConns.size() ; i++)
      {
         m_statusLed.wifiTX();
         m_validatedConns[i].write(sbuf, sizeof(sbuf));
#ifdef DEBUG_PROTOCOL
            debug("D:Send: ");
            debugln(str);
#endif
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
#ifdef DEBUG_PROTOCOL
            debug("D:read: ");
            debugln(buff);
#endif
          }
        }
      }
    }

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
    WiFiServer       m_server;

    std::vector<WiFiClient> m_validatedConns;
    std::vector<WiFiClient> m_validatingConns;
    float                   m_validateTimeout;
    espWebServer            m_webServer;
    ESP8266HTTPUpdateServer m_httpUpdater;


    void connectAP()
    {
        char cessid[m_essid.length()+1];
        m_essid.toCharArray(cessid, sizeof(cessid));
        char cpass[m_pass.length()+1];
        m_pass.toCharArray(cpass, sizeof(cpass));

      debug(String(F("D:AP: ")));
      debug(cessid);
      debug(String(F("#")));
      debugln(cpass);

      WiFi.disconnect();
      yield();
      WiFi.softAP(cessid, cpass);
      yield();
      m_server.begin();
    }
    
    void connectStation()
    {
        char cessid[m_essid.length()+1];
        m_essid.toCharArray(cessid, sizeof(cessid));
        char cpass[m_pass.length()+1];
        m_pass.toCharArray(cpass, sizeof(cpass));

        debug(String(F("D:AP: ")));
        debug(cessid);
        debug(String(F("#")));
        debugln(cpass);

        WiFi.softAPdisconnect(true);
        yield();
        WiFi.mode(WIFI_STA);
        WiFi.begin(cessid, cpass);
        yield();
        m_server.begin();;
    }
};

#else
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

#endif
