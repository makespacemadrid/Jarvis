/*
alarm.h.
Created by CrakerNano, May 27, 2015. Released into the public domain.
*/
#ifndef alarm_h
#define alarm_h

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

class alarm{
   public:alarm(int LEDpin, int altavozPin);
   void typeAlarm(int type);
   void soundAlarm(int t);
   void soundAlarm(int t, int frecuency);
   void ligthAlarm(int t, int d);
   void soundAlarm(int t1, int frecuency, int repeat);
   
   private:
    int _altavozPin;
    int _LEDpin;
   
    int   fireLight[];    
    int   gasLight[];
    int   COLight[];
    int   COOLight[];
    int   smokeLight[];
    int   tempLight[];
    int   vccLight[];
    int   modeLight[];
    int   lowEnergyModeLight[];
    int   disableLight[];
    int   SDfailLight[];  

    int   fireSound;  
    int   gasSound;  
    int   COSound;  
    int   COOSound;  
    int   smokeSound;  
    int   tempSound;  
    int   vccSound;  
    int   modeSound;  
    int   lowEnergyModeSound;  
    int   disableSound;  
    int   SDfailSound;
    
    int t;
    int d;
    int frecuency;
    int repeat;
};

#endif
