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
RTC_DS1307 RTC;  
#include <avr/interrupt.h>
#include <avr/sleep.h>

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
#define FLAMEpin 3 //Sensor de Llamas
#define WAKEUPpin 2//Pin de interrupcion para despertar al Arduino
#define RX 1//RX del puerto serial y tigger del modo ahorro de energia
#define TX 0//TX del puerto serial
#define MQ135PIN A0 //Pin del sensor NH3, Benceno, Alcohol
#define LDRpin A1 //Pin del sensor de Luz
#define MQ811pin A2 //Pin del sensor de CO2
#define MQ07pin A3//Pin del sensor de CO
#define SDA A4//SDA del I2C
#define SCL A5//SCL del I2C
#define DHTTYPE DHT22 //Configuracion del modelo de sensor DHT22

//Declaracion de Variables 

boolean debuggear;
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
   
   BT.begin(9600);//Inicializacion del puerto serial BT
   BT.println("Serial Port Open"); 
   
   dht.begin(); //Se inicia el sensor de temperatura y humedad
   BT.println("init DHT"); 
   
   Wire.begin();
   RTC.begin();//Inicializamos el módulo reloj  
   BT.println("Real clock init"); 
   
   // inicializacion de las variables
   debuggear = false; 
   Serial.println("init variable");
   
   //Iniciamos el control de interrupciones
   attachInterrupt(WAKEUPpin, wakeup, CHANGE);  
   attachInterrupt(FLAMEpin, llamas, RISING);  
 
}

void loop(){
  //Si se ha establecido una conexion Bluetooth, enviamos las lecturas  
  if (BT.available()){
    char c = BT.read();
    jarvisInterface(c);        
  }  
  
  if(debuggear){debuggerMode;}//Si esta activo el modo de debuggeo llamamos a la funcion
}



boolean checkExcLimit(int lectura, int limite){if(lectura>limite)return true;}//Comprueba si el valor pasado supera el limite
int getLight(){return analogRead(LDRpin);}//Funcion que devuelve la lectura de la intensidad de luz.
boolean getFlame(){return digitalRead(FLAMEpin);}//Funcion que devuelve la lectura del sensor de llamas.
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
void soundAlarm(int t){
  analogWrite(9,20);
  delay(t);
  analogWrite(9,20);
  delay (t);
}

//Alarma visual
void ligthAlarm(int t, int d){
  digitalWrite(luminoso, HIGH);
  delay(t);
  digitalWrite(luminoso, LOW);
  delay(d);
}

float calibrarMQ135(){  
  float rzero = mq135.getRZero();
  return rzero;//El valor devuelto por esta funcion debera ser cargado en MQ135.h -> #define RZERO ____
}

void setMAC(uint8_t mac){EEPROM.write(0, mac);}//metodo para asignar MAC de forma dinamica
uint8_t getMAC(){return  EEPROM.read(0);}//metodo que devuelve la MAC almacenada en la EPROM

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
    Serial.println(getFlame());
    unsigned long t1 = millis();    // Se toma el tiempo actual
    Serial.print("Todo medido en: ");
    Serial.print(t1-t0);
    Serial.println("ms");  
}

//Activar el modo de debuggeo
void disableDebuggerMode(){
  Serial.end();//Cerramos la conexion y liberamos los pines RX y TX
  digitalWrite(RX,HIGH);//Reactiva la alimentacion externa  
  debuggear = false;  
}

//Activa el modo de debuggeo
void enableDebuggerMode(){
  debuggear = true;
  Serial.begin(9600);//Inicializacion del puerto serial para debugger
  Serial.println("Serial Debugger Port Open"); 
}

boolean debuggerStatusMode(){return debuggear;} 

//Devuelve TRUE si hay alimentacion externa y FALSE en caso contrario
boolean externalVcc(){
  if(digitalRead(TX)==HIGH){return true;}
  else{return false;}
}
void enableExternalVcc(){digitalWrite(RX,HIGH);}
void disableExternalVcc(){digitalWrite(RX,LOW);}

//Devuelve el porcentaje de luz etectado
float getLightPerCent(int LDRpin){
   int light = analogRead(LDRpin);
   float lightPerCent = light/10.23;//calculamos el tanto por ciento de luz detectada
   return lightPerCent;
}

//Devuelve la lectura del CO en partes por millon
float getCO(int MQ07pin){
  int val = analogRead(MQ07pin);
  float ppm = map(val, 0, 1023, 0, 2000);//Mapeamos el valor leido a ppm
  return ppm;
}

