#ifndef SPIFS_H
#define SPIFS_H
#include "settings.h"

class SPIFSStorage
{
public:
  SPIFSStorage()
  {
  }

  void setup()
  {
      Serial.print("Loading SPIFFS...");
      if(SPIFFS.begin())
      {
        Serial.println("OK!");
        Dir dir = SPIFFS.openDir("/");
        while (dir.next()) {
          String fileName = dir.fileName();
          size_t fileSize = dir.fileSize();
          Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
        }
        Serial.printf("\n");
      }
      else
      {
          Serial.println("FAIL!");
      }

  }

  String formatBytes(size_t bytes){
    if (bytes < 1024){
      return String(bytes)+"B";
    } else if(bytes < (1024 * 1024)){
      return String(bytes/1024.0)+"KB";
    } else if(bytes < (1024 * 1024 * 1024)){
      return String(bytes/1024.0/1024.0)+"MB";
    } else {
      return String(bytes/1024.0/1024.0/1024.0)+"GB";
    }
  }

private:

};



#endif // SPIFS_H

