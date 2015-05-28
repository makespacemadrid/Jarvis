//Version 0.1 del control ambiental de Jarvis.
//Desarrollado por CrakerNano 4 Makespace Madrid

//Lets go!

//llamadas a la librerias
#include <SoftwareSerial.h>
#include <EEPROM.h>
#include <MQ135.h>
#include "DHT.h"
#include <Wire.h>  
#include "RTClib.h"  
#include <SD.h>    
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "MQ07.h"
#include "alarm.h"
#include "MG811.h"
#include "data.h"
#include "connection.h"
#include "LDR.h"
#include "energy.h"
#include "reboot.h"
#include "jarvisTool.h"
#include "debugger.h"

//CONTANTES-------------------------------------------------------------->
//pines
#define SCLK 13//SCLK del puerto SPI
#define MISO 12//MISO del puerto SPI
#define MOSI 11//MOSI del puerto SPI
#define CS 10//CS del puerto SPI
#define luminoso 9//Led para señales visuales
#define RESETpin 8//Pin para activar el reset de Arduino
#define BTtx 7//TX del puerto serial emulado
#define BTrx 6//RX del puerto serial emulado
#define acustico 5//Altavoz piezolectrico para señales acusticas
#define DHTPIN 4 //Pin del sensor de humedad y temperatura
#define FLAMEpin 3 //Sensor de Llamas
#define WAKEUPpin 2//Pin de interrupcion para despertar al Arduino
#define RX 1//RX del puerto serial y tigger del modo ahorro de energia
#define TX 0//TX del puerto serial
#define MQ135PIN A0 //Pin del sensor NH3, Benceno, Alcohol
#define LDRpin A1 //Pin del sensor de Luz
#define MG811pin A2 //Pin del sensor de CO2
#define MQ07pin A3//Pin del sensor de CO
#define SDA A4//SDA del I2C
#define SCL A5//SCL del I2C

//Limites
#define NH3LIMIT 15000
#define COOLIMIT 15000
#define COLIMIT  15000
#define TEMPMAX 40
#define TEMPMIN 3

//Otros
#define DHTTYPE DHT22 //Configuracion del modelo de sensor DHT22

//Declaracion de Variables 

boolean debuggear;
int tRefresh;


//Declaracion de objetos
SoftwareSerial BT(10, 11); //virtualizamos un puerto serial para la conectividad Bluetooth (RX, TX)
MQ135 mq135 = MQ135(MQ135PIN);//Objeto de la libreria MQ135
DHT dht(DHTPIN, DHTTYPE); //Se inicia una variable que será usada por Arduino para comunicarse con el sensor
MQ07 mq07(MQ07pin);
alarm alarmas(luminoso, acustico);
MG811 mg811(MG811pin);
data jarvisData(CS);
connection link;
LDR ldr(LDRpin);
energy energy(TX,RX);
reboot reboot(RESETpin);
jarvisTool util;
debugger bug(RX,TX);

void setup(){
 
   
   BT.begin(9600);//Inicializacion del puerto serial BT
   BT.println("Serial Port Open"); 
   
   dht.begin(); //Se inicia el sensor de temperatura y humedad
   BT.println("init DHT"); 
   
   Wire.begin();
   BT.println("Real clock init"); 
   
   // inicializacion de las variables
   debuggear = false; 
   Serial.println("init variable");
   
   //Iniciamos el control de interrupciones
   attachInterrupt(WAKEUPpin, wakeup, CHANGE);  
   attachInterrupt(FLAMEpin, llamas, RISING);   
   Serial.println("Attach Interrupt OK");
   
   //Cargamos los valores almacenados en la EEPROM
   tRefresh = 1000;//valor por defecto para el primer arranque
   tRefresh = EEPROM.read(1);
   Serial.println("Load EEPROM values");
   
}

void loop(){
  //Si se ha establecido una conexion Bluetooth, enviamos las lecturas  
  if (BT.available()){
    char c = BT.read();
    jarvisInterface(c);        
  }else{
    //Si no hay conexion, entramos en modo autonomo      
    if(checkExcLimit(getMQ135(dht.readTemperature(),dht.readHumidity()),NH3LIMIT)){alarmas.typeAlarm(1);}//Comprobacion de los niveles de calidad del aire
    if(checkExcLimit(mq07.getCO(),COLIMIT)){alarmas.typeAlarm(2);}//Comprobacion de los niveles de CO
    if(checkExcLimit(mg811.getCOO(),COOLIMIT)){alarmas.typeAlarm(3);} //Comprobacion de los niveles de CO2 
    
    delay(tRefresh);//Tiempo de espera entre refrescos
  }
  
  //Comprobamos si el modo de debuggeo esta activo 
  if(bug.debuggerStatusMode()){bug.debuggerMode(dht.readTemperature(),dht.readHumidity(),getMQ135(dht.readTemperature(),dht.readHumidity()),mq07.getCO(),mg811.getCOO(),getFlame());}//Si esta activo el modo de debuggeo llamamos a su funcion funcion
}