//Devuelve la lectura del CO2 en partes por millon
float getCOO(int MQ811){
  int val = analogRead(MQ811);
  float ppm = map(val,0,1023,0,10000);
  return ppm;
}

void saveData(float datos[], String fichero){
  if (!SD.begin(CS)) {
    myFile = SD.open("registros.txt", FILE_WRITE);
    if (myFile) {
      myFile.print(getDate() + " - " + getTime() + " - ");  //Marcamos fecha y hora en el registro
      for(int i=0; i<sizeof(datos)/sizeof(float); i++){
        myFile.print(datos[i]);
        myFile.print(",");
       }
       myFile.println();
    }else{saveLog("Error al abrir el fichero de registros");}
  }else{saveLog("Error al iniciar la tarjeta SD");}
  myFile.close();
}

void saveLog(String msg){
  if (!SD.begin(CS)) {
    myFile = SD.open("log.txt", FILE_WRITE);
    if (myFile) {
      myFile.print(getDate() + " - " + getTime() + " - ");  //Marcamos fecha y hora en el registro
      myFile.println(msg);
    }else{alarm(10);}
  }else{alarm(10);}
  myFile.close();
}

//Vuelca el contenido de la tarjeta SD a la conexion
void downloadSD(){
  myFile = SD.open("registros.txt");   
  if (myFile) {
    //leemos los datos del fichero   
    while (myFile.available()) {BT.write(myFile.read());}  
    myFile.close();// cerramos el fichero  
  }else {Serial.println("opening error");}// Si el fichero no puede abrirse, mostramos un error.
}


//Muestra el contenido del fichero de log
void showLog(){
  myFile = SD.open("log.txt");  
  if (myFile) {
    //leemos los datos del fichero   
    while (myFile.available()) {BT.println(myFile.read());}  
    myFile.close();// cerramos el fichero  
  }else {Serial.println("opening error");}// Si el fichero no puede abrirse, mostramos un error.  

}

//Vuelva el contenido del fichero de log
void downloadSDlog(){
  myFile = SD.open("log.txt");  
  if (myFile) {
    //leemos los datos del fichero   
    while (myFile.available()) {BT.write(myFile.read());}  
    myFile.close();// cerramos el fichero  
  }else {Serial.println("opening error");}// Si el fichero no puede abrirse, mostramos un error.
}

//Devuelve un String con la fecha en formato dd/mm/aaaa
String getDate(){
   DateTime now = RTC.now();//Creamos un objeto que contiene la hora y fecha actual      
   String fecha = now.day()+"/";
   fecha = fecha+"/"+now.month();
   fecha = fecha + "/"+now.year();
   return fecha;
}

//Devuelve un String con la hora en formato hh/mm/ss
String getTime(){
  DateTime now = RTC.now();
  String hora = now.hour() + ":";
   hora = hora + ":" +now.minute();
   hora = hora + ":"+now.second();
   return hora;
}

