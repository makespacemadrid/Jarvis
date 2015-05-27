/*
data.cpp - Library for . 
Created by Crakernano, May 20, 2015. Released into the public domain.
http://www.trastejant.es
*/
#include "Arduino.h"
#include "data.h"
#include <SD.h> 
#include <Wire.h>  
#include "RTClib.h" 
RTC_DS1307 RTC; 

data::data(int pin){
   pinMode(pin, INPUT);
   _CS = pin;
   RTC.begin();//Inicializamos el módulo reloj
}

boolean data::saveData(float datos[]){
  if (!SD.begin(_CS)) {
    myFile = SD.open("registros.txt", FILE_WRITE);
    if (myFile) {
      myFile.print(getDate() + " - " + getTime() + " - ");  //Marcamos fecha y hora en el registro
      for(int i=0; i<sizeof(datos)/sizeof(float); i++){
        myFile.print(datos[i]);
        myFile.print(",");
       }
       myFile.println();
    }else{return false;}
  }else{return false;}
  myFile.close();
  return true;
}

boolean data::saveLog(String msg){
  if (!SD.begin(_CS)) {
    myFile = SD.open("log.txt", FILE_WRITE);
    if (myFile) {
      myFile.print(getDate() + " - " + getTime() + " - ");  //Marcamos fecha y hora en el registro
      myFile.println(msg);
    }else{return false;}
  }else{return false;}
  myFile.close();
  return true;
}

//Vuelca el contenido de la tarjeta SD a la conexion
byte data::downloadSD(){
  myFile = SD.open("registros.txt");   
  if (myFile) {
    //leemos los datos del fichero   
    while (myFile.available()) {return myFile.read();}  
    myFile.close();// cerramos el fichero  
  }else {return '#';}// Si el fichero no puede abrirse, mostramos un error.
}


//Muestra el contenido del fichero de log
byte data::showLog(){
  myFile = SD.open("log.txt");  
  if (myFile) {
    //leemos los datos del fichero   
    while (myFile.available()) {return myFile.read();}  
    myFile.close();// cerramos el fichero     
  }else {return '#';}// Si el fichero no puede abrirse, mostramos un error.  

}

//Vuelva el contenido del fichero de log
byte data::downloadSDlog(){
  myFile = SD.open("log.txt");  
  if (myFile) {
    //leemos los datos del fichero   
    while (myFile.available()) {return myFile.read();}  
    myFile.close();// cerramos el fichero  
  }else {return '#';}// Si el fichero no puede abrirse, mostramos un error.
}

//Devuelve un String con la fecha en formato dd/mm/aaaa
String data::getDate(){
   DateTime now = RTC.now();//Creamos un objeto que contiene la hora y fecha actual      
   String fecha = now.day()+"/";
   fecha = fecha+"/"+now.month();
   fecha = fecha + "/"+now.year();
   return fecha;
}

//Devuelve un String con la hora en formato hh/mm/ss
String data::getTime(){
  DateTime now = RTC.now();
  String hora = now.hour() + ":";
   hora = hora + ":" +now.minute();
   hora = hora + ":"+now.second();
   return hora;
}
