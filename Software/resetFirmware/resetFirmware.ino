
#include "FS.h"
#include <EEPROM.h>


void setup() {
  // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.print("Reset EEPROM...");
    yield();
      EEPROM.begin(512);
      for ( int i = 0 ; i < 512 ; i++ )
        EEPROM.write(i, 0);
      EEPROM.end();

      Serial.print("Reset SPIFS...");
      SPIFFS.format();

      Serial.println("Done!!");
}

void loop() {
  // put your main code here, to run repeatedly:
    Serial.println("Nothing else to do, repeat or load firmware now :)");
    delay(10000);
}