void alarm(int alarm){
  /*
  0 -> Alarma de incendios
  1 -> Alarma de gases explosivos
  2 -> Alarma de CO
  3 -> Alarma de CO2
  4 -> Alarma de humo
  5 -> Alarma de temperatura
  6 -> Alarma de alimentacion
  7 -> Alarma de cambio de modo
  8 -> Alarma de modo ahorro
  9 -> Alarma de desabilitacion
  10 -> Mal funcionamiento de la tarjeta SD
  
  */
  //Tiempos de encedido y apagado de la alarma visual
  int   fireLight[]  =   {200,100};
  int   gasLight[]   =   {200,100};
  int   COLight[]    =   {200,100};
  int   COOLight[]   =   {200,100};
  int   smokeLight[] =   {200,100};
  int   tempLight[]  =   {200,100};
  int   vccLight[]   =   {200,100};
  int   modeLight[]  =   {200,100};
  int   lowEnergyModeLight[] = {200,100};
  int   disableLight[]       = {200,100};
  int   SDfailLight[]        = {200,100};
  
  //Frecuencia de sonido de al alarma acustica
  int   fireSound  = 200;  
  int   gasSound   = 200;  
  int   COSound    = 200;  
  int   COOSound   = 200;  
  int   smokeSound = 200;  
  int   tempSound  = 200;  
  int   vccSound   = 200;  
  int   modeSound  = 200;  
  int   lowEnergyModeSound  = 200;  
  int   disableSound        = 200;  
  int   SDfailSound         = 200;
  
  switch (alarm) {
  case 0:    //Alarma de incendios
    soundAlarm(fireSound);
    ligthAlarm(fireLight[0],fireLight[1]);
    break;    
  case 1:    //Alarma de gases explisivos
    soundAlarm(gasSound);
    ligthAlarm(gasLight[0],gasLight[1]);
    break;
  case 2:    // Alarma de monoxido de carbono
    soundAlarm(COSound);
    ligthAlarm(COLight[0],COLight[1]);
    break;
  case 3:    // Alarma de dioxido de carbono
    soundAlarm(COOSound);
    ligthAlarm(COOLight[0],COOLight[1]);
    break;
  case 4:    // Alarma de humo
    soundAlarm(smokeSound);
    ligthAlarm(smokeLight[0],smokeLight[1]);
    break;   
  case 5:  //Alarma de temperatura
   soundAlarm(tempSound);
    ligthAlarm(tempLight[0],tempLight[1]); 
    break;
  case 6:    // Alarma de alimentacion
    soundAlarm(vccSound);
    ligthAlarm(vccLight[0],vccLight[1]);
    break;
  case 7:    // Alarma de cambio de modo
    soundAlarm(modeSound);
    ligthAlarm(modeLight[0],modeLight[1]);
    break;
  case 8:    // Alarma de modo LowEnergy
    soundAlarm(lowEnergyModeSound);
    ligthAlarm(lowEnergyModeLight[0],lowEnergyModeLight[1]);
    break;
  case 9:    // Alarma de 
    soundAlarm(disableSound);
    ligthAlarm(disableLight[0],disableLight[1]);
    break;    
  case 10:    // Alarma de desabilitacion de la placa
    soundAlarm(SDfailSound);
    ligthAlarm(SDfailLight[0],SDfailLight[1]);
    break;  
  default:    // your hand is nowhere near the sensor
    Serial.println("bright");
    break;
    
  } 
  
}
float* getArrayLecturas(){float lecturas[] = {getHum(), getTemp(), getLight(), getFlame()}; }
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
  21 -> Volcado de registro de log
  
  */
  
  switch(c){
    case 0:      
      sendArray(getArrayLecturas());
      break;
    case 1:
      enableExternalVcc();
      break;
      
    case 2:
      disableExternalVcc();
      break;
      
    case 3:
      externalVcc();
      break;
      
    case 4:
      BT.println(getFlame());
      break;
      
    case 5:
      BT.println(getHum());
      break;
      
    case 6:
      BT.println(getTemp());
      break;
   
    case 7:
      soundAlarm(110);
      break;
      
    case 8:
      Reset();
      break;
      
    case 9:
      ligthAlarm(500,500);
      break;
      
    case 10:
      downloadSD();
      break;
      
    case 11:
      getMQ135(getTemp(),getHum());
      break;
      
    case 12:
      BT.print(getLight());
      BT.print(" (");
      BT.print(getLightPerCent(LDRpin));
      BT.println(" )");
      break;

    case 13:
      BT.println(getCO(MQ07pin));
      break;
      
    case 14:
      BT.println(getCOO(MQ811pin));
      break;
      
    case 15:
      BT.println(getDate() +" - "+ getTime());
      break;
      
    case 16:
      BT.println("¿Cuanto tiempo deseas mantener la placa desabilitada?(s)");
      int t; 
      t = BT.read();
      t = t * 1000;//Convertimos los segundos en milis
      disable(t);
      break;
      
    case 17:
      goToSleep();
      break;
      
    case 18:
      if(debuggerStatusMode()){disableDebuggerMode();}else{enableDebuggerMode();}
      break;
   
    case 19:
      BT.println(getMAC());
      break;
      
    case 20:
      BT.println("Escriba la MAC que desea asignar al modulo");
      uint8_t mac; 
      mac = BT.read();
      setMAC(mac);
      break;
      
    case 21:
      showLog();
      break;
      
    case 22:
      downloadSDlog();
      break;
      
    case 23:
    
      break;
      
    case 24:
    
      break;
      
    default:
      BT.print("Command not found");
      break;    
  }
}

void llamas(){alarm(0);}
void wakeup(){sleep_disable();}
void goToSleep(){
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); 
  sleep_enable(); 
}
