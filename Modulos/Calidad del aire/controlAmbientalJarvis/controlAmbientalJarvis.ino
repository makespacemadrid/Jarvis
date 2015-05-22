//Version 0.1 del control ambiental de Jarvis.
//Desarrollado por CrakerNano 4 Makespace Madrid

//Lets go!

//llamadas a la librerias
#include <SoftwareSerial.h>
#include <MQ135.h>
#include "DHT.h"

//contantes
#define luminoso 13
#define resetPin 12
#define BTtx 11
#define BTrx 10
#define acustico 9
#define DHTPIN 8 //Seleccionamos el pin en el que se //conectará el sensor
#define flamepin 3 //Llamas
#define WAKEUPpin 2//Pin de interrupcion para despertar al Arduino
#define RX 1
#define TX 0
#define MQ135PIN A0 //NH3, Benceno, Alcohol
#define LDRpin A1 //Luz
#define MQ811pin A2 //CO2
#define MQ07pin A3//CO
#define DHTTYPE DHT22 //Se selecciona el modelo del DHT

//Variables 
float temp, humedad;
int CO, COO;
boolean humo, llamas, debuggear;
int address;//direccion del modulo
String name; //Nombre del modulo

//Declaracion de objetos
SoftwareSerial BT(10, 11); //virtualizamos un puerto serial para la conectividad Bluetooth (RX, TX)
MQ135 mq135 = MQ135(MQ135PIN);//Objeto de la libreria MQ135
DHT dht(DHTPIN, DHTTYPE); //Se inicia una variable que será usada por Arduino para comunicarse con el sensor

void setup(){
   
   Serial.begin(57600);//Inicializacion del puerto serial para debugger
   Serial.println("Serial Debugger Port Open"); 
   
   BT.begin(4800);//Inicializacion del puerto serial BT
   BT.println("Serial Port Open"); 
   
   dht.begin(); //Se inicia el sensor de temperatura y humedad
   BT.println("init DHT"); 
 
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
  Serial.print("Temperatura");
  Serial.println(temp);
  Serial.print("CO");
  Serial.println(CO);
  Serial.print("CO2");
  Serial.println(COO);
  Serial.print("humo detectado");
  Serial.println(humo);
  delay(5000);
}
