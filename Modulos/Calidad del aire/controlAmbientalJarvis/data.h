/*
data.h - Library .
Created by CrakerNano, May 20, 2015. Released into the public domain.
*/
#ifndef data_h
#define data_h
#include "Arduino.h"
#include <SD.h> 

class data{
   public: data(int pin);
     boolean saveData(float datos[]);
     boolean saveLog(String msg);
     boolean downloadSD();
     boolean showLog();
     boolean downloadSDlog();
     String getDate();
     String getTime();
     boolean checkExcLimit(int lectura, int limite);
     float* getArrayLecturas(float humedad,float temperatura, int luz, boolean llama);

   private:
     int _pin;
     File myFile; 
     int _CS;
};

#endif
