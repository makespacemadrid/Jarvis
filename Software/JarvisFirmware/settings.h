#ifndef SETTINGS
#define SETTINGS

#include <EEPROM.h> //WTF hay que hacer el include en el main!



#ifndef ESP8266
#include <avr/wdt.h>
#endif
enum jarvisModules
{
    unknownModule,
    espRepeaterModule,
    simpleSwitchModule,
    makeSwitchModule,
    airQualityModule,
    simplePowerControlModule,
    advancedPowerControlModule,
    coffeeMakerModule,
    ledPanelModule
};

struct settingList
{//Configuracion por defecto (Factory)
  //Datos de conexion
  char          id[15]          = "Configureme";
  char          remoteHost[17]  = "10.0.100.1";
  int           remotePort      = 31416;
  int           localPort       = 31416;
  //jarvisModules moduleType      = simplePowerControlModule;
  //jarvisModules moduleType      = makeSwitchModule;
  //jarvisModules moduleType      = coffeeMakerModule;
  jarvisModules moduleType      = ledPanelModule;
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
  #ifdef ESP8266
  //Pineado por defecto del ESP8266
  int   alivePin               =  2;
  int   relayPin               = -1;
  int   currentMeterPin        = -1;
  int   relayTemperatureSensor = -1;
  int   fanPin                 = -1;
  int   piezoPin               =  0;
  //int   ledStripPin            =  4;
  int   ledStripPin            =  15;
  int   factoryResetPin        =  2;
  #else
  //Pineado por defecto del arduino
  int   alivePin               = 13;
  int   relayPin               =  6;
  int   currentMeterPin        = -1;
  int   relayTemperatureSensor = -1;
  int   fanPin                 = -1;
  int   piezoPin               =  6;
  int   ledStripPin            =  9;
  int   factoryResetPin        =  -1;//8
  #endif

  //Configuracion
  int   ledStripLedNr          = 256;
  bool  bridgeMode             = false;
  float currentMeterFactor     = 29.296875f;
  int   currentMeterVolts      = 220;
  int   relayMaxAmps           = 15;
  bool  relayDimmable          = false;
  int   relayMaxTemp           = 60;
  int   ledNumber              =  3;
  int   magicNumber            = 31416; 
};

class EEPROMStorage
{
public:
  EEPROMStorage()
  {
    refresh();
  }

  settingList& settings()
  {
    return m_settings;
  }

  void refresh()
  {
    m_settings = getSettings();
  }
    
  static bool hasSettings()
  {
    settingList settings;
    settings.magicNumber = 0;
    //EEPROM.get(0,settings); // De momento dejo la EEPROM desactivada
    return settings.magicNumber == 31416;
  }
  
  static settingList getSettings()
  {
    settingList settings;
    settings.magicNumber = 0;
    //EEPROM.get(0,settings);  // De momento dejo la EEPROM desactivada
    if(settings.magicNumber == 31416)
      return settings;
    else
      return settingList(); // Si falla la comprobacion se devuelven los settings por defecto.
  }
  
  static void storeSettings(settingList settings)
  {
    EEPROM.put(0,settings);
  }

  void clearEEPROM()
  {
#ifndef ESP8266
    //no hay .length() en el ESP de momento
    for ( int i = 0 ; i < EEPROM.length() ; i++ )
      EEPROM.write(i, 0);
#endif
  }
private:
  settingList m_settings;
};


#endif