boolean checkExcLimit(int lectura, int limite){if(lectura>limite)return true;}//Comprueba si el valor pasado supera el limite
boolean getFlame(){return digitalRead(FLAMEpin);}//Funcion que devuelve la lectura del sensor de llamas.


//Funcion para que devuelve el valor dado por el sensor MQ135
float getMQ135(float t, float h){
  mq135.getCorrectionFactor(t,h);
  float ppm = mq135.getPPM();
  return ppm;
}


float calibrarMQ135(){  
  float rzero = mq135.getRZero();
  return rzero;//El valor devuelto por esta funcion debera ser cargado en MQ135.h -> #define RZERO ____
}

//void setRefresh(int t){EEPROM.write(1, t);}
void sendArray(float lecturas[]){

  BT.print('#');//Caracter de inicializacion de la cadena    
    for(int i=0; i<sizeof(lecturas)/sizeof(float); i++){
      BT.print(lecturas[i]);
      BT.print('+');    
    }
  BT.print('~'); //caracter de final de cadena
  BT.println();
  delay(10);  
}


float* getArrayLecturas(){float lecturas[] = {dht.readHumidity(), dht.readTemperature(), ldr.getLight(), getFlame()}; }

void jarvisInterface(int c){
  /*
  0 -> Envia un array con todas las lecturas
  1 -> Activa la alimentacion externa
  2 -> Desactiva la alimentacion externa
  3 -> Devuelve el estado de la alimentacion externa
  4 -> Devuelva la llectura del sensor de llamas
  5 -> devuelve la lectura de la humedad
  6 -> Devuelve la lectura de la temperatura
  7 -> Prueba del zumbador
  8 -> Producen un reset en la placa de Arduino
  9 -> Prueba la alarma visual
  10 -> Volcado de datos de la tarjeta SD
  11 -> Lectura de la calidad del aire
  12 -> Lectura de la cantidad de luz
  13 -> Lectura del sensor de CO 
  14 -> Lectura del sensor de COO
  15 -> Devuelve la hora del modulo
  16 -> Desabilita la placa
  17 -> Coloca el Arduino en Sleep  
  18 -> Activa/Desactiva el modo de debuggeo
  19 -> Devuelve la MAC del modulo
  20 -> Asigna una MAC al modulo
  21 -> Mostrar registro de log
  22 -> Volcado del registro del log
  23 -> Asignar tiempo de refresco
  
  */
  
  switch(c){
    case 0:      
      sendArray(getArrayLecturas());
      break;
    case 1:
      energy.enableExternalVcc();
      break;
      
    case 2:
      energy.disableExternalVcc();
      break;
      
    case 3:
      energy.externalVcc();
      break;
      
    case 4:
      BT.println(getFlame());
      break;
      
    case 5:
      BT.println(dht.readHumidity());
      break;
      
    case 6:
      BT.println(dht.readTemperature());
      break;
   
    case 7:
      alarmas.soundAlarm(110);
      break;
      
    case 8:
      reboot.rebootNow();
      break;
      
    case 9:
      alarmas.ligthAlarm(500,500);
      break;
      
    case 10:
      jarvisData.downloadSD();
      break;
      
    case 11:
      getMQ135(dht.readTemperature(),dht.readHumidity());
      break;
      
    case 12:
      BT.print(ldr.getLight());
      BT.print(" (");
      BT.print(ldr.getLightPerCent());
      BT.println(" )");
      break;

    case 13:
      BT.println(mq07.getCO());
      break;
      
    case 14:
      BT.println(mg811.getCOO());
      break;
      
    case 15:
      BT.println(jarvisData.getDate() +" - "+ jarvisData.getTime());
      break;
      
    case 16:
      BT.println("¿Cuanto tiempo deseas mantener la placa desabilitada?(s)");
      int t; 
      t = BT.read();
      t = t * 1000;//Convertimos los segundos en milis
      util.disable(t);
      break;
      
    case 17:
      util.goToSleep();
      break;
      
    case 18:
      if(bug.debuggerStatusMode()){bug.disableDebuggerMode();}else{bug.enableDebuggerMode();}
      break;
   
    case 19:
      BT.println(link.getMAC());
      break;
      
    case 20:
      BT.println("Escriba la MAC que desea asignar al modulo");
      uint8_t mac; 
      mac = BT.read();
      link.setMAC(mac);
      break;
      
    case 21:
      jarvisData.showLog();
      break;
      
    case 22:
      jarvisData.downloadSDlog();
      break;
      
    case 23:
      BT.println("Escriba el tiempo de refresco que desea para el modulo(s)");      
      t = BT.read();
      util.setRefresh(t);
      break;
      
    case 24:
      BT.println("HOLA!");
      break;
      
    default:
      BT.print("Command not found");
      break;    
  }
}

void llamas(){alarmas.typeAlarm(0);}//Dispara una alarma de tipo fuego si se detectan llamas
void wakeup(){sleep_disable();}//reactiva Arduino

