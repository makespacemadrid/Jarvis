#include <EEPROM.h> //WTF hay que hacer el include en el main!

struct settingList
{//Configuracion por defecto (Factory)
  //Datos de conexion
  char    id[15]          = "Configureme";
  char    macAddr[18]     = "00:00:00:FA:BA:DA";
  char    remoteHost[17]  = "10.0.100.1";
  int     remotePort      = 31416;
  int     localPort       = 31416;
  
  #ifdef ESP8266
  //Pineado por defecto del ESP8266
  int   alivePin               =  2;
  int   relayPin               = -1;
  int   currentMeterPin        = -1;
  int   relayTemperatureSensor = -1;
  int   fanPin                 = -1;
  int   piezoPin               = -1;
  int   ledStripPin            = -1;
  int   factoryResetPin        = -1;
  #else
  //Pineado por defecto del arduino
  int   alivePin               = 13;
  int   relayPin               =  6;
  int   currentMeterPin        = -1;
  int   relayTemperatureSensor = -1;
  int   fanPin                 = -1;
  int   piezoPin               = -1;
  int   ledStripPin            =  9;
  int   factoryResetPin        = -1;
  #endif

  //Configuracion
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
  bool hasSettings()
  {
    settingList settings;
    EEPROM.get(0,settings);
    return settings.magicNumber == 31416;
  }
  
  settingList getSettings()
  {
    settingList settings;
    EEPROM.get(0,settings);
    if(settings.magicNumber == 31416)
      return settings;
    else
      return settingList();// Si falla la comprobacion se devuelven los settings por defecto.
  }
  
  void storeSettings(settingList settings)
  {
    EEPROM.put(0,settings);
  }

  void clearEEPROM()
  {
#ifndef ESP8266
    for ( int i = 0 ; i < EEPROM.length() ; i++ )
      EEPROM.write(i, 0);
#endif
  }
private:

};
