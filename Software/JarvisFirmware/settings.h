#ifndef FIRMWARESETTINGS
#define FIRMWARESETTINGS

#ifndef ESP8266
#ifndef __AVR_ATmega2560__
//buscar otra etiqueta que descarte el arduino
//Por alguna razon no soy capaz de hacerlo funcionar con #ifdef QT
    #include <QObject>
    typedef quint16 uint16_t;
#endif
#endif

enum jarvisModules
{// el orden de los elementos tiene que coincidir con el de la ventana de configuracion del lado del server
 // Añadir elementos por el final, no reordenarlos.
    unknownModule,
    unConfiguredModule,
    simpleSwitchModule,
    makeSwitchModule,
    airQualityModule,
    simplePowerControlModule,
    advancedPowerControlModule,
    coffeeMakerModule,
    ledNotificationPanelModule,
    ledPanelModule,
    testNodeModule,
    termometroNodeModule
};

struct settingList
{//Configuracion por defecto (Factory)
  //Datos de conexion
  jarvisModules moduleType    = unConfiguredModule;
  char          id[20]          = "Configureme";
  char          wifiESSID[25]   = "ConfigureMe";
  char          wifiPasswd[25]  = "configureme";
  bool          wifiAPMode      = true;
  char          remoteHost[25]  = "Jarvis";
  uint16_t      remotePort      = 31416;
  uint16_t      localPort       = 31416;
  uint16_t      updateInterval  = 25;

  int8_t            alivePin            =  2;
  int8_t            piezoPin            = -1;
  int8_t            ledStripPin         = 13;
  float             ledStripBrightness  = 0.05f;
  uint16_t          ledCount            = 3;

  int8_t           buttonPins[10]      = {  16,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1};
  int8_t           tempSensorPins[10]  = {   5,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1};
  int8_t           relaypins[10]       = {  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1};

  uint16_t      magicNumber            = 31415;

  /**
Tabla de correspondencia de pines nodemcu arduino
0 [*]  GPIO16  
1  GPIO5 
2  GPIO4
3  GPIO0
4  GPIO2
5  GPIO14 (SPI CLK)    
6  GPIO12 (SPI MISO)   
7  GPIO13 (SPI MOSI)   
8  GPIO15 (SPI CS)
9  GPIO3 (UART RX)
10 GPIO1 (UART TX)
11 GPIO9
12  GPIO10
**/  

//Pineado por defecto de los nodos ESP8266
//  int   alivePin               =  2;
// int   ledStripPin            =  4; //makeswitch /coffeemaker
//    int   ledStripPin            =  13; // LedPanel
//      DHT makeswitch  5
//  dht ledPanel    12
//  dht termimetro  2


};

#endif
