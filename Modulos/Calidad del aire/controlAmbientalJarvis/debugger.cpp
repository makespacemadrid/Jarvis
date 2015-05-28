/*
debugger.cpp - Library for debug air quality module. 
Created by Crakernano, May 28, 2015. Released into the public domain.
http://www.trastejant.es
*/
#include "Arduino.h"
#include "debugger.h"

debugger::debugger(int rx, int tx){
   _rx = rx;
   _tx = tx;
}

//Modulo de debugger
void debugger::debuggerMode(float temperatura, float humedad, float NH3, float CO, float COO, boolean flame){    
    unsigned long t0 = millis();    // Se toma el tiempo actual  
    Serial.print("Temperatura");
//  Serial.println(dht.readTemperature());
    Serial.println(temperatura);
    Serial.print("Humedad");
//    Serial.print(dht.readHumidity());
    Serial.println(humedad);
    Serial.println("%");
    Serial.print("CO");
//    Serial.println(getMQ135(dht.readTemperature(), dht.readHumidity()));
    Serial.println(NH3);
    Serial.print("CO2");
    Serial.println(COO);
    Serial.print("CO");
    Serial.println(CO);
    Serial.print("Deteccion de llamas:");
//  Serial.println(getFlame());
    Serial.println(flame);
    unsigned long t1 = millis();    // Se toma el tiempo actual
    Serial.print("Todo medido en: ");
    Serial.print(t1-t0);
    Serial.println("ms");  
}

//Activar el modo de debuggeo
void debugger::disableDebuggerMode(){
  Serial.end();//Cerramos la conexion y liberamos los pines RX y TX
  digitalWrite(_rx,HIGH);//Reactiva la alimentacion externa  
  _debuggear = false;  
}

//Activa el modo de debuggeo
void debugger::enableDebuggerMode(){
  _debuggear = true;
  Serial.begin(9600);//Inicializacion del puerto serial para debugger
  Serial.println("Serial Debugger Port Open"); 
}

boolean debugger::debuggerStatusMode(){return _debuggear;} 

