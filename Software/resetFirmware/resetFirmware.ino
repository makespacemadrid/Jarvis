
#include "FS.h"
#include <EEPROM.h>


void setup() {
  // put your setup code here, to run once:
    delay(5000);
    Serial.print("Reset EEPROM...");
    yield();
      EEPROM.begin(512);
      for ( int i = 0 ; i < 512 ; i++ )
        EEPROM.write(i, 0);
      EEPROM.end();

      Serial.print("Reset EEPROM...");
      SPIFFS.format();

      Serial.println("Done!!");
}

void loop() {
  // put your main code here, to run repeatedly:

}
