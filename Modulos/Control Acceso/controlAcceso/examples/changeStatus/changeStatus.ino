#include <puertaInterior.h>

puertaInterior puerta(2,3);

void setup(){}

void loop(){
  if(getDoorStatus()){closeDoor();}else{openDoor();}
}
