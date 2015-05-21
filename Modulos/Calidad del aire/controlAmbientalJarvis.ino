//Version 0.1 del control ambiental de Jarvis.
//Desarrollado por CrakerNano 4 Makespace Madrid

//Lets go!

//llamadas a la librerias
#include <SoftwareSerial.h>


//Variables 
float temp, humedad;
int CO, COO;
boolean humo, debuggear;
int address;//direccion del modulo
String name; //Nombre del modulo

//virtualizamos un puerto serial para la conectividad Bluetooth
SoftwareSerial BT(10, 11); // RX, TX

void setup(){
 //Inicializacion del puerto serial para debugger
 Serial.begin(57600);
 Serial.println("Serial Debugger Port Open");
 
 //Inicializacion del puerto serial BT
 BT.begin(4800);
 BT.println("Serial Port Open");
 
 // inicializacion de las variables
 temp = -273;
 humedad = 101;
 CO = -1;
 COO = -1;
 humo = true;
 debuggear = false;
 
 Serial.println("Variables init already");
 
}

void loop(){
  //Si se ha establecido una conexion Bluetooth, enviamos las lecturas
if (BT.available()){}

//Si esta activo el modo de debuggeo llamamos a la funcion
if(debuggear == true){debuggerMode;}
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

void checkCO(int co){
  

}

void checkFire(boolean humo){
if (humo){
  //Activamos avisos sonoros y acusticos
  
  //Mandamos una alerta al resto del sitema con maxima prioridad
  
}
}
