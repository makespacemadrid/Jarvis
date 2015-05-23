//Version 0.1 del control ambiental de Jarvis.
//Desarrollado por CrakerNano 4 Makespace Madrid

//Lets go!

//llamadas a la librerias
#include <SoftwareSerial.h>
#include <MQ135.h>
#include "DHT.h"
#include <Wire.h>  
#include "RTClib.h"  
#include <SD.h>  
RTC_DS1307 RTC;  

//contantes
#define SCLK 13//SCLK del puerto SPI
#define MISO 12//MISO del puerto SPI
#define MOSI 11//MOSI del puerto SPI
#define CS 10//CS del puerto SPI
#define luminoso 9//Led para señales visuales
#define resetPin 8//Pin para activar el reset de Arduino
#define BTtx 7//TX del puerto serial emulado
#define BTrx 6//RX del puerto serial emulado
#define acustico 5//Altavoz piezolectrico para señales acusticas
#define DHTPIN 4 //Pin del sensor de humedad y temperatura
#define flamepin 3 //Sensor de Llamas
#define WAKEUPpin 2//Pin de interrupcion para despertar al Arduino
#define RX 1//RX del puerto serial
#define TX 0//TX del puerto serial
#define MQ135PIN A0 //Pin del sensor NH3, Benceno, Alcohol
#define LDRpin A1 //Pin del sensor de Luz
#define MQ811pin A2 //Pin del sensor de CO2
#define MQ07pin A3//Pin del sensor de CO
#define SDA A4//SDA del I2C
#define SCL A5//SCL del I2C
#define DHTTYPE DHT22 //Configuracion del modelo de sensor DHT22

//Variables 
float temp, humedad;
int CO, COO;
boolean humo, llamas, debuggear;
int address;//direccion del modulo
String name; //Nombre del modulo
File myFile;  

//Declaracion de objetos
SoftwareSerial BT(10, 11); //virtualizamos un puerto serial para la conectividad Bluetooth (RX, TX)
MQ135 mq135 = MQ135(MQ135PIN);//Objeto de la libreria MQ135
DHT dht(DHTPIN, DHTTYPE); //Se inicia una variable que será usada por Arduino para comunicarse con el sensor

void setup(){
   
   //Serial.begin(57600);//Inicializacion del puerto serial para debugger
   //Serial.println("Serial Debugger Port Open"); 
   
   BT.begin(4800);//Inicializacion del puerto serial BT
   BT.println("Serial Port Open"); 
   
   dht.begin(); //Se inicia el sensor de temperatura y humedad
   BT.println("init DHT"); 
   
   Wire.begin();
   RTC.begin();//Inicializamos el módulo reloj  
   BT.println("Real clock init"); 
   
   // inicializacion de las variables
   temp = -273;
   humedad = -1;
   CO = -1;
   COO = -1;
   humo = true;
   debuggear = false; 
   Serial.println("init variable");
 
}

void loop(){
  //Si se ha establecido una conexion Bluetooth, enviamos las lecturas
  
  if (BT.available()){
    float lecturas[] = {getHum(), getTemp(), getLight(), llama()}; 
    sendArray(lecturas);    
  }  
  
  if(debuggear){debuggerMode;}//Si esta activo el modo de debuggeo llamamos a la funcion
}



boolean checkExcLimit(int lectura, int limite){if(lectura>limite)return true;}//Comprueba si el valor pasado supera el limite
int getLight(){return analogRead(LDRpin);}//Funcion que devuelve la lectura de la intensidad de luz.
boolean llama(){return digitalRead(flamepin);}//Funcion que devuelve la lectura del sensor de llamas.
float getTemp(){return dht.readTemperature();}//devuelve la temperatura medida
float getHum(){return dht.readHumidity();} //devuelve la humedad medida
void Reset(){digitalWrite(resetPin,HIGH);}//Funcion para resetear la placa de Arduino
void disable(int t){delay(t);}//Permite desabilitar el modulo por un periodo de tiempo

//Funcion para que devuelve el valor dado por el sensor MQ135
float getMQ135(float t, float h){
  mq135.getCorrectionFactor(t,h);
  float ppm = mq135.getPPM();
  return ppm;
}

//Alarma Sonora
void alarm(int t){
  analogWrite(9,20);
  delay(t);
  analogWrite(9,20);
  delay (t);
}

//Alarma visual
void destellos(int t, int d){
  digitalWrite(luminoso, HIGH);
  delay(t);
  digitalWrite(luminoso, LOW);
  delay(d);
}

float calibrarMQ135(){  
  float rzero = mq135.getRZero();
  return rzero;//El valor devuelto por esta funcion debera ser cargado en MQ135.h -> #define RZERO ____
}


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
//Modulo de debugger
void debuggerMode(){    
    unsigned long t0 = millis();    // Se toma el tiempo actual  
    Serial.print("Temperatura");
    Serial.println(getTemp());
    Serial.print("Humedad");
    Serial.print(getHum());
    Serial.println("%");
    Serial.print("CO");
    Serial.println(getMQ135(getTemp(), getHum()));
    Serial.print("CO2");
    Serial.println();
    Serial.print("CO");
    Serial.println();
    Serial.print("Deteccion de llamas:");
    Serial.println(llama());
    unsigned long t1 = millis();    // Se toma el tiempo actual
    Serial.print("Todo medido en: ");
    Serial.print(t1-t0);
    Serial.println("ms");  
}

//Activar el modo de debuggeo
void disableDebuggerMode(){
  Serial.end();//Cerramos la conexion y liberamos los pines RX y TX
  digitalWrite(RX,HIGH);//cambia el estado del biestable
  digitalWrite(RX,LOW);
  debuggear = false;  
}

void enableDebuggerMode(){
  debuggear = true;
  Serial.begin(9600);//Inicializacion del puerto serial para debugger
  Serial.println("Serial Debugger Port Open"); 
}

boolean externalVcc(){
  if(digitalRead(TX)==HIGH){return true;}
  else{return false;}
}
/*
void saveData(float datos[]){
  //Guardamos el registro de datos en la SD
  DateTime now = RTC.now();//Creamos un objeto que contiene la hora y fecha actual    
  //String ultimosRegistros = now.day() + "/" + now.month() + "/" + now.year() + " " +now.hour()+":"+now.minute()+":"+now.second()
  //if(!guardaDatos(ultimosRegistros,"registro.txt")){guardaDatos(datos[],"log.txt");}
}

boolean guardaDatos(float informacion[], String fichero){return true;}
void volcarDatos(){}
*/
