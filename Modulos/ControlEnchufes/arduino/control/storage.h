#include <EEPROM.h> //WTF hay que hacer el include en el main!

struct settingList
{
  char  id[15]                 = "Configureme";
  char  macAddr[18]            = "00:00:00:FA:BA:DA";
  
  int   relayPin               = -1;
  int   currentMeterPin        = -1;
  float currentMeterFactor     = 29.296875f;
  int   currentMeterVolts      = 220;
  int   relayMaxAmps                = 15;
  bool  relayDimmable               = false;
  int   relayTemperatureSensor = -1;
  int   relayMaxTemp           = 60;
  int   fanPin                 = -1;
  
  int   piezoPin               = -1;
  
  int   ledStripPin            = -1;
  int   ledNumber              =  1;
  int magicNumber = 31416; 
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
    for ( int i = 0 ; i < EEPROM.length() ; i++ )
    EEPROM.write(i, 0);
  }
private:

};
